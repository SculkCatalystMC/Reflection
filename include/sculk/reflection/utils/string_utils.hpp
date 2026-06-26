// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/reflection/detail/concepts.hpp"
#include <charconv>
#include <limits>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>
#if defined(__APPLE__) && (!defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) || __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 260000)
#include <cerrno>
#include <cstdlib>
#endif

namespace sculk::reflection {

namespace string_utils {

template <typename T>
    requires(std::is_arithmetic_v<T> && !std::same_as<T, bool>)
constexpr std::optional<T> str_to_num(std::string_view sv) {
#if defined(__APPLE__) && (!defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) || __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 260000)
    if constexpr (std::is_floating_point_v<T>) {
        std::string input(sv);
        char*       end = nullptr;
        errno           = 0;

        T res{};
        if constexpr (std::same_as<T, float>) {
            res = std::strtof(input.c_str(), &end);
        } else if constexpr (std::same_as<T, double>) {
            res = std::strtod(input.c_str(), &end);
        } else {
            res = std::strtold(input.c_str(), &end);
        }

        if (errno == ERANGE || end != input.c_str() + input.size()) { return std::nullopt; }
        return res;
    } else {
        T res{};
        auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), res);
        if (ec != std::errc() || ptr != sv.data() + sv.size()) { return std::nullopt; }
        return res;
    }
#else
    T res{};
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), res);
    if (ec != std::errc() || ptr != sv.data() + sv.size()) { return std::nullopt; }
    return res;
#endif
}

template <reflection::concepts::is_enum T, bool _IntCast = false>
constexpr std::optional<T> str_to_enum(std::string_view sv, bool case_sensitive = false) {
    using RT = std::remove_cvref_t<T>;
    if (auto val = case_sensitive ? magic_enum::enum_cast<RT>(sv) : magic_enum::enum_cast<RT>(sv, magic_enum::case_insensitive)) { return val; }
    if (auto val = case_sensitive ? magic_enum::enum_flags_cast<RT>(sv) : magic_enum::enum_flags_cast<RT>(sv, magic_enum::case_insensitive)) {
        return val;
    }
    if constexpr (_IntCast) {
        if (auto num = str_to_num<std::underlying_type_t<T>>(sv)) { return static_cast<RT>(*num); }
    }
    return std::nullopt;
}

template <reflection::concepts::is_enum T, bool _IntCast = false>
constexpr std::optional<std::string> enum_to_str(T val) {
    if (auto name = magic_enum::enum_name(val); !name.empty()) { return std::string(name); }
    if (auto flag = magic_enum::enum_flags_name(val); !flag.empty()) { return std::string(flag); }
    if constexpr (_IntCast) {
        return std::to_string(std::to_underlying(val));
    } else {
        return {};
    }
}

constexpr bool is_upper(char c) { return c >= 'A' && c <= 'Z'; }
constexpr bool is_lower(char c) { return c >= 'a' && c <= 'z'; }
constexpr char to_upper(char c) { return is_lower(c) ? c - ('a' - 'A') : c; }
constexpr char to_lower(char c) { return is_upper(c) ? c + ('a' - 'A') : c; }

constexpr std::string to_snake_case(std::string_view s, char split = '-') {
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

constexpr std::string to_pascal_case(std::string_view s) {
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

constexpr std::string to_camel_case(std::string_view s) {
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

constexpr auto default_key_formatter = [](std::string_view sv) -> std::string_view { return sv; };
constexpr auto snake_case_formatter  = [](std::string_view sv) -> std::string { return string_utils::to_snake_case(sv); };
constexpr auto pascal_case_formatter = [](std::string_view sv) -> std::string { return string_utils::to_pascal_case(sv); };
constexpr auto camel_case_formatter  = [](std::string_view sv) -> std::string { return string_utils::to_camel_case(sv); };
constexpr auto upper_case_formatter = [](std::string_view sv) -> std::string { return string_utils::to_upper_case(string_utils::to_snake_case(sv)); };

} // namespace builtin_key_formatter

} // namespace sculk::reflection
