#pragma once
#include "jsonc/detail/reflection/fixed_number.hpp"
#include <limits>

namespace jsonc::reflection {

template <typename T, FixedNumber<T> Min = std::numeric_limits<T>::min(), FixedNumber<T> Max = std::numeric_limits<T>::max()>
    requires std::is_arithmetic_v<T>
class Ranged {
    static_assert(*Min <= *Max, "Min must be less than Max");

    T storage_;

public:
    [[nodiscard]] constexpr Ranged() noexcept = default;

    [[nodiscard]] constexpr Ranged(T val) noexcept : storage_(val) { fix_range(); }

    constexpr Ranged& operator=(T value) noexcept {
        storage_ = value;
        fix_range();
        return *this;
    }

    [[nodiscard]] constexpr T&       storage() noexcept { return storage_; }
    [[nodiscard]] constexpr const T& storage() const noexcept { return storage_; }

    [[nodiscard]] constexpr T&       operator*() noexcept { return storage_; }
    [[nodiscard]] constexpr const T& operator*() const noexcept { return storage_; }

    [[nodiscard]] constexpr T*       operator->() noexcept { return &storage_; }
    [[nodiscard]] constexpr T const* operator->() const noexcept { return &storage_; }

    [[nodiscard]] constexpr operator const T&() const noexcept { return storage_; }
    [[nodiscard]] constexpr operator T&() noexcept { return storage_; }

    constexpr void fix_range() noexcept { storage_ = std::clamp(storage_, *Min, *Max); }

    friend std::ostream& operator<<(std::ostream& os, const Ranged& num) noexcept { return os << num.storage(); }
};

} // namespace jsonc::reflection

template <typename T, jsonc::reflection::FixedNumber<T> Min, jsonc::reflection::FixedNumber<T> Max>
struct std::formatter<jsonc::reflection::Ranged<T, Min, Max>> : std::formatter<T> {
    template <typename FormatContext>
    auto format(const jsonc::reflection::Ranged<T, Min, Max>& num, FormatContext& ctx) const {
        return formatter<T>::format(num.storage(), ctx);
    }
};

#if __has_include(<fmt/format.h>)
#include <fmt/format.h>
template <typename T, jsonc::reflection::FixedNumber<T> Min, jsonc::reflection::FixedNumber<T> Max>
struct fmt::formatter<jsonc::reflection::Ranged<T, Min, Max>> : fmt::formatter<T> {
    template <typename FormatContext>
    auto format(const jsonc::reflection::Ranged<T, Min, Max>& num, FormatContext& ctx) const {
        return formatter<T>::format(num.storage(), ctx);
    }
};
#endif