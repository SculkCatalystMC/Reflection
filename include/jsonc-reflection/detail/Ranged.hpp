#pragma once
#include "endgate/base/FixedNumber.hpp"
#include <limits>

namespace jsonc_reflection {

template <
    typename T,
    FixedNumber<T> Min = std::numeric_limits<T>::min(),
    FixedNumber<T> Max = std::numeric_limits<T>::max()>
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

namespace fmt {
template <typename T, endgate::FixedNumber<T> Min, endgate::FixedNumber<T> Max>
struct formatter<endgate::Ranged<T, Min, Max>> : formatter<T> {
    template <typename FormatContext>
    auto format(const endgate::Ranged<T, Min, Max>& num, FormatContext& ctx) const {
        return formatter<T>::format(num.storage(), ctx);
    }
};
} // namespace fmt

namespace std {
template <typename T, endgate::FixedNumber<T> Min, endgate::FixedNumber<T> Max>
struct formatter<endgate::Ranged<T, Min, Max>> : formatter<T> {
    auto format(const endgate::Ranged<T, Min, Max>& num, format_context& ctx) const {
        return formatter<T>::format(num.storage(), ctx);
    }
};
} // namespace std