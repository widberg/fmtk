#ifndef SINKER_TPP
#define SINKER_TPP

namespace sinker {

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

template<typename T>
std::optional<T> Attributable::get_attribute(std::string_view attribute_name) const {
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
void Attributable::set_attribute(std::string const& attribute_name, T value) {
    attributes[attribute_name] = value;
}


    template<typename T>
    std::optional<T> Symbol::get_cached_calculated_address() const {
        if (!cached_calculated_address) {
            return {};
        }

        return static_cast<T>(cached_calculated_address.value());
    }

}

#endif // !SINKER_TPP
