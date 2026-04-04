// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <algorithm>
#include <cstdint>
#include <format>
#include <type_traits>

namespace sculk::reflection {

template <typename T>
    requires std::is_arithmetic_v<T>
struct fixed_number {
    [[nodiscard]] consteval fixed_number() noexcept = default;
    [[nodiscard]] consteval fixed_number(T num) noexcept : storage_(num) {}

    template <typename U>
        requires std::is_arithmetic_v<U>
    [[nodiscard]] consteval fixed_number(U num) noexcept : storage_(static_cast<T>(num)) {}

    [[nodiscard]] constexpr operator T() const noexcept { return storage_; }
    [[nodiscard]] constexpr operator const T&() const noexcept { return storage_; }
    [[nodiscard]] constexpr operator T&() noexcept { return storage_; }

    [[nodiscard]] constexpr T&       operator*() noexcept { return storage_; }
    [[nodiscard]] constexpr const T& operator*() const noexcept { return storage_; }

    [[nodiscard]] constexpr T*       operator->() noexcept { return &storage_; }
    [[nodiscard]] constexpr T const* operator->() const noexcept { return &storage_; }

    friend std::ostream& operator<<(std::ostream& os, const fixed_number& num) noexcept { return os << num.storage_; }

    T storage_;
};

} // namespace sculk::reflection

template <typename T>
struct std::formatter<sculk::reflection::fixed_number<T>> : std::formatter<T> {
    template <typename FormatContext>
    auto format(const sculk::reflection::fixed_number<T>& num, FormatContext& ctx) const {
        return formatter<T>::format(num.storage_, ctx);
    }
};

#if __has_include(<fmt/format.h>)
#include <fmt/format.h>
template <typename T>
struct fmt::formatter<sculk::reflection::fixed_number<T>> : fmt::formatter<T> {
    template <typename FormatContext>
    auto format(const sculk::reflection::fixed_number<T>& num, FormatContext& ctx) const {
        return formatter<T>::format(num.storage_, ctx);
    }
};
#endif