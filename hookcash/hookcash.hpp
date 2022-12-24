#ifndef HOOKCASH_HPP
#define HOOKCASH_HPP

#include <iostream>
#include <string>
#include <unordered_map>

enum class Language
{
    HOOKCASH,
    SOURCE_CODE,
    BINARY,
};

struct Symbol
{
    bool required;
    std::string type;
    std::unordered_map<std::string, std::string> addresses;

    void dump(const std::string& module_name, const std::string& symbol_name)
    {
        std::cout << "symbol " << module_name << " " << symbol_name << " " << (required ? "required" : "optional") << " \"" << type << "\";\n";
        for (auto& [address_name, address] : addresses)
        {
            std::cout << "address " << module_name << " " << symbol_name << " " << address_name << " \"" << address << "\";\n";
        }
    }
};

enum class VariableType
{
    base_address,
};

struct Module
{
    std::string value = {};
    std::string base_address = {};
    std::unordered_map<std::string, std::string> variants = {};
    std::unordered_map<std::string, Symbol> symbols = {};

    void set(const VariableType variable_type, const std::string& variable_value)
    {
        switch (variable_type)
        {
            case VariableType::base_address:
                base_address = variable_value;
                break;
        }
    }

    const std::string& get(const VariableType variable_type)
    {
        switch (variable_type)
        {
            case VariableType::base_address:
                return base_address;
        }
    }

    void dump(const std::string& name)
    {
        std::cout << "module " << name << " \"" << value << "\";\n";

        if (!base_address.empty()) std::cout << "set " << name << " base_address \"" << base_address << "\";\n";

        for (auto& [variant_name, hash]: variants)
        {
            std::cout << "variant " << name << " " << variant_name << " \"" << hash << "\";\n";
        }

        for (auto& [symbol_name, symbol]: symbols)
        {
            symbol.dump(name, symbol_name);
        }
    }
};

struct Context
{
    std::unordered_map<std::string, Module> modules;

    void dump()
    {
        for (auto& [module_name, module]: modules)
        {
            module.dump(module_name);
        }
    }
};

#endif // !HOOKCASH_HPP
