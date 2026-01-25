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
    [[nodiscard]] constexpr Ranged() = default;

    [[nodiscard]] constexpr Ranged(T val) : storage_(val) { fix_range(); }

    constexpr Ranged& operator=(T value) {
        storage_ = value;
        fix_range();
        return *this;
    }

    [[nodiscard]] constexpr T&       storage() { return storage_; }
    [[nodiscard]] constexpr T const& storage() const { return storage_; }

    [[nodiscard]] constexpr T&       operator*() { return storage_; }
    [[nodiscard]] constexpr T const& operator*() const { return storage_; }

    [[nodiscard]] constexpr T*       operator->() { return &storage_; }
    [[nodiscard]] constexpr T const* operator->() const { return &storage_; }

    [[nodiscard]] constexpr operator T const&() const { return storage_; }
    [[nodiscard]] constexpr operator T&() { return storage_; }

    constexpr void fix_range() { storage_ = std::clamp(storage_, *Min, *Max); }

    friend std::ostream& operator<<(std::ostream& os, const Ranged& num) { return os << num.storage(); }
};

} // namespace jsonc::reflection

// template <typename T, jsonc::reflection::FixedNumber<T> Min, jsonc::reflection::FixedNumber<T> Max>
// struct std::formatter<jsonc::reflection::Ranged<T, Min, Max>> : std::formatter<T> {
//     auto format(auto&& num, auto&& ctx) const { return formatter::format(num.storage(), ctx); }
// };