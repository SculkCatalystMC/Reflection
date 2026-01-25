#pragma once
#include <optional>
#include <string>

namespace jsonc::reflection {

template <typename T>
struct Serializer {
    using value_type = T;
};

template <>
struct Serializer<bool> {
    static std::string         to_string(const bool& t) noexcept { return t ? "true" : "false"; }
    static std::optional<bool> from_string(std::string_view s) noexcept {
        if (s == "true") { return true; }
        if (s == "false") { return false; }
        return std::nullopt;
    }
};

template <>
struct Serializer<uint8_t> {
    static std::string            to_string(const uint8_t& t) noexcept { return std::to_string(t); }
    static std::optional<uint8_t> from_string(std::string_view s) noexcept { return std::stoi(s.data()); }
};

template <>
struct Serializer<int> {
    static std::string        to_string(const int& t) noexcept { return std::to_string(t); }
    static std::optional<int> from_string(std::string_view s) noexcept { return std::stoi(s.data()); }
};

} // namespace jsonc::reflection
