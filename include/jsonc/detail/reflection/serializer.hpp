#pragma once
#include "jsonc/detail/reflection/jsonc_header.hpp"
#include <charconv>
#include <format>
#include <optional>
#include <string>

namespace jsonc::reflection {

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
struct serializer<int8_t> {
    static std::string           to_string(int8_t t) noexcept { return std::format("{}", t); }
    static std::optional<int8_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<int8_t>(s); }
};

template <>
struct serializer<uint8_t> {
    static std::string            to_string(uint8_t t) noexcept { return std::format("{}", t); }
    static std::optional<uint8_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<uint8_t>(s); }
};

template <>
struct serializer<short> {
    static std::string          to_string(short t) noexcept { return std::format("{}", t); }
    static std::optional<short> from_string(std::string_view s) noexcept { return string_utils::str_to_num<short>(s); }
};

template <>
struct serializer<uint16_t> {
    static std::string             to_string(uint16_t t) noexcept { return std::format("{}", t); }
    static std::optional<uint16_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<uint16_t>(s); }
};

template <>
struct serializer<int> {
    static std::string        to_string(int t) noexcept { return std::format("{}", t); }
    static std::optional<int> from_string(std::string_view s) noexcept { return string_utils::str_to_num<int>(s); }
};

template <>
struct serializer<uint32_t> {
    static std::string             to_string(uint32_t t) noexcept { return std::format("{}", t); }
    static std::optional<uint32_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<uint32_t>(s); }
};

template <>
struct serializer<int64_t> {
    static std::string            to_string(int64_t t) noexcept { return std::format("{}", t); }
    static std::optional<int64_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<int64_t>(s); }
};

template <>
struct serializer<uint64_t> {
    static std::string             to_string(uint64_t t) noexcept { return std::format("{}", t); }
    static std::optional<uint64_t> from_string(std::string_view s) noexcept { return string_utils::str_to_num<uint64_t>(s); }
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

template <>
struct serializer<json::object_type> {
    static json::object_type                to_object(const json::object_type& o) noexcept { return o; }
    static std::optional<json::object_type> from_object(const json::object_type& o) noexcept { return o; }
};

template <>
struct serializer<json::array_type> {
    static json::array_type                to_array(const json::array_type& a) noexcept { return a; }
    static std::optional<json::array_type> from_array(const json::array_type& a) noexcept { return a; }
};

template <>
struct serializer<ordered_json::object_type> {
    static ordered_json::object_type                to_object(const ordered_json::object_type& o) noexcept { return o; }
    static std::optional<ordered_json::object_type> from_object(const ordered_json::object_type& o) noexcept { return o; }
};

template <>
struct serializer<ordered_json::array_type> {
    static ordered_json::array_type                to_array(const ordered_json::array_type& a) noexcept { return a; }
    static std::optional<ordered_json::array_type> from_array(const ordered_json::array_type& a) noexcept { return a; }
};

template <>
struct serializer<jsonc::object_type> {
    static jsonc::object_type                to_object(const jsonc::object_type& o) noexcept { return o; }
    static std::optional<jsonc::object_type> from_object(const jsonc::object_type& o) noexcept { return o; }
};

template <>
struct serializer<jsonc::array_type> {
    static jsonc::array_type                to_array(const jsonc::array_type& a) noexcept { return a; }
    static std::optional<jsonc::array_type> from_array(const jsonc::array_type& a) noexcept { return a; }
};

template <>
struct serializer<ordered_jsonc::object_type> {
    static ordered_jsonc::object_type                to_object(const ordered_jsonc::object_type& o) noexcept { return o; }
    static std::optional<ordered_jsonc::object_type> from_object(const ordered_jsonc::object_type& o) noexcept { return o; }
};

template <>
struct serializer<ordered_jsonc::array_type> {
    static ordered_jsonc::array_type                to_array(const ordered_jsonc::array_type& a) noexcept { return a; }
    static std::optional<ordered_jsonc::array_type> from_array(const ordered_jsonc::array_type& a) noexcept { return a; }
};

} // namespace jsonc::reflection
