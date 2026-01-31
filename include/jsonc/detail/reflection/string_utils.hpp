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

constexpr bool is_upper(char c) noexcept { return c >= 'A' && c <= 'Z'; }
constexpr bool is_lower(char c) noexcept { return c >= 'a' && c <= 'z'; }
constexpr char to_upper(char c) noexcept { return is_lower(c) ? c - ('a' - 'A') : c; }
constexpr char to_lower(char c) noexcept { return is_upper(c) ? c + ('a' - 'A') : c; }

constexpr std::string to_snake_case(std::string_view s, char split = '-') noexcept {
    std::string r{};
    for (size_t i = 0; i < s.size(); ++i) {
        uint8_t c        = s[i];
        bool    is_upper = std::isupper(c);
        if (c == '-' || c == ' ' || c == '_') { c = split; }
        if (is_upper && i > 0 && !r.empty() && r.back() != split) {
            uint8_t prev = s[i - 1];
            uint8_t next = (i + 1 < s.size()) ? s[i + 1] : 0;
            if (std::islower(prev) || (std::isupper(prev) && std::islower(next))) { r.push_back(split); }
        }
        r += is_upper ? static_cast<uint8_t>(std::tolower(c)) : c;
    }
    return r;
}

constexpr std::string to_pascal_case(std::string_view s) noexcept {
    std::string r;
    bool        new_word = true;
    for (uint8_t c : s) {
        if (c == '_' || c == '-' || c == ' ') {
            new_word = true;
        } else if (new_word) {
            r        += static_cast<uint8_t>(std::toupper(c));
            new_word  = false;
        } else {
            r += static_cast<uint8_t>(std::tolower(c));
        }
    }
    return r;
}

constexpr std::string to_camel_case(std::string_view s) noexcept {
    std::string r = to_pascal_case(s);
    if (!r.empty()) { r[0] = static_cast<uint8_t>(std::tolower(static_cast<uint8_t>(r[0]))); }
    return r;
}

constexpr std::string to_upper_case(std::string_view s) {
    std::string r;
    r.reserve(s.size());
    for (unsigned char c : s) { r += static_cast<char>(std::toupper(c)); }
    return r;
}

constexpr std::string to_lower_case(std::string_view s) {
    std::string r;
    r.reserve(s.size());
    for (unsigned char c : s) { r += static_cast<char>(std::tolower(c)); }
    return r;
}

} // namespace string_utils

namespace detail {

template <concepts::is_stringifiable_type T>
constexpr std::string type_to_string(const T& t) noexcept {
    if constexpr (traits::is_string_convertible_v<T>) {
        return t;
    } else if constexpr (traits::is_string_serializable_v<T>) {
        return serializer<T>::to_string(t);
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
        return serializer<RT>::from_string(sv);
    } else {
        if (auto val = magic_enum::enum_cast<RT>(sv)) { return val; }
        if (auto val = magic_enum::enum_flags_cast<RT>(sv)) { return val; }
        if (auto num = string_utils::str_to_num<std::underlying_type_t<RT>>(sv)) { return static_cast<RT>(*num); }
        return std::nullopt;
    }
}

} // namespace detail

namespace builtin_key_formatter {

constexpr auto default_key_formatter = [](std::string_view sv) noexcept -> std::string { return std::string(sv); };
constexpr auto snake_case_formatter  = [](std::string_view sv) noexcept -> std::string { return string_utils::to_snake_case(sv); };
constexpr auto pascal_case_formatter = [](std::string_view sv) noexcept -> std::string { return string_utils::to_pascal_case(sv); };
constexpr auto camel_case_formatter  = [](std::string_view sv) noexcept -> std::string { return string_utils::to_camel_case(sv); };
constexpr auto upper_case_formatter  = [](std::string_view sv) noexcept -> std::string { return string_utils::to_upper_case(sv); };
constexpr auto lower_case_formatter  = [](std::string_view sv) noexcept -> std::string { return string_utils::to_lower_case(sv); };

} // namespace builtin_key_formatter

} // namespace jsonc::reflection