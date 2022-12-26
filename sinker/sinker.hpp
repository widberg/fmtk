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
#include <ostream>
#include <iostream>
#include <iomanip>

enum class Language
{
    SINKER,
    SOURCE_CODE,
};

typedef unsigned long long expression_value_t;
typedef std::variant<expression_value_t, bool, std::string> attribute_value_t;
struct pattern_byte {
    std::uint8_t value;
    std::uint8_t mask;
};

class Context;

class Expression {
public:
    virtual std::optional<expression_value_t> calculate(Context *context) const = 0;
    virtual void dump(std::ostream& out) const = 0;
    virtual ~Expression() {}
};

std::ostream& operator<<(std::ostream& os, Expression const& expression)
{
    expression.dump(os);
    return os;
}


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

    std::map<std::string, attribute_value_t, std::less<>> const& get_attributes() const {
        return attributes;
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
    void dump(std::ostream& out) const;
private:
    std::vector<Module> modules;
};


std::ostream& operator<<(std::ostream& os, Context const& context)
{
    context.dump(os);
    return os;
}

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
    void dump(std::ostream& out) const;
    ~Symbol() {
        for (auto variant : variants) {
            delete variant.second;
        }
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


std::ostream& operator<<(std::ostream& os, Symbol const& symbol)
{
    symbol.dump(os);
    return os;
}

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
    std::optional<expression_value_t> get_relocated_base_address() const {
        return relocated_base_address;
    }
    void dump(std::ostream& out) const {
        out << "module " << name;
        if (lpModuleName) {
            out << ", \"" << lpModuleName.value() << "\"";
        }
        out << ", " << prefered_base_address << ";\n";

        for (auto variant : variants) {
            out << "variant " << name << ", " << variant.first << ", \"" << variant.second << "\";\n";
        }

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

        for (Symbol const& symbol : symbols) {
            symbol.dump(out);
        }
    }
private:
    Module(std::string_view name, std::optional<std::string> lpModuleName, expression_value_t prefered_base_address, Context *context)
        : name(name), lpModuleName(lpModuleName), prefered_base_address(prefered_base_address), context(context) {};
    Context *context;
    std::string name;
    std::optional<std::string> lpModuleName;
    expression_value_t prefered_base_address;
    std::optional<expression_value_t> relocated_base_address;
    std::vector<Symbol> symbols;
    std::map<std::string, std::string, std::less<>> variants;
    std::string real_variant;
};


std::ostream& operator<<(std::ostream& os, Module const& module)
{
    module.dump(os);
    return os;
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

void Context::dump(std::ostream& out) const {
    for (Module const& module : modules) {
        module.dump(out);
    }
}

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


class ParenthesesExpression final : Expression {
public:
    ParenthesesExpression(Expression *expression)
        : expression(expression) {}
    virtual std::optional<expression_value_t> calculate(Context *context) const override {
        return expression->calculate(context);
    }
    virtual void dump(std::ostream& out) const override {
        out << "(";
        expression->dump(out);
        out << ")";
    }
    virtual ~ParenthesesExpression() override {
        delete expression;
    }
private:
    Expression *expression;
};


class IntegerExpression final : Expression {
public:
    IntegerExpression(expression_value_t value)
        : value(value) {}
    virtual std::optional<expression_value_t> calculate(Context *context) const override {
        return value;
    }
    virtual void dump(std::ostream& out) const override {
        out << value;
    }
    virtual ~IntegerExpression() override {
    }
private:
    expression_value_t value;
};

class AdditionExpression final : Expression {
public:
    AdditionExpression(Expression *lhs, Expression *rhs)
        : lhs(lhs), rhs(rhs) {}
    virtual std::optional<expression_value_t> calculate(Context *context) const override {
        return lhs->calculate(context).value() + rhs->calculate(context).value();
    }
    virtual void dump(std::ostream& out) const override {
        out << *lhs << " + " << *rhs;
    }
    virtual ~AdditionExpression() override {
        delete lhs;
        delete rhs;
    }
private:
    Expression *lhs;
    Expression *rhs;
};

class SubtractionExpression final : Expression {
public:
    SubtractionExpression(Expression *lhs, Expression *rhs)
        : lhs(lhs), rhs(rhs) {}
    virtual std::optional<expression_value_t> calculate(Context *context) const override {
        return lhs->calculate(context).value() - rhs->calculate(context).value();
    }
    virtual void dump(std::ostream& out) const override {
        out << *lhs << " - " << *rhs;
    }
    virtual ~SubtractionExpression() override {
        delete lhs;
        delete rhs;
    }
private:
    Expression *lhs;
    Expression *rhs;
};

class MultiplicationExpression final : Expression {
public:
    MultiplicationExpression(Expression *lhs, Expression *rhs)
        : lhs(lhs), rhs(rhs) {}
    virtual std::optional<expression_value_t> calculate(Context *context) const override {
        return lhs->calculate(context).value() * rhs->calculate(context).value();
    }
    virtual void dump(std::ostream& out) const override {
        out << *lhs << " * " << *rhs;
    }
    virtual ~MultiplicationExpression() override {
        delete lhs;
        delete rhs;
    }
private:
    Expression *lhs;
    Expression *rhs;
};

class IndirectionExpression final : Expression {
public:
    IndirectionExpression(Expression *expression)
        : expression(expression) {}
    virtual std::optional<expression_value_t> calculate(Context *context) const override {
        return (expression_value_t)*(void**)(expression->calculate(context).value());
    }
    virtual void dump(std::ostream& out) const override {
        out << "*" << *expression;
    }
    virtual ~IndirectionExpression() override {
        delete expression;
    }
private:
    Expression *expression;
};

class RelocateExpression final : Expression {
public:
    RelocateExpression(Expression *expression)
        : expression(expression) {}
    virtual std::optional<expression_value_t> calculate(Context *context) const override {
        auto result = expression->calculate(context);
        if (!result) {
            return {};
        }
        return result.value();
    }
    virtual void dump(std::ostream& out) const override {
        out << "@" << *expression;
    }
    virtual ~RelocateExpression() override {
        delete expression;
    }
private:
    Expression *expression;
};

class NullCheckExpression final : Expression {
public:
    NullCheckExpression(Expression *expression)
        : expression(expression) {}
    virtual std::optional<expression_value_t> calculate(Context *context) const override {
        auto result = expression->calculate(context);
        if (!result || result.value() == 0) {
            return {};
        }
        return result;
    }
    virtual void dump(std::ostream& out) const override {
        out << "?" << *expression;
    }
    virtual ~NullCheckExpression() override {
        delete expression;
    }
private:
    Expression *expression;
};

class ArraySubscriptExpression final : Expression {
public:
    ArraySubscriptExpression(Expression *origin, Expression *offset)
        : origin(origin), offset(offset) {}
    virtual std::optional<expression_value_t> calculate(Context *context) const override {
        return (expression_value_t)*(void**)(origin->calculate(context).value() + offset->calculate(context).value() * sizeof(void*));
    }
    virtual void dump(std::ostream& out) const override {
        out << *origin << "[" << *offset << "]";
    }
    virtual ~ArraySubscriptExpression() override {
        delete origin;
        delete offset;
    }
private:
    Expression *origin;
    Expression *offset;
};

class GetProcAddressExpression final : Expression {
public:
    GetProcAddressExpression(std::string const& module, std::string const& lpProcName)
        : module(module), lpProcName(lpProcName) {}
    virtual std::optional<expression_value_t> calculate(Context *context) const override {
        return 0;
    }
    virtual void dump(std::ostream& out) const override {
        out << "!" << module << "::" << lpProcName;
    }
    virtual ~GetProcAddressExpression() override {
    }
private:
    std::string module;
    std::string lpProcName;
};

class ModuleExpression final : Expression {
public:
    ModuleExpression(std::string const& module)
        : module(module) {}
    virtual std::optional<expression_value_t> calculate(Context *context) const override {
        return context->get_module(module)->get_relocated_base_address();
    }
    virtual void dump(std::ostream& out) const override {
        out << module;
    }
    virtual ~ModuleExpression() override {
    }
private:
    std::string module;
};

class SymbolExpression final : Expression {
public:
    SymbolExpression(std::string const& module, std::string const& symbol)
        : module(module), symbol(symbol) {}
    virtual std::optional<expression_value_t> calculate(Context *context) const override {
        return context->get_module(module)->get_symbol(symbol)->get_cached_calculated_address<expression_value_t>();
    }
    virtual void dump(std::ostream& out) const override {
        out << module << "::" << symbol;
    }
    virtual ~SymbolExpression() override {
    }
private:
    std::string module;
    std::string symbol;
};

class PatternMatchExpression final : Expression {
public:
    PatternMatchExpression(std::vector<pattern_byte> const& pattern)
        : pattern(pattern) {}
    virtual std::optional<expression_value_t> calculate(Context *context) const override {
        return 0;
    }
    virtual void dump(std::ostream& out) const override {
        out << "{ ";
        std::ios_base::fmtflags f( out.flags() );
        for (pattern_byte pb : pattern) {
            if (pb.mask == 0xFF) {
                out << std::hex << std::setfill('0') << std::setw(2) << (int)pb.value;
            } else {
                out << "??";
            }
            out << " ";
        }
        out.flags( f );
        out << "}";
    }
    virtual ~PatternMatchExpression() override {
    }
private:
    std::vector<pattern_byte> pattern;
};

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
