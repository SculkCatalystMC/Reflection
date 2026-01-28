#pragma once
#include "jsonc/detail/reflection/jsonc_header.hpp"
#include <charconv>
#include <format>
#include <optional>
#include <string>

namespace jsonc::reflection {

template <typename T>
struct Serializer {};

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
struct Serializer<bool> {
    static std::string         to_string(bool t) noexcept { return t ? "true" : "false"; }
    static std::optional<bool> from_string(std::string_view s) noexcept {
        if (s == "true") { return true; }
        if (s == "false") { return false; }
        return std::nullopt;
    }
};

template <>
struct Serializer<int8_t> {
    static std::string           to_string(int8_t t) noexcept { return std::format("{}", t); }
    static std::optional<int8_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<int8_t>(s); }
};

template <>
struct Serializer<uint8_t> {
    static std::string            to_string(uint8_t t) noexcept { return std::format("{}", t); }
    static std::optional<uint8_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<uint8_t>(s); }
};

template <>
struct Serializer<short> {
    static std::string          to_string(short t) noexcept { return std::format("{}", t); }
    static std::optional<short> from_string(std::string_view s) noexcept { return string_utils::str_to_num<short>(s); }
};

template <>
struct Serializer<uint16_t> {
    static std::string             to_string(uint16_t t) noexcept { return std::format("{}", t); }
    static std::optional<uint16_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<uint16_t>(s); }
};

template <>
struct Serializer<int> {
    static std::string        to_string(int t) noexcept { return std::format("{}", t); }
    static std::optional<int> from_string(std::string_view s) noexcept { return string_utils::str_to_num<int>(s); }
};

template <>
struct Serializer<uint32_t> {
    static std::string             to_string(uint32_t t) noexcept { return std::format("{}", t); }
    static std::optional<uint32_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<uint32_t>(s); }
};

template <>
struct Serializer<int64_t> {
    static std::string            to_string(int64_t t) noexcept { return std::format("{}", t); }
    static std::optional<int64_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<int64_t>(s); }
};

template <>
struct Serializer<uint64_t> {
    static std::string             to_string(uint64_t t) noexcept { return std::format("{}", t); }
    static std::optional<uint64_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<uint64_t>(s); }
};

template <>
struct Serializer<Object> {
    static Object                to_object(const Object& o) noexcept { return o; }
    static std::optional<Object> from_object(const Object& o) noexcept { return o; }
};

template <>
struct Serializer<Array> {
    static Array                to_array(const Array& a) noexcept { return a; }
    static std::optional<Array> from_array(const Array& a) noexcept { return a; }
};

} // namespace jsonc::reflection
