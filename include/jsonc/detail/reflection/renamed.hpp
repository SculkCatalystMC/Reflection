#pragma once
#include "jsonc/detail/reflection/fixed_string.hpp"

namespace jsonc::reflection {

template <typename T, FixedString AliasName>
class Renamed {
    T mStorage;

public:
    [[nodiscard]] constexpr Renamed() = default;

    template <typename U>
        requires(std::convertible_to<T, U>)
    [[nodiscard]] constexpr Renamed(U&& value) : mStorage(std::forward<U>(value)) {}

    template <typename U>
        requires(!std::convertible_to<T, U> && std::is_constructible_v<T, U>)
    [[nodiscard]] constexpr Renamed(U&& value) : mStorage(std::forward<U>(value)) {}

    template <size_t N>
        requires(std::is_same_v<T, std::string>)
    [[nodiscard]] constexpr Renamed(const char (&str)[N]) : mStorage(std::string(str)) {}

    template <typename U>
        requires std::is_constructible_v<T, std::initializer_list<U>>
    [[nodiscard]] constexpr Renamed(std::initializer_list<U> init) : mStorage(init) {}

    constexpr Renamed(T&& value) : mStorage(std::move(value)) {}

    constexpr Renamed(T const& value) : mStorage(value) {}

    constexpr Renamed& operator=(T const& value) {
        mStorage = value;
        return *this;
    }

    constexpr Renamed& operator=(T&& value) {
        mStorage = std::move(value);
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

    template <typename U>
        requires(std::convertible_to<T, U> && !std::is_same_v<T, U>)
    [[nodiscard]] constexpr operator U() const {
        return static_cast<U>(mStorage);
    }

    template <typename U>
        requires(std::is_same_v<T, U> && std::convertible_to<T, U>)
    [[nodiscard]] constexpr operator const U&() const {
        return mStorage;
    }

    template <typename U>
        requires(std::is_same_v<T, U> && std::convertible_to<T, U>)
    [[nodiscard]] constexpr operator U&() {
        return mStorage;
    }

public:
    static constexpr std::string      name() { return AliasName.str(); }
    static constexpr std::string_view view() { return AliasName.view(); }
};

} // namespace jsonc::reflection

// template <typename T, jsonc::reflection::FixedString AliasName>
// struct std::formatter<jsonc::reflection::Renamed<T, AliasName>> : std::formatter<T> {
//     auto format(auto&& value, auto&& ctx) const { return formatter::format(*value, ctx); }
// };