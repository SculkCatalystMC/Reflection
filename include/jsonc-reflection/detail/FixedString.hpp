#pragma once
#include <algorithm>
#include <cstddef>
#include <format>
#include <string>
#include <string_view>

namespace jsonc_reflection {

template <size_t N>
struct FixedString {
    char mBuffer[N + 1]{};

    [[nodiscard]] consteval FixedString() noexcept = default;
    [[nodiscard]] consteval FixedString(std::string_view str) noexcept { std::copy_n(str.data(), str.size(), mBuffer); }
    [[nodiscard]] consteval FixedString(char const* str) noexcept { std::copy_n(str, N, mBuffer); }

    [[nodiscard]] constexpr operator const char*() const noexcept { return mBuffer.c_str(); }
    [[nodiscard]] constexpr operator std::string_view() const noexcept { return mBuffer; }
    [[nodiscard]] constexpr operator std::string() const noexcept { return mBuffer; }

    [[nodiscard]] constexpr const char*      c_str() const noexcept { return mBuffer; }
    [[nodiscard]] constexpr std::string_view view() const noexcept { return mBuffer; }
    [[nodiscard]] constexpr std::string      str() const noexcept { return mBuffer; }

    [[nodiscard]] constexpr char const& operator[](size_t index) const noexcept { return mBuffer[index]; }
    [[nodiscard]] constexpr char&       operator[](size_t index) noexcept { return mBuffer[index]; }

    template <size_t Y>
    consteval auto operator+(FixedString<Y> const& other) noexcept {
        FixedString<N + Y> result{};
        std::copy_n(mBuffer, N, result.mBuffer);
        std::copy_n(other.mBuffer, Y, N + result.mBuffer);
        return result;
    }
};

template <size_t N>
FixedString(char const (&)[N]) -> FixedString<N - 1>;

} // namespace jsonc_reflection

namespace std {
template <size_t N>
struct formatter<jsonc_reflection::FixedString<N>> : formatter<string_view> {
    auto format(const jsonc_reflection::FixedString<N>& str, format_context& ctx) const { return formatter<string_view>::format(str.view(), ctx); }
};
} // namespace std
