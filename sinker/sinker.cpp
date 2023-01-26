#include <cstdlib>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <istream>

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
        symbols.push_back(std::move(Symbol(name, type, this)));
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

        for (auto variant : variants)
        {
            out << "variant " << name << ", " << variant.first << ", \"" << variant.second << "\";\n";
        }

        for (Symbol const &symbol : symbols)
        {
            symbol.dump(out);
        }
    }
    void Module::dump_def(std::ostream &out) const
    {
        out << "#ifndef SINKER_" << name << "_SYMBOLS\n";
        out << "#define SINKER_" << name << "_SYMBOLS(symbol_name, symbol_type)\n";
        out << "#endif\n";
        out << "SINKER_MODULE(" << name << ")\n";

        for (Symbol const &symbol : symbols)
        {
            symbol.dump_def(out);
        }

        out << "#undef SINKER_" << name << "_SYMBOLS\n";
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

    Module *Symbol::get_module() const
    {
        return module;
    }

    void Symbol::add_address(std::optional<std::string> const &variant, std::shared_ptr<Expression> expression)
    {
        variants.push_back({variant, std::move(expression)});
    }

    std::map<std::string, attribute_value_t, std::less<>> const &Attributable::get_attributes() const
    {
        return attributes;
    }

    void Symbol::dump_def(std::ostream &out) const
    {
        out << "SINKER_SYMBOL(" << module->get_name() << ", " << name << ", " << type << ")\n";
        out << "SINKER_" << module->get_name() << "_SYMBOL(" << name << ", " << type << ")\n";
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

        for (Module const *module : modules)
        {
            module->dump_def(out);
            out << "\n";
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
            out << "set " << name << ", " << attribute.first << ", " << attribute.second << ";\n";
        }

        for (auto const& address : variants)
        {
            out << "address " << module->get_name() << "::" << name;
            if (address.first)
            {
                out << ", " << address.first.value();
            }
            out << ", " << *address.second << ";\n";
        }
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
