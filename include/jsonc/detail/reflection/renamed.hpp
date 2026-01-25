#pragma once
#include "jsonc/detail/reflection/fixed_string.hpp"

namespace jsonc::reflection {

template <typename T, FixedString AliasName>
class Renamed {
public:
    [[nodiscard]] constexpr Renamed() = default;

    template <typename U>
        requires(std::convertible_to<T, U>)
    [[nodiscard]] constexpr Renamed(U&& value) : storage_(std::forward<U>(value)) {}

    template <typename U>
        requires(!std::convertible_to<T, U> && std::is_constructible_v<T, U>)
    [[nodiscard]] constexpr Renamed(U&& value) : storage_(std::forward<U>(value)) {}

    template <size_t N>
        requires(std::is_same_v<T, std::string>)
    [[nodiscard]] constexpr Renamed(const char (&str)[N]) : storage_(std::string(str)) {}

    template <typename U>
        requires std::is_constructible_v<T, std::initializer_list<U>>
    [[nodiscard]] constexpr Renamed(std::initializer_list<U> init) : storage_(init) {}

    constexpr Renamed(T&& value) : storage_(std::move(value)) {}

    constexpr Renamed(T const& value) : storage_(value) {}

    constexpr Renamed& operator=(T const& value) {
        storage_ = value;
        return *this;
    }

    constexpr Renamed& operator=(T&& value) {
        storage_ = std::move(value);
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

    template <typename U>
        requires(std::convertible_to<T, U> && !std::is_same_v<T, U>)
    [[nodiscard]] constexpr operator U() const {
        return static_cast<U>(storage_);
    }

    template <typename U>
        requires(std::is_same_v<T, U> && std::convertible_to<T, U>)
    [[nodiscard]] constexpr operator const U&() const {
        return storage_;
    }

    template <typename U>
        requires(std::is_same_v<T, U> && std::convertible_to<T, U>)
    [[nodiscard]] constexpr operator U&() {
        return storage_;
    }

public:
    static constexpr std::string      name() { return AliasName.str(); }
    static constexpr std::string_view view() { return AliasName.view(); }

private:
    T storage_;
};

} // namespace jsonc::reflection

// template <typename T, jsonc::reflection::FixedString AliasName>
// struct std::formatter<jsonc::reflection::Renamed<T, AliasName>> : std::formatter<T> {
//     auto format(auto&& value, auto&& ctx) const { return formatter::format(*value, ctx); }
// };