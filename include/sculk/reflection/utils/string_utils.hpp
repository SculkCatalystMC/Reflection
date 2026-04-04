#pragma once
#include "sculk/reflection/detail/concepts.hpp"
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

namespace sculk::reflection {

namespace string_utils {

template <typename T>
    requires(std::is_arithmetic_v<T> && !std::same_as<T, bool>)
constexpr std::optional<T> str_to_num(std::string_view sv) noexcept;

template <reflection::concepts::is_enum T, bool _IntCast = false>
constexpr std::optional<T> str_to_enum(std::string_view sv) noexcept {
    using RT = std::remove_cvref_t<T>;
    if (auto val = magic_enum::enum_cast<RT>(sv)) { return val; }
    if (auto val = magic_enum::enum_flags_cast<RT>(sv)) { return val; }
    if constexpr (_IntCast) {
        if (auto num = str_to_num<std::underlying_type_t<T>>(sv)) { return static_cast<RT>(*num); }
    }
    return std::nullopt;
}

template <reflection::concepts::is_enum T, bool _IntCast = false>
constexpr std::optional<std::string> enum_to_str(T val) noexcept {
    if (auto name = magic_enum::enum_name(val); !name.empty()) { return std::string(name); }
    if (auto flag = magic_enum::enum_flags_name(val); !flag.empty()) { return std::string(flag); }
    if constexpr (_IntCast) {
        return std::to_string(std::to_underlying(val));
    } else {
        return {};
    }
}

constexpr bool is_upper(char c) noexcept { return c >= 'A' && c <= 'Z'; }
constexpr bool is_lower(char c) noexcept { return c >= 'a' && c <= 'z'; }
constexpr char to_upper(char c) noexcept { return is_lower(c) ? c - ('a' - 'A') : c; }
constexpr char to_lower(char c) noexcept { return is_upper(c) ? c + ('a' - 'A') : c; }

constexpr std::string to_snake_case(std::string_view s, char split = '-') noexcept {
    std::string r{};
    for (size_t i = 0; i < s.size(); ++i) {
        std::uint8_t c        = s[i];
        bool         is_upper = std::isupper(c);
        if (c == '-' || c == ' ' || c == '_') { c = split; }
        if (is_upper && i > 0 && !r.empty() && r.back() != split) {
            std::uint8_t prev = s[i - 1];
            std::uint8_t next = (i + 1 < s.size()) ? s[i + 1] : 0;
            if (std::islower(prev) || (std::isupper(prev) && std::islower(next))) { r.push_back(split); }
        }
        r += is_upper ? static_cast<std::uint8_t>(std::tolower(c)) : c;
    }
    return r;
}

constexpr std::string to_pascal_case(std::string_view s) noexcept {
    std::string r;
    bool        new_word = true;
    for (std::uint8_t c : s) {
        if (c == '_' || c == '-' || c == ' ') {
            new_word = true;
        } else if (new_word) {
            r        += static_cast<std::uint8_t>(std::toupper(c));
            new_word  = false;
        } else {
            r += static_cast<std::uint8_t>(std::tolower(c));
        }
    }
    return r;
}

constexpr std::string to_camel_case(std::string_view s) noexcept {
    std::string r = to_pascal_case(s);
    if (!r.empty()) { r[0] = static_cast<std::uint8_t>(std::tolower(static_cast<std::uint8_t>(r[0]))); }
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

namespace builtin_key_formatter {

constexpr auto default_key_formatter = [](std::string_view sv) noexcept -> std::string { return std::string(sv); };
constexpr auto snake_case_formatter  = [](std::string_view sv) noexcept -> std::string { return string_utils::to_snake_case(sv); };
constexpr auto pascal_case_formatter = [](std::string_view sv) noexcept -> std::string { return string_utils::to_pascal_case(sv); };
constexpr auto camel_case_formatter  = [](std::string_view sv) noexcept -> std::string { return string_utils::to_camel_case(sv); };
constexpr auto upper_case_formatter  = [](std::string_view sv) noexcept -> std::string {
    return string_utils::to_upper_case(string_utils::to_snake_case(sv));
};

} // namespace builtin_key_formatter

} // namespace sculk::reflection
