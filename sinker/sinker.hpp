#ifndef SINKER_HPP
#define SINKER_HPP

#include <format>
#include <map>
#include <set>
#include <variant>
#include <string>
#include <string_view>
#include <cassert>
#include <optional>

enum class Language
{
    SINKER,
    SOURCE_CODE,
};

typedef unsigned long long expression_value_t;
typedef std::variant<expression_value_t, bool, std::string> attribute_value_t;

template <>
struct std::formatter<attribute_value_t> {
    std::formatter<expression_value_t> ull_formatter;
    std::formatter<bool> bool_formatter;
    std::formatter<std::string> string_formatter;

    constexpr auto parse(std::format_parse_context& ctx) {
        return string_formatter.parse(ctx);
    }

    auto format(const attribute_value_t& attr, std::format_context& ctx) {
        if (std::holds_alternative<expression_value_t>(attr)) {
            return ull_formatter.format(std::get<expression_value_t>(attr), ctx);
        } else if (std::holds_alternative<bool>(attr)) {
            return bool_formatter.format(std::get<bool>(attr), ctx);
        } 
        return string_formatter.format('"' + std::get<std::string>(attr) + '"', ctx);
    }
};

class Expression {
public:
    virtual std::optional<expression_value_t> calculate() const = 0;
};

class IntegerExpression final : Expression {
public:
    IntegerExpression(expression_value_t value)
        : value(value) {}
    virtual std::optional<expression_value_t> calculate() const override {
        return 0;
    }
private:
    expression_value_t value;
};

class AdditionExpression final : Expression {
public:
    AdditionExpression(Expression *lhs, Expression *rhs)
        : lhs(lhs), rhs(rhs) {}
    virtual std::optional<expression_value_t> calculate() const override {
        return lhs->calculate().value() + rhs->calculate().value();
    }
private:
    Expression *lhs;
    Expression *rhs;
};

class SubtractionExpression final : Expression {
public:
    SubtractionExpression(Expression *lhs, Expression *rhs)
        : lhs(lhs), rhs(rhs) {}
    virtual std::optional<expression_value_t> calculate() const override {
        return lhs->calculate().value() - rhs->calculate().value();
    }
private:
    Expression *lhs;
    Expression *rhs;
};

class MultiplicationExpression final : Expression {
public:
    MultiplicationExpression(Expression *lhs, Expression *rhs)
        : lhs(lhs), rhs(rhs) {}
    virtual std::optional<expression_value_t> calculate() const override {
        return lhs->calculate().value() * rhs->calculate().value();
    }
private:
    Expression *lhs;
    Expression *rhs;
};

class IndirectionExpression final : Expression {
public:
    IndirectionExpression(Expression *expression)
        : expression(expression) {}
    virtual std::optional<expression_value_t> calculate() const override {
        return (expression_value_t)*(void**)(expression->calculate().value());
    }
private:
    Expression *expression;
};

class RelocateExpression final : Expression {
public:
    RelocateExpression(Expression *expression)
        : expression(expression) {}
    virtual std::optional<expression_value_t> calculate() const override {
        return expression->calculate();
    }
private:
    Expression *expression;
};

class NullCheckExpression final : Expression {
public:
    NullCheckExpression(Expression *expression)
        : expression(expression) {}
    virtual std::optional<expression_value_t> calculate() const override {
        return expression->calculate();
    }
private:
    Expression *expression;
};

class ArraySubscriptExpression final : Expression {
public:
    ArraySubscriptExpression(Expression *origin, Expression *offset)
        : origin(origin), offset(offset) {}
    virtual std::optional<expression_value_t> calculate() const override {
        return (expression_value_t)*(void**)(origin->calculate().value() + offset->calculate().value() * sizeof(void*));
    }
private:
    Expression *origin;
    Expression *offset;
};

class GetProcAddressExpression final : Expression {
public:
    GetProcAddressExpression(std::string const& module, std::string const& lpProcName)
        : module(module), lpProcName(lpProcName) {}
    virtual std::optional<expression_value_t> calculate() const override {
        return 0;
    }
private:
    std::string module;
    std::string lpProcName;
};

class ModuleExpression final : Expression {
public:
    ModuleExpression(std::string const& module)
        : module(module) {}
    virtual std::optional<expression_value_t> calculate() const override {
        return 0;
    }
private:
    std::string module;
};

class SymbolExpression final : Expression {
public:
    SymbolExpression(std::string const& module, std::string const& symbol)
        : module(module), symbol(symbol) {}
    virtual std::optional<expression_value_t> calculate() const override {
        return 0;
    }
private:
    std::string module;
    std::string symbol;
};

class Attributable {
public:
    template<typename T>
    std::optional<T> get_attribute(std::string_view attribute_name) const {
        if (!attributes.count(attribute_name)) {
            return {};
        }

        attribute_value_t const& attr = attributes.at(attribute_name);

        if (T const* value = std::get_if<T>(&attr)) {
            return *value;
        }

        return {};
    }

    template<typename T>
    void set_attribute(std::string const& attribute_name, T value) {
        attributes[attribute_name] = value;
    }

private:
    std::map<std::string, attribute_value_t, std::less<>> attributes;
};

class Module;

class Context {
public:
    Context() {}
    Context (const Context&) = delete;
    Context& operator= (const Context&) = delete;
    std::vector<Module> const& get_modules() const {
        return modules;
    }
    Module* get_module(std::string_view module_name);

    void emplace_module(std::string_view name, std::optional<std::string> lpModuleName, expression_value_t prefered_base_address);
private:
    std::vector<Module> modules;
};

class Symbol : public Attributable {
    friend class Module;
public:
    Symbol (const Symbol&) = delete;
    Symbol& operator= (const Symbol&) = delete;
    Symbol(Symbol&&) = default;
    Symbol& operator=(Symbol&& mE) = default;
    std::string const& get_name() const {
        return name;
    }
    template<typename T>
    std::optional<T> calculate_address();

    template<typename T>
    std::optional<T> get_cached_calculated_address() const {
        if (!cached_calculated_address) {
            return {};
        }

        return static_cast<T>(cached_calculated_address.value());
    }

    Module *get_module() const {
        return module;
    }

    void add_address(std::optional<std::string> const& variant, Expression *expression) {
        variants.push_back({variant, expression});
    }
private:
    Symbol(std::string const& name, std::string const& type, Module *module)
        : name(name), type(type), module(module) {}
    std::optional<expression_value_t> cached_calculated_address;
    std::string name;
    std::string type;
    Module *module;
    std::vector<std::pair<std::optional<std::string>, Expression*>> variants;
};

class Module : public Attributable {
    friend class Context;
public:
    Module (const Module&) = delete;
    Module& operator= (const Module&) = delete;
    Module(Module&&) = default;
    Module& operator=(Module&& mE) = default;
    std::string const& get_name() const {
        return name;
    }
    std::string const& get_real_variant() const {
        return real_variant;
    }
    Symbol *get_symbol(std::string_view symbol_name) {
    for (Symbol& symbol : symbols) {
        if (symbol_name == symbol.get_name()) {
            return &symbol;
        }
    }

    return nullptr;
    }
    void emplace_symbol(std::string const& name, std::string const& type) {
        symbols.push_back(std::move(Symbol(name, type, this)));
    }
    void add_variant(std::string const& name, std::string const& hash) {
        variants.insert({name, hash});
    }
private:
    Module(std::string_view name, std::optional<std::string> lpModuleName, expression_value_t prefered_base_address, Context *context)
        : name(name), lpModuleName(lpModuleName), prefered_base_address(prefered_base_address), context(context) {};
    Context *context;
    std::string name;
    std::optional<std::string> lpModuleName;
    expression_value_t prefered_base_address;
    expression_value_t relocated_base_address;
    std::vector<Symbol> symbols;
    std::map<std::string, std::string, std::less<>> variants;
    std::string real_variant;
};

template<typename T>
std::optional<T> Symbol::calculate_address() {
    for (auto variant : variants) {
        if (!variant.first.has_value() || variant.first.value() == module->get_real_variant()) {
            cached_calculated_address = variant.second->calculate();
            if (cached_calculated_address) {
                return cached_calculated_address;
            }
        }
    }

    return {};
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




// class Installable {
// public:
//     virtual void install();
//     virtual void uninstall();
// };

// template<typename T>
// class Detour : Installable {
// public:
//     Detour(T& original, T detour);
// };

// template<typename T>
// class Patch : Installable {
// public:
//     Patch(T *dst, T *src);
// };

// class Transaction {
// public:
//     Transaction();
//     Transaction(Installable& installable);
//     void push_back(Installable& installable);
//     void commit();
//     void abort();
// };

// class StackCheck {
// public:
//     StackCheck();
//     bool good() const;
// };

// class Process {
// public:
//     Process(std::string_view path, std::vector<std::string_view> const& argv, std::vector<std::pair<std::string_view, std::string_view>> const& env);
//     void push_back_dll(std::string_view path);
//     void execute();
//     int wait();
// };

#endif // !SINKER_HPP
