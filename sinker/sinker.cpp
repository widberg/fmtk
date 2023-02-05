#include <windows.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <istream>
#include <hashlibpp.h>

#include "sinker.hpp"

namespace sinker
{
    std::ostream &operator<<(std::ostream &out, attribute_value_t const &attribute_value) {
            if (std::holds_alternative<expression_value_t>(attribute_value))
            {
                out << std::get<expression_value_t>(attribute_value);
            }
            else if (std::holds_alternative<bool>(attribute_value))
            {
                out << std::boolalpha << std::get<bool>(attribute_value);
            }
            else
            {
                out << "\"" << std::get<std::string>(attribute_value) << "\"";
            }
            return out;
    }

    Symbol *Module::get_symbol(std::string_view symbol_name)
    {
        for (Symbol &symbol : symbols)
        {
            if (symbol_name == symbol.get_name())
            {
                return &symbol;
            }
        }
        return nullptr;
    }
    void Module::emplace_symbol(std::string const &name, std::string const &type)
    {
        symbols.push_back(Symbol(name, type, this));
    }
    void Module::add_variant(std::string const &name, std::string const &hash)
    {
        variants.insert({name, hash});
    }
    bool Module::has_variant(std::string_view name) const
    {
        return variants.count(name);
    }
    void Module::dump(std::ostream &out) const
    {
        out << "module " << name;
        if (lpModuleName)
        {
            out << ", \"" << lpModuleName.value() << "\"";
        }
        out << ";\n";

        for (auto const &attribute : get_attributes())
        {
            out << "set " << name << ", " << attribute.first << ", " << attribute.second << ";\n";
        }
        
        for (auto it = tags.begin(); it != tags.end(); ++it)
        {
            out << "tag " << name << ", " << *it << ";\n";
        }

        for (auto variant : variants)
        {
            out << "variant " << name << ", " << variant.first << ", \"" << variant.second << "\";\n";
        }

        for (Symbol const &symbol : symbols)
        {
            symbol.dump(out);
        }
    }


    identifier_set_t const& Context::get_symbol_tags() const
    {
        return symbol_tags;
    }

    void Module::dump_def(std::ostream &out) const
    {
        out << "#ifndef SINKER_" << name << "_SYMBOL\n";
        out << "#define SINKER_" << name << "_SYMBOL(symbol_name, symbol_type)\n";
        out << "#endif\n";
        for (auto it = get_context()->get_symbol_tags().begin(); it != get_context()->get_symbol_tags().end(); ++it)
        {
            out << "#ifndef SINKER_" << name << "_TAG_" << *it << "_SYMBOL\n";
            out << "#define SINKER_" << name << "_TAG_" << *it << "_SYMBOL(symbol_name, symbol_type)\n";
            out << "#endif\n";
        }
        out << "SINKER_MODULE(" << name << ")\n";

        for (auto it = tags.begin(); it != tags.end(); ++it)
        {
            out << "SINKER_TAG_" << *it << "_MODULE(" << name << ")\n";
        }

        for (Symbol const &symbol : symbols)
        {
            symbol.dump_def(out);
        }

        for (auto it = get_context()->get_symbol_tags().rbegin(); it != get_context()->get_symbol_tags().rend(); ++it)
        {
            out << "#undef SINKER_" << name << "_TAG_" << *it << "_SYMBOL\n";
        }
        out << "#undef SINKER_" << name << "_SYMBOL\n";
    }
    std::optional<expression_value_t> Module::get_relocated_base_address() const
    {
        return relocated_base_address;
    }
    HMODULE Module::get_hModule() const
    {
        return hModule;
    }
    std::optional<expression_value_t> Module::get_preferred_base_address() const
    {
        return preferred_base_address;
    }
    std::string const &Module::get_name() const
    {
        return name;
    }
    std::string const &Module::get_real_variant() const
    {
        return real_variant;
    }
    
    bool Module::concretize()
    {
        HMODULE tmphModule;
        if (lpModuleName) {
            tmphModule = GetModuleHandleA(lpModuleName.value().c_str());
        } else {
            tmphModule = GetModuleHandleA(NULL);
        }

        if(tmphModule == NULL)
        {
            return false;
        }
        
        char path[MAX_PATH + 1];
        if (!GetModuleFileNameA(tmphModule, path, MAX_PATH + 1))
        {
            return false;
        }

        sha256wrapper sha256;

        try
        {
            std::string hash = sha256.getHashFromFile(path);
            for (auto variant : variants)
            {
                if (variant.second == hash)
                {
                    real_variant = variant.first;
                    break;
                }
            }
        } catch(hlException &e) {
            std::cerr << "Error("  << e.error_number() << "): " << e.error_message() << std::endl;
            return false;
        }

        IMAGE_DOS_HEADER* pDOSHeader = (IMAGE_DOS_HEADER*)tmphModule;
        IMAGE_NT_HEADERS* pNTHeaders =(IMAGE_NT_HEADERS*)((BYTE*)pDOSHeader + pDOSHeader->e_lfanew);

        preferred_base_address = pNTHeaders->OptionalHeader.ImageBase;
        relocated_base_address = (expression_value_t)tmphModule;
        hModule = tmphModule;

        return true;
    }

    bool Module::is_concrete() const
    {
        return hModule;
    }

    Module *Symbol::get_module() const
    {
        return module;
    }

    void Symbol::add_address(identifier_set_t const &variant_set, std::shared_ptr<Expression> expression)
    {
        addresses.push_back({variant_set, std::move(expression)});
    }

    std::map<std::string, attribute_value_t, std::less<>> const &Attributable::get_attributes() const
    {
        return attributes;
    }

    void Symbol::dump_def(std::ostream &out) const
    {
        out << "SINKER_SYMBOL(" << module->get_name() << ", " << name << ", " << type << ")\n";
        for (auto it = tags.begin(); it != tags.end(); ++it)
        {
            out << "SINKER_TAG_" << *it << "_SYMBOL(" << module->get_name() << ", " << name << ", " << type << ")\n";
        }
        out << "SINKER_" << module->get_name() << "_SYMBOL(" << name << ", " << type << ")\n";
        for (auto it = tags.begin(); it != tags.end(); ++it)
        {
            out << "SINKER_" << module->get_name() << "_TAG_" << *it << "_SYMBOL(" << name << ", " << type << ")\n";
        }
    }

    void Context::dump(std::ostream &out) const
    {
        for (Module const *module : modules)
        {
            module->dump(out);
        }
    }

    void Context::dump_def(std::ostream &out) const
    {
        out <<
            R"%(#ifndef SINKER_MODULE
#define SINKER_MODULE(module_name)
#endif
#ifndef SINKER_SYMBOL
#define SINKER_SYMBOL(module_name, symbol_name, symbol_type)
#endif
)%";

        for (auto it = module_tags.begin(); it != module_tags.end(); ++it)
        {
            out << "#ifndef SINKER_TAG_" << *it << "_MODULE\n"
                << "#define SINKER_TAG_" << *it << "_MODULE(module_name)\n"
                << "#endif\n";
        }

        for (auto it = symbol_tags.begin(); it != symbol_tags.end(); ++it)
        {
            out << "#ifndef SINKER_TAG_" << *it << "_SYMBOL\n"
                << "#define SINKER_TAG_" << *it << "_SYMBOL(module_name, symbol_name, symbol_type)\n"
                << "#endif\n";
        }

        out << '\n';

        for (Module const *module : modules)
        {
            module->dump_def(out);
            out << "\n";
        }

        for (auto it = symbol_tags.rbegin(); it != symbol_tags.rend(); ++it)
        {
            out << "#undef SINKER_TAG_" << *it << "_SYMBOL\n";
        }

        for (auto it = module_tags.rbegin(); it != module_tags.rend(); ++it)
        {
            out << "#undef SINKER_TAG_" << *it << "_MODULE\n";
        }

        out <<
            R"%(#undef SINKER_MODULE
#undef SINKER_SYMBOL
)%";
    }

    Module *Context::get_module(std::string_view module_name)
    {
        for (Module *module : modules)
        {
            if (module_name == module->get_name())
            {
                return module;
            }
        }

        return nullptr;
    }

    Context::~Context()
    {
        for (Module *module : modules) {
            delete module;
        }
    }

    void Context::emplace_module(std::string_view name, std::optional<std::string> lpModuleName)
    {
        modules.push_back(new Module(name, lpModuleName, this));
    }

    void Symbol::dump(std::ostream &out) const
    {
        out << "symbol " << module->get_name() << "::" << name << ", \"" << type << "\";\n";

        for (auto const &attribute : get_attributes())
        {
            out << "set " << module->get_name() << "::" << name << ", " << attribute.first << ", " << attribute.second << ";\n";
        }

        for (auto it = tags.begin(); it != tags.end(); ++it)
        {
            out << "tag " << module->get_name() << "::" << name << ", " << *it << ";\n";
        }

        for (auto const& address : addresses)
        {
            out << "address " << module->get_name() << "::" << name << ", [";
            if (address.first.empty())
            {
                out << "*";
            } else {
                auto it = address.first.begin();
                out << *it;
                ++it;

                for (; it != address.first.end(); ++it)
                {
                    out << ", " << *it;
                }
            }
            out << "], " << *address.second << ";\n";
        }
    }

    Context *Module::get_context() const
    {
        return context;
    }

    void Symbol::add_tag(std::string const& tag)
    {
        tags.insert(tag);
        get_module()->get_context()->add_symbol_tag(tag);
    }
    
    void Module::add_tag(std::string const& tag)
    {
        tags.insert(tag);
        get_context()->add_module_tag(tag);
    }

    void Context::add_module_tag(std::string const& tag)
    {
        module_tags.insert(tag);
    }

    void Context::add_symbol_tag(std::string const& tag)
    {
        symbol_tags.insert(tag);
    }

    std::ostream &operator<<(std::ostream &os, Expression const &expression)
    {
        expression.dump(os);
        return os;
    }

    std::ostream &operator<<(std::ostream &os, Module const &module)
    {
        module.dump(os);
        return os;
    }

    std::ostream &operator<<(std::ostream &os, Symbol const &symbol)
    {
        symbol.dump(os);
        return os;
    }

    std::ostream &operator<<(std::ostream &os, Context const &context)
    {
        context.dump(os);
        return os;
    }

}
