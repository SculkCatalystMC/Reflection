#pragma once
#include <algorithm>
#include <cstdint>
#include <fmt/format.h>
#include <format>
#include <type_traits>

namespace jsonc_reflection {

template <typename T>
    requires std::is_arithmetic_v<T>
struct FixedNumber {
    T mStorage;

    [[nodiscard]] consteval FixedNumber() noexcept = default;
    [[nodiscard]] consteval FixedNumber(T num) noexcept : mStorage(num) {}

    template <typename U>
        requires std::is_arithmetic_v<U>
    [[nodiscard]] consteval FixedNumber(U num) noexcept : mStorage(static_cast<T>(num)) {}

    [[nodiscard]] constexpr operator T() const noexcept { return mStorage; }
    [[nodiscard]] constexpr operator T const&() const noexcept { return mStorage; }
    [[nodiscard]] constexpr operator T&() noexcept { return mStorage; }

    [[nodiscard]] constexpr T&       operator*() { return mStorage; }
    [[nodiscard]] constexpr T const& operator*() const { return mStorage; }

    [[nodiscard]] constexpr T*       operator->() { return &mStorage; }
    [[nodiscard]] constexpr T const* operator->() const { return &mStorage; }
};

} // namespace jsonc_reflection

namespace fmt {
template <typename T>
struct formatter<endgate::FixedNumber<T>> : formatter<T> {
    template <typename FormatContext>
    auto format(const endgate::FixedNumber<T>& num, FormatContext& ctx) const {
        return formatter<T>::format(*num, ctx);
    }
};
} // namespace fmt

namespace std {
template <typename T>
struct formatter<endgate::FixedNumber<T>> : formatter<T> {
    auto format(const endgate::FixedNumber<T>& num, format_context& ctx) const {
        return formatter<T>::format(*num, ctx);
    }
};
} // namespace std