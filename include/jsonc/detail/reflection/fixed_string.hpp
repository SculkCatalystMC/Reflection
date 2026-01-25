#pragma once
#include <algorithm>
#include <cstddef>
#include <format>
#include <string>
#include <string_view>

namespace jsonc::reflection {

template <size_t N>
struct FixedString {
    char buffer_[N + 1]{};

    [[nodiscard]] consteval FixedString() noexcept = default;
    [[nodiscard]] consteval FixedString(std::string_view str) noexcept { std::copy_n(str.data(), str.size(), buffer_); }
    [[nodiscard]] consteval FixedString(char const* str) noexcept { std::copy_n(str, N, buffer_); }

    [[nodiscard]] constexpr operator const char*() const noexcept { return buffer_.c_str(); }
    [[nodiscard]] constexpr operator std::string_view() const noexcept { return buffer_; }
    [[nodiscard]] constexpr operator std::string() const noexcept { return buffer_; }

    [[nodiscard]] constexpr const char*      c_str() const noexcept { return buffer_; }
    [[nodiscard]] constexpr std::string_view view() const noexcept { return buffer_; }
    [[nodiscard]] constexpr std::string      str() const noexcept { return buffer_; }

    [[nodiscard]] constexpr char const& operator[](size_t index) const noexcept { return buffer_[index]; }
    [[nodiscard]] constexpr char&       operator[](size_t index) noexcept { return buffer_[index]; }

    template <size_t Y>
    consteval auto operator+(FixedString<Y> const& other) noexcept {
        FixedString<N + Y> result{};
        std::copy_n(buffer_, N, result.buffer_);
        std::copy_n(other.buffer_, Y, N + result.buffer_);
        return result;
    }
};

template <size_t N>
FixedString(char const (&)[N]) -> FixedString<N - 1>;

} // namespace jsonc::reflection
