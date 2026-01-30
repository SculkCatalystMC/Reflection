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
    [[nodiscard]] constexpr operator const T&() const noexcept { return storage_; }
    [[nodiscard]] constexpr operator T&() noexcept { return storage_; }

    [[nodiscard]] constexpr T&       operator*() noexcept { return storage_; }
    [[nodiscard]] constexpr const T& operator*() const noexcept { return storage_; }

    [[nodiscard]] constexpr T*       operator->() noexcept { return &storage_; }
    [[nodiscard]] constexpr T const* operator->() const noexcept { return &storage_; }

    friend std::ostream& operator<<(std::ostream& os, const FixedNumber& num) noexcept { return os << num.storage_; }

    T storage_;
};

} // namespace jsonc::reflection

template <typename T>
struct std::formatter<jsonc::reflection::FixedNumber<T>> : std::formatter<T> {
    template <typename FormatContext>
    auto format(const jsonc::reflection::FixedNumber<T>& num, FormatContext& ctx) const {
        return formatter<T>::format(num.storage_, ctx);
    }
};

#if __has_include(<fmt/format.h>)
#include <fmt/format.h>
template <typename T>
struct fmt::formatter<jsonc::reflection::FixedNumber<T>> : fmt::formatter<T> {
    template <typename FormatContext>
    auto format(const jsonc::reflection::FixedNumber<T>& num, FormatContext& ctx) const {
        return formatter<T>::format(num.storage_, ctx);
    }
};
#endif