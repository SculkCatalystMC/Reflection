#pragma once
#include <algorithm>
#include <cstddef>
#include <format>
#include <string>
#include <string_view>

namespace jsonc::reflection {

template <size_t N>
struct FixedString {
    [[nodiscard]] consteval FixedString() noexcept = default;
    [[nodiscard]] consteval FixedString(std::string_view str) noexcept { std::copy_n(str.data(), str.size(), buffer_); }
    [[nodiscard]] consteval FixedString(char const* str) noexcept { std::copy_n(str, N, buffer_); }

    [[nodiscard]] constexpr operator const char*() const noexcept { return buffer_.c_str(); }
    [[nodiscard]] constexpr operator std::string_view() const noexcept { return buffer_; }
    [[nodiscard]] constexpr operator std::string() const noexcept { return buffer_; }

    [[nodiscard]] constexpr const char*      c_str() const noexcept { return buffer_; }
    [[nodiscard]] constexpr std::string_view view() const noexcept { return buffer_; }
    [[nodiscard]] constexpr std::string      str() const noexcept { return buffer_; }
    [[nodiscard]] constexpr size_t           size() const noexcept { return N; }
    [[nodiscard]] constexpr bool             empty() const noexcept { return N == 0; }

    [[nodiscard]] constexpr const char& operator[](size_t index) const noexcept { return buffer_[index]; }
    [[nodiscard]] constexpr char&       operator[](size_t index) noexcept { return buffer_[index]; }

    template <size_t Y>
    consteval auto operator+(const FixedString<Y>& other) noexcept {
        FixedString<N + Y> result{};
        std::copy_n(buffer_, N, result.buffer_);
        std::copy_n(other.buffer_, Y, N + result.buffer_);
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const FixedString& str) noexcept { return os << str.view(); }

    char buffer_[N + 1]{};
};

template <size_t N>
FixedString(char const (&)[N]) -> FixedString<N - 1>;

} // namespace jsonc::reflection

template <size_t N>
struct std::formatter<jsonc::reflection::FixedString<N>> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const jsonc::reflection::FixedString<N>& str, FormatContext& ctx) const {
        return formatter<std::string_view>::format(str.view(), ctx);
    }
};

#if __has_include(<fmt/format.h>)
#include <fmt/format.h>
template <size_t N>
struct fmt::formatter<jsonc::reflection::FixedString<N>> : fmt::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const jsonc::reflection::FixedString<N>& str, FormatContext& ctx) const {
        return formatter<std::string_view>::format(str.view(), ctx);
    }
};
#endif