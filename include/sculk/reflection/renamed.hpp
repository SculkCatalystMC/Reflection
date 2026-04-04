// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/reflection/fixed_string.hpp"

namespace sculk::reflection {

template <typename T, fixed_string AliasName>
class renamed {
public:
    using value_type = T;

    [[nodiscard]] constexpr renamed() noexcept = default;

    template <typename U>
        requires(std::convertible_to<T, U>)
    [[nodiscard]] constexpr renamed(U&& value) noexcept : storage_(std::forward<U>(value)) {}

    template <typename U>
        requires(!std::convertible_to<T, U> && std::is_constructible_v<T, U>)
    [[nodiscard]] constexpr renamed(U&& value) noexcept : storage_(std::forward<U>(value)) {}

    template <size_t N>
        requires(std::is_same_v<T, std::string>)
    [[nodiscard]] constexpr renamed(const char (&str)[N]) noexcept : storage_(std::string(str)) {}

    template <typename U>
        requires std::is_nothrow_constructible_v<T, std::initializer_list<U>>
    [[nodiscard]] constexpr renamed(std::initializer_list<U> init) noexcept : storage_(init) {}

    constexpr renamed(T&& value) noexcept : storage_(std::move(value)) {}

    constexpr renamed(const T& value) noexcept : storage_(value) {}

    constexpr renamed& operator=(const T& value) noexcept {
        storage_ = value;
        return *this;
    }

    constexpr renamed& operator=(T&& value) noexcept {
        storage_ = std::move(value);
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

    template <typename U>
        requires(std::convertible_to<T, U> && !std::is_same_v<T, U>)
    [[nodiscard]] constexpr operator U() const noexcept {
        return static_cast<U>(storage_);
    }

    template <typename U>
        requires(std::is_same_v<T, U> && std::is_nothrow_convertible_v<T, U>)
    [[nodiscard]] constexpr operator const U&() const noexcept {
        return storage_;
    }

    template <typename U>
        requires(std::is_same_v<T, U> && std::is_nothrow_convertible_v<T, U>)
    [[nodiscard]] constexpr operator U&() noexcept {
        return storage_;
    }

    friend std::ostream& operator<<(std::ostream& os, const renamed& rename) noexcept { return os << rename.storage(); }

public:
    static constexpr std::string      name() noexcept { return AliasName.str(); }
    static constexpr std::string_view view() noexcept { return AliasName.view(); }

private:
    T storage_;
};

} // namespace sculk::reflection

template <typename T, size_t N, sculk::reflection::fixed_string<N> AliasName>
struct std::formatter<sculk::reflection::renamed<T, AliasName>> : std::formatter<T> {
    template <typename FormatContext>
    auto format(const sculk::reflection::renamed<T, AliasName>& str, FormatContext& ctx) const {
        return formatter<T>::format(str.storage(), ctx);
    }
};

#if __has_include(<fmt/format.h>)
#include <fmt/format.h>
template <typename T, sculk::reflection::fixed_string AliasName>
struct fmt::formatter<sculk::reflection::renamed<T, AliasName>> : fmt::formatter<T> {
    template <typename FormatContext>
    auto format(const sculk::reflection::renamed<T, AliasName>& str, FormatContext& ctx) const {
        return formatter<T>::format(str.storage(), ctx);
    }
};
#endif