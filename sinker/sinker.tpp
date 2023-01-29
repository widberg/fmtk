#ifndef SINKER_TPP
#define SINKER_TPP

#include <cstring>
#include <stdexcept>
namespace sinker
{
    template <typename T>
    std::optional<T> Symbol::calculate_address()
    {
        for (auto address : addresses)
        {
            if (address.first.empty() || address.first.find(module->get_real_variant()) != address.first.end())
            {
                cached_calculated_address = address.second->calculate(this);
                if (cached_calculated_address)
                {
                    return cached_calculated_address;
                }
            }
        }

        return {};
    }

    template <typename T>
    std::optional<T> Attributable::get_attribute(std::string_view attribute_name) const
    {
        if (!attributes.count(attribute_name))
        {
            return {};
        }

        attribute_value_t const &attr = attributes.at(attribute_name);

        if (T const *value = std::get_if<T>(&attr))
        {
            return *value;
        }

        return {};
    }

    template <typename T>
    void Attributable::set_attribute(std::string const &attribute_name, T value)
    {
        attributes[attribute_name] = value;
    }

    template <typename T>
    std::optional<T> Symbol::get_cached_calculated_address() const
    {
        if (!cached_calculated_address)
        {
            return {};
        }

        return static_cast<T>(cached_calculated_address.value());
    }

    template<std::size_t S, std::uint8_t C>
    StackCheck<S, C>::StackCheck(bool destructor_check)
    {
        std::memset(buffer, C, S);
    }

    template<std::size_t S, std::uint8_t C>
    bool StackCheck<S, C>::good() const
    {
        for (std::size_t i = 0; i < S; ++i)
        {
            if (buffer[i] != C)
            {
                return false;
            }
        }
        return true;
    }
    
    template<std::size_t S, std::uint8_t C>
    bool StackCheck<S, C>::~StackCheck()
    {
        if (!good())
        {
            throw std::runtime_error("Stack check failed");
        }
    }
}

#endif // !SINKER_TPP
