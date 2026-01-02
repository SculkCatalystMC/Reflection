#pragma once
#include <algorithm>
#include <cstdint>
#include <format>
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

// template <typename T>
// struct std::formatter<jsonc_reflection::FixedNumber<T>> : std::formatter<T> {
//     auto format(auto&& num, auto&& ctx) const { return formatter<T>::format(*num, ctx); }
// };