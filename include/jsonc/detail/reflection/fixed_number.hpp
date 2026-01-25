#pragma once
#include <algorithm>
#include <cstdint>
#include <format>
#include <type_traits>

namespace jsonc::reflection {

template <typename T>
    requires std::is_arithmetic_v<T>
struct FixedNumber {
    [[nodiscard]] consteval FixedNumber() noexcept = default;
    [[nodiscard]] consteval FixedNumber(T num) noexcept : storage_(num) {}

    template <typename U>
        requires std::is_arithmetic_v<U>
    [[nodiscard]] consteval FixedNumber(U num) noexcept : storage_(static_cast<T>(num)) {}

    [[nodiscard]] constexpr operator T() const noexcept { return storage_; }
    [[nodiscard]] constexpr operator T const&() const noexcept { return storage_; }
    [[nodiscard]] constexpr operator T&() noexcept { return storage_; }

    [[nodiscard]] constexpr T&       operator*() { return storage_; }
    [[nodiscard]] constexpr T const& operator*() const { return storage_; }

    [[nodiscard]] constexpr T*       operator->() { return &storage_; }
    [[nodiscard]] constexpr T const* operator->() const { return &storage_; }

    T storage_;
};

} // namespace jsonc::reflection
