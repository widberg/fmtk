#include <cstdlib>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <format>
#include <CLI/CLI.hpp>
#include <istream>

#include "sinker.hpp"

void Symbol::dump_def(std::ostream& out) const {
    out << "SINKER_SYMBOL(" << module->get_name() << ", " << name << ", " << type << ")\n";
    out << "SINKER_" << module->get_name() << "_SYMBOL(" << name << ", " << type << ")\n";
}

void Context::dump(std::ostream& out) const {
    for (Module const& module : modules) {
        module.dump(out);
    }
}

void Context::dump_def(std::ostream& out) const {
    out <<
R"%(#ifndef SINKER_MODULE
#define SINKER_MODULE(module_name)
#endif
#ifndef SINKER_SYMBOL
#define SINKER_SYMBOL(module_name, symbol_name, symbol_type)
#endif

)%";

    for (Module const& module : modules) {
        module.dump_def(out);
        out << "\n";
    }

    out <<
R"%(#undef SINKER_MODULE
#undef SINKER_SYMBOL
)%";
}


Module* Context::get_module(std::string_view module_name) {
    for (Module& module : modules) {
        if (module_name == module.get_name()) {
            return &module;
        }
    }

    return nullptr;
}

void Context::emplace_module(std::string_view name, std::optional<std::string> lpModuleName, expression_value_t prefered_base_address) {
    modules.push_back(std::move(Module(name, lpModuleName, prefered_base_address, this)));
}

void Symbol::dump(std::ostream& out) const {
    out << "symbol " << module->get_name() << "::" << name << ", \"" << type << "\";\n";

    for (auto const& attribute : get_attributes()) {
        out << "set " << name << ", " << attribute.first << ", ";
        if (std::holds_alternative<expression_value_t>(attribute.second)) {
            out << std::get<expression_value_t>(attribute.second);
        } else if (std::holds_alternative<bool>(attribute.second)) {
                out << (std::get<bool>(attribute.second) ? "true" : "false");
        } else {
            out << "\"" << std::get<std::string>(attribute.second) << "\"";
        }
        out << ";\n";
    }

    for (auto address : variants) {
        out << "address " << module->get_name() << "::" << name;
        if (address.first) {
            out << ", " << address.first.value();
        }
        out << ", " << *address.second << ";\n";
    }
}

std::ostream& operator<<(std::ostream& os, Expression const& expression)
{
    expression.dump(os);
    return os;
}

std::ostream& operator<<(std::ostream& os, Module const& module)
{
    module.dump(os);
    return os;
}


std::ostream& operator<<(std::ostream& os, Symbol const& symbol)
{
    symbol.dump(os);
    return os;
}

std::ostream& operator<<(std::ostream& os, Context const& context)
{
    context.dump(os);
    return os;
}
