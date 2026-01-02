#pragma once
#include "jsonc-reflection/detail/FixedNumber.hpp"
#include <limits>

namespace jsonc_reflection {

template <typename T, FixedNumber<T> Min = std::numeric_limits<T>::min(), FixedNumber<T> Max = std::numeric_limits<T>::max()>
    requires std::is_arithmetic_v<T>
class Ranged {
    static_assert(*Min <= *Max, "Min must be less than Max");

    T mStorage;

public:
    [[nodiscard]] constexpr Ranged() = default;

    [[nodiscard]] constexpr Ranged(T val) : mStorage(val) { fixRange(); }

    constexpr Ranged& operator=(T value) {
        mStorage = value;
        fixRange();
        return *this;
    }

    [[nodiscard]] constexpr T&       storage() { return mStorage; }
    [[nodiscard]] constexpr T const& storage() const { return mStorage; }

    [[nodiscard]] constexpr T&       operator*() { return mStorage; }
    [[nodiscard]] constexpr T const& operator*() const { return mStorage; }

    [[nodiscard]] constexpr T*       operator->() { return &mStorage; }
    [[nodiscard]] constexpr T const* operator->() const { return &mStorage; }

    [[nodiscard]] constexpr operator T const&() const { return mStorage; }
    [[nodiscard]] constexpr operator T&() { return mStorage; }

    constexpr void fixRange() { mStorage = std::clamp(mStorage, *Min, *Max); }

    friend std::ostream& operator<<(std::ostream& os, const Ranged& num) { return os << num.storage(); }
};

} // namespace jsonc_reflection

// template <typename T, jsonc_reflection::FixedNumber<T> Min, jsonc_reflection::FixedNumber<T> Max>
// struct std::formatter<jsonc_reflection::Ranged<T, Min, Max>> : std::formatter<T> {
//     auto format(auto&& num, auto&& ctx) const { return formatter::format(num.storage(), ctx); }
// };