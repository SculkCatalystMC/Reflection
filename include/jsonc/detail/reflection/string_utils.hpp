#pragma once
#include "jsonc/detail/reflection/concepts.hpp"
#include "jsonc/detail/reflection/serializer.hpp"
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

namespace jsonc::reflection {

namespace string_utils {

template <typename T>
    requires(std::is_arithmetic_v<T> && !std::same_as<T, bool>)
constexpr std::optional<T> str_to_num(std::string_view sv) noexcept;

} // namespace string_utils

namespace detail {

template <concepts::is_stringifiable_type T>
constexpr std::string type_to_string(const T& t) {
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

template <concepts::is_stringifiable_type T>
constexpr std::optional<T> string_to_type(std::string_view sv) noexcept {
    using RT = std::remove_cvref_t<T>;
    if constexpr (traits::is_string_convertible_v<T>) {
        return std::string(sv);
    } else if constexpr (traits::is_string_serializable_v<T>) {
        return Serializer<RT>::from_string(sv);
    } else {
        if (auto val = magic_enum::enum_cast<RT>(sv)) { return val; }
        if (auto val = magic_enum::enum_flags_cast<RT>(sv)) { return val; }
        if (auto num = string_utils::str_to_num<std::underlying_type_t<RT>>(sv)) { return static_cast<RT>(*num); }
        return std::nullopt;
    }
}

} // namespace detail

using KeyFormatter = const std::function<std::string(std::string_view)>&;

} // namespace jsonc::reflection