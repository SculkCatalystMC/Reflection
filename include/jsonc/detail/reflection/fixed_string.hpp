#pragma once
#include <algorithm>
#include <cstddef>
#include <format>
#include <string>
#include <string_view>

namespace jsonc::reflection {

template <size_t N>
struct fixed_string {
    [[nodiscard]] consteval fixed_string() noexcept = default;
    [[nodiscard]] consteval fixed_string(std::string_view str) noexcept { std::copy_n(str.data(), str.size(), buffer_); }
    [[nodiscard]] consteval fixed_string(char const* str) noexcept { std::copy_n(str, N, buffer_); }

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
    consteval auto operator+(const fixed_string<Y>& other) noexcept {
        fixed_string<N + Y> result{};
        std::copy_n(buffer_, N, result.buffer_);
        std::copy_n(other.buffer_, Y, N + result.buffer_);
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const fixed_string& str) noexcept { return os << str.view(); }

    char buffer_[N + 1]{};
};

template <size_t N>
fixed_string(char const (&)[N]) -> fixed_string<N - 1>;

} // namespace jsonc::reflection

template <size_t N>
struct std::formatter<jsonc::reflection::fixed_string<N>> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const jsonc::reflection::fixed_string<N>& str, FormatContext& ctx) const {
        return formatter<std::string_view>::format(str.view(), ctx);
    }
};

#if __has_include(<fmt/format.h>)
#include <fmt/format.h>
template <size_t N>
struct fmt::formatter<jsonc::reflection::fixed_string<N>> : fmt::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const jsonc::reflection::fixed_string<N>& str, FormatContext& ctx) const {
        return formatter<std::string_view>::format(str.view(), ctx);
    }
};
#endif