#pragma once
#include "jsonc/detail/reflection/jsonc_header.hpp"
#include <charconv>
#include <format>
#include <optional>
#include <string>

namespace sculk::jsonc::reflection {

template <typename T>
struct serializer {};

namespace string_utils {
template <typename T>
    requires(std::is_arithmetic_v<T> && !std::same_as<T, bool>)
constexpr std::optional<T> str_to_num(std::string_view sv) noexcept {
    T res{};
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), res);
    if (ec != std::errc() || ptr != sv.data() + sv.size()) { return std::nullopt; }
    return res;
}
} // namespace string_utils

template <>
struct serializer<bool> {
    static std::string         to_string(bool t) noexcept { return t ? "true" : "false"; }
    static std::optional<bool> from_string(std::string_view s) noexcept {
        if (s == "true") { return true; }
        if (s == "false") { return false; }
        return std::nullopt;
    }
};

template <>
struct serializer<std::int8_t> {
    static std::string                to_string(std::int8_t t) noexcept { return std::format("{}", t); }
    static std::optional<std::int8_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<std::int8_t>(s); }
};

template <>
struct serializer<std::uint8_t> {
    static std::string                 to_string(std::uint8_t t) noexcept { return std::format("{}", t); }
    static std::optional<std::uint8_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<std::uint8_t>(s); }
};

template <>
struct serializer<std::int16_t> {
    static std::string                 to_string(std::int16_t t) noexcept { return std::format("{}", t); }
    static std::optional<std::int16_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<std::int16_t>(s); }
};

template <>
struct serializer<std::uint16_t> {
    static std::string                  to_string(std::uint16_t t) noexcept { return std::format("{}", t); }
    static std::optional<std::uint16_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<std::uint16_t>(s); }
};

template <>
struct serializer<std::int32_t> {
    static std::string                 to_string(std::int32_t t) noexcept { return std::format("{}", t); }
    static std::optional<std::int32_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<std::int32_t>(s); }
};

template <>
struct serializer<std::uint32_t> {
    static std::string                  to_string(std::uint32_t t) noexcept { return std::format("{}", t); }
    static std::optional<std::uint32_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<std::uint32_t>(s); }
};

template <>
struct serializer<std::int64_t> {
    static std::string                 to_string(std::int64_t t) noexcept { return std::format("{}", t); }
    static std::optional<std::int64_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<std::int64_t>(s); }
};

template <>
struct serializer<std::uint64_t> {
    static std::string                  to_string(std::uint64_t t) noexcept { return std::format("{}", t); }
    static std::optional<std::uint64_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<std::uint64_t>(s); }
};

template <>
struct serializer<float> {
    static std::string          to_string(float t) noexcept { return std::format("{}", t); }
    static std::optional<float> from_string(std::string_view s) noexcept { return string_utils::str_to_num<float>(s); }
};

template <>
struct serializer<double> {
    static std::string           to_string(double t) noexcept { return std::format("{}", t); }
    static std::optional<double> from_string(std::string_view s) noexcept { return string_utils::str_to_num<double>(s); }
};

} // namespace sculk::jsonc::reflection
