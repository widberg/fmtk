#ifndef SINKER_HPP
#define SINKER_HPP

// #include <iostream>
// #include <string>
// #include <unordered_map>
#include <variant>
#include <string>
#include <string_view>
#include <cassert>

enum class Language
{
    SINKER,
    SOURCE_CODE,
};

typedef std::variant<unsigned long long, bool, std::string> attribute_value_t;

template <>
struct std::formatter<attribute_value_t> {
    std::formatter<unsigned long long> ull_formatter;
    std::formatter<bool> bool_formatter;
    std::formatter<std::string> string_formatter;

    constexpr auto parse(std::format_parse_context& ctx) {
        return string_formatter.parse(ctx);
    }

    auto format(const attribute_value_t& attr, std::format_context& ctx) {
        if (std::holds_alternative<unsigned long long>(attr)) {
            return ull_formatter.format(std::get<unsigned long long>(attr), ctx);
        } else if (std::holds_alternative<bool>(attr)) {
            return bool_formatter.format(std::get<bool>(attr), ctx);
        } 
        return string_formatter.format('"' + std::get<std::string>(attr) + '"', ctx);
    }
};

class Expression {
public:
    virtual unsigned long long calculate() const = 0;
};

class IntegerExpression final : Expression {
public:
    IntegerExpression(unsigned long long value)
        : value(value) {}
    virtual unsigned long long calculate() const override {
        return 0;
    }
private:
    unsigned long long value;
};

class AdditionExpression final : Expression {
public:
    AdditionExpression(Expression *lhs, Expression *rhs)
        : lhs(lhs), rhs(rhs) {}
    virtual unsigned long long calculate() const override {
        return lhs->calculate() + rhs->calculate();
    }
private:
    Expression *lhs;
    Expression *rhs;
};

class SubtractionExpression final : Expression {
public:
    SubtractionExpression(Expression *lhs, Expression *rhs)
        : lhs(lhs), rhs(rhs) {}
    virtual unsigned long long calculate() const override {
        return lhs->calculate() - rhs->calculate();
    }
private:
    Expression *lhs;
    Expression *rhs;
};

class MultiplicationExpression final : Expression {
public:
    MultiplicationExpression(Expression *lhs, Expression *rhs)
        : lhs(lhs), rhs(rhs) {}
    virtual unsigned long long calculate() const override {
        return lhs->calculate() * rhs->calculate();
    }
private:
    Expression *lhs;
    Expression *rhs;
};

// class Attributable {
// public:
//     template<typename T>
//     std::optional<T> get_attribute(std::string_view attribute_name) const;

//     template<typename T>
//     void set_attribute(std::string_view attribute_name, T value);

// private:
//     std::unordered_map<std::string, std::variant<unsigned long long, bool, std::string>> attributes;
// };

// class Module;

// class Symbol : Attributable {
// public:
//     std::string const& get_name() const;
//     template<typename T>
//     std::optional<T> calculate_address();
//     template<typename T>
//     std::optional<T> get_cached_calculated_address() const;
//     Module const& get_module() const;
// };

// class Module : Attributable {
// public:
//     std::string const& get_name() const;
//     std::vector<Symbol const> const& get_symbols() const;
//     std::optional<Symbol const&> get_symbol(std::string_view symbol_name) const;
// };

// class Context {
// public:
//     Context();
//     void interpret(std::istream& in);
//     void dump(std::ostream& out) const;
//     std::vector<Module const> const& get_modules() const;
//     std::optional<Module const&> get_module(std::string_view module_name) const;
// };

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