#ifndef SINKER_HPP
#define SINKER_HPP

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
#include <memory>
#include <Windows.h>

namespace sinker
{

    enum class Language
    {
        SINKER,
        SOURCE_CODE,
    };

    typedef unsigned long long expression_value_t;
    typedef std::variant<expression_value_t, bool, std::string> attribute_value_t;

    std::ostream &operator<<(std::ostream &out, attribute_value_t const &attribute_value);

    struct pattern_byte
    {
        std::uint8_t value;
        std::uint8_t mask;
    };

    class Context;
    class Symbol;

    class Expression
    {
    public:
        virtual std::optional<expression_value_t> calculate(Symbol *symbol) const = 0;
        virtual void dump(std::ostream &out) const = 0;
        virtual ~Expression() {}
    };

    std::ostream &operator<<(std::ostream &os, Expression const &expression);

    class Attributable
    {
    public:
        template <typename T>
        std::optional<T> get_attribute(std::string_view attribute_name) const;

        template <typename T>
        void set_attribute(std::string const &attribute_name, T value);

        std::map<std::string, attribute_value_t, std::less<>> const &get_attributes() const;

    private:
        std::map<std::string, attribute_value_t, std::less<>> attributes;
    };

    class Module;

    class Context
    {
    public:
        Context() {}
        Context(const Context &) = delete;
        Context &operator=(const Context &) = delete;
        std::vector<Module> const &get_modules() const
        {
            return modules;
        }
        Module *get_module(std::string_view module_name);

        void emplace_module(std::string_view name, std::optional<std::string> lpModuleName, expression_value_t prefered_base_address);
        void dump(std::ostream &out) const;
        void dump_def(std::ostream &out) const;
        bool interpret(std::istream &input_stream, Language language, std::string input_filename, bool debug = false);
        bool interpret(char *input, unsigned int size, Language language, std::string input_filename, bool debug = false);
    private:
        std::vector<Module> modules;
    };

    std::ostream &operator<<(std::ostream &os, Context const &context);

    class Symbol : public Attributable
    {
        friend class Module;

    public:
        Symbol(const Symbol &) = delete;
        Symbol &operator=(const Symbol &) = delete;
        Symbol(Symbol &&) = default;
        Symbol &operator=(Symbol &&mE) = default;
        std::string const &get_name() const
        {
            return name;
        }
        template <typename T>
        std::optional<T> calculate_address();

        template <typename T>
        std::optional<T> get_cached_calculated_address() const;

        Module *get_module() const;

        void add_address(std::optional<std::string> const &variant, std::shared_ptr<Expression> expression);
        void dump(std::ostream &out) const;

        void dump_def(std::ostream &out) const;

    private:
        Symbol(std::string const &name, std::string const &type, Module *module)
            : name(name), type(type), module(module) {}
        std::optional<expression_value_t> cached_calculated_address;
        std::string name;
        std::string type;
        Module *module;
        std::vector<std::pair<std::optional<std::string>, std::shared_ptr<Expression>>> variants;
    };

    std::ostream &operator<<(std::ostream &os, Symbol const &symbol);

    class Module : public Attributable
    {
        friend class Context;

    public:
        Module(const Module &) = delete;
        Module &operator=(const Module &) = delete;
        Module(Module &&) = default;
        Module &operator=(Module &&mE) = default;
        std::string const &get_name() const;
        std::string const &get_real_variant() const;
        Symbol *get_symbol(std::string_view symbol_name);

        void emplace_symbol(std::string const &name, std::string const &type);
        void add_variant(std::string const &name, std::string const &hash);
        bool has_variant(std::string_view name) const;
        void dump(std::ostream &out) const;
        void dump_def(std::ostream &out) const;
        expression_value_t get_preferred_base_address() const;
        std::optional<expression_value_t> get_relocated_base_address() const;
        HMODULE get_hModule() const;

    private:
        Module(std::string_view name, std::optional<std::string> lpModuleName, expression_value_t preferred_base_address, Context *context)
            : name(name), lpModuleName(lpModuleName), preferred_base_address(preferred_base_address), context(context){};
        Context *context;
        std::string name;
        std::optional<std::string> lpModuleName;
        expression_value_t preferred_base_address;
        std::optional<expression_value_t> relocated_base_address;
        std::vector<Symbol> symbols;
        std::map<std::string, std::string, std::less<>> variants;
        std::string real_variant;
        HMODULE hModule = 0;
    };

    std::ostream &operator<<(std::ostream &os, Module const &module);

#define PROPAGATE_UNRESOLVED(x) \
    do                          \
    {                           \
        if (!x)                 \
            return {};          \
    } while (0)

    class ParenthesesExpression final : Expression
    {
    public:
        ParenthesesExpression(std::shared_ptr<Expression> expression)
            : expression(expression) {}
        virtual std::optional<expression_value_t> calculate(Symbol *symbol) const override
        {
            return expression->calculate(symbol);
        }
        virtual void dump(std::ostream &out) const override
        {
            out << "(";
            expression->dump(out);
            out << ")";
        }

    private:
        std::shared_ptr<Expression> expression;
    };

    class IntegerExpression final : Expression
    {
    public:
        IntegerExpression(expression_value_t value)
            : value(value) {}
        virtual std::optional<expression_value_t> calculate(Symbol *symbol) const override
        {
            return value;
        }
        virtual void dump(std::ostream &out) const override
        {
            out << value;
        }

    private:
        expression_value_t value;
    };

    class AdditionExpression final : Expression
    {
    public:
        AdditionExpression(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
            : lhs(lhs), rhs(rhs) {}
        virtual std::optional<expression_value_t> calculate(Symbol *symbol) const override
        {
            auto lhs_result = lhs->calculate(symbol);
            auto rhs_result = rhs->calculate(symbol);
            PROPAGATE_UNRESOLVED(lhs_result);
            PROPAGATE_UNRESOLVED(rhs_result);
            return lhs_result.value() + rhs_result.value();
        }
        virtual void dump(std::ostream &out) const override
        {
            out << *lhs << " + " << *rhs;
        }

    private:
        std::shared_ptr<Expression> lhs;
        std::shared_ptr<Expression> rhs;
    };

    class SubtractionExpression final : Expression
    {
    public:
        SubtractionExpression(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
            : lhs(lhs), rhs(rhs) {}
        virtual std::optional<expression_value_t> calculate(Symbol *symbol) const override
        {
            auto lhs_result = lhs->calculate(symbol);
            auto rhs_result = rhs->calculate(symbol);
            PROPAGATE_UNRESOLVED(lhs_result);
            PROPAGATE_UNRESOLVED(rhs_result);
            return lhs_result.value() - rhs_result.value();
        }
        virtual void dump(std::ostream &out) const override
        {
            out << *lhs << " - " << *rhs;
        }

    private:
        std::shared_ptr<Expression> lhs;
        std::shared_ptr<Expression> rhs;
    };

    class MultiplicationExpression final : Expression
    {
    public:
        MultiplicationExpression(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
            : lhs(lhs), rhs(rhs) {}
        virtual std::optional<expression_value_t> calculate(Symbol *symbol) const override
        {
            auto lhs_result = lhs->calculate(symbol);
            auto rhs_result = rhs->calculate(symbol);
            PROPAGATE_UNRESOLVED(lhs_result);
            PROPAGATE_UNRESOLVED(rhs_result);
            return lhs_result.value() * rhs_result.value();
        }
        virtual void dump(std::ostream &out) const override
        {
            out << *lhs << " * " << *rhs;
        }

    private:
        std::shared_ptr<Expression> lhs;
        std::shared_ptr<Expression> rhs;
    };

    class IndirectionExpression final : Expression
    {
    public:
        IndirectionExpression(std::shared_ptr<Expression> expression)
            : expression(expression) {}
        virtual std::optional<expression_value_t> calculate(Symbol *symbol) const override
        {
            auto expression_result = expression->calculate(symbol);
            PROPAGATE_UNRESOLVED(expression_result);
            return (expression_value_t) * (void **)(expression_result.value());
        }
        virtual void dump(std::ostream &out) const override
        {
            out << "*" << *expression;
        }

    private:
        std::shared_ptr<Expression> expression;
    };

    class RelocateExpression final : Expression
    {
    public:
        RelocateExpression(std::shared_ptr<Expression> expression)
            : expression(expression) {}
        virtual std::optional<expression_value_t> calculate(Symbol *symbol) const override
        {
            auto expression_result = expression->calculate(symbol);
            auto module_result = symbol->get_module()->get_relocated_base_address();
            PROPAGATE_UNRESOLVED(expression_result);
            PROPAGATE_UNRESOLVED(module_result);
            return expression_result.value() - symbol->get_module()->get_preferred_base_address() + module_result.value();
        }
        virtual void dump(std::ostream &out) const override
        {
            out << "@" << *expression;
        }

    private:
        std::shared_ptr<Expression> expression;
    };

    class NullCheckExpression final : Expression
    {
    public:
        NullCheckExpression(std::shared_ptr<Expression> expression)
            : expression(expression) {}
        virtual std::optional<expression_value_t> calculate(Symbol *symbol) const override
        {
            auto expression_result = expression->calculate(symbol);
            PROPAGATE_UNRESOLVED(expression_result);
            expression_value_t expression_value = expression_result.value();
            if (expression_value)
            {
                return expression_value;
            }
            return {};
        }
        virtual void dump(std::ostream &out) const override
        {
            out << "?" << *expression;
        }

    private:
        std::shared_ptr<Expression> expression;
    };

    class ArraySubscriptExpression final : Expression
    {
    public:
        ArraySubscriptExpression(std::shared_ptr<Expression> origin, std::shared_ptr<Expression> offset)
            : origin(origin), offset(offset) {}
        virtual std::optional<expression_value_t> calculate(Symbol *symbol) const override
        {
            auto origin_result = origin->calculate(symbol);
            auto offset_result = offset->calculate(symbol);
            PROPAGATE_UNRESOLVED(origin_result);
            PROPAGATE_UNRESOLVED(offset_result);
            return (expression_value_t) * (void **)(origin_result.value() + offset_result.value() * sizeof(void *));
        }
        virtual void dump(std::ostream &out) const override
        {
            out << *origin << "[" << *offset << "]";
        }

    private:
        std::shared_ptr<Expression> origin;
        std::shared_ptr<Expression> offset;
    };

    class GetProcAddressExpression final : Expression
    {
    public:
        GetProcAddressExpression(Module *module, std::string const &lpProcName)
            : module(module), lpProcName(lpProcName) {}
        virtual std::optional<expression_value_t> calculate(Symbol *symbol) const override
        {
            HMODULE hModule = module->get_hModule();
            if (!hModule)
            {
                return {};
            }

            FARPROC addr = GetProcAddress(hModule, lpProcName.c_str());

            if (addr)
            {
                return (expression_value_t)addr;
            }

            return {};
        }
        virtual void dump(std::ostream &out) const override
        {
            out << "!" << module->get_name() << "::" << lpProcName;
        }

    private:
        Module *module;
        std::string lpProcName;
    };

    class ModuleExpression final : Expression
    {
    public:
        ModuleExpression(Module *module)
            : module(module) {}
        virtual std::optional<expression_value_t> calculate(Symbol *symbol) const override
        {
            return module->get_relocated_base_address();
        }
        virtual void dump(std::ostream &out) const override
        {
            out << module->get_name();
        }

    private:
        Module *module;
    };

    class SymbolExpression final : Expression
    {
    public:
        SymbolExpression(Symbol *symbol)
            : symbol(symbol) {}
        virtual std::optional<expression_value_t> calculate(Symbol *_symbol) const override
        {
            return symbol->get_cached_calculated_address<expression_value_t>();
        }
        virtual void dump(std::ostream &out) const override
        {
            out << symbol->get_module()->get_name() << "::" << symbol->get_name();
        }

    private:
        Symbol *symbol;
    };

    class PatternMatchExpression final : Expression
    {
    public:
        PatternMatchExpression(std::vector<pattern_byte> const &pattern)
            : pattern(pattern) {}
        virtual std::optional<expression_value_t> calculate(Symbol *symbol) const override
        {
            return {};
        }
        virtual void dump(std::ostream &out) const override
        {
            out << "{ ";
            std::ios_base::fmtflags f(out.flags());
            for (pattern_byte pb : pattern)
            {
                if (pb.mask == 0xFF)
                {
                    out << std::hex << std::setfill('0') << std::setw(2) << (int)pb.value;
                }
                else
                {
                    out << "??";
                }
                out << " ";
            }
            out.flags(f);
            out << "}";
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

}

#include "sinker.tpp"

#endif // !SINKER_HPP
