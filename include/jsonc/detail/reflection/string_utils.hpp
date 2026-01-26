#pragma once
#include "jsonc/detail/reflection/concepts.hpp"
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

namespace jsonc::reflection::detail::string_utils {

template <concepts::is_string_type T>
inline std::string type_to_string(const T& t) {
    if constexpr (traits::is_string_convertible_v<T>) {
        return t;
    } else if constexpr (traits::is_string_serializable_v<T>) {
        return Serializer<T>::to_string(t);
    } else {
        if (auto name = magic_enum::enum_name(t); !name.empty()) { return std::string(name); }
        if (auto flag = magic_enum::enum_flags_name(t); !flag.empty()) { return std::string(flag); }
        return std::to_string(std::to_underlying(t));
    }
}

} // namespace jsonc::reflection::detail::string_utils