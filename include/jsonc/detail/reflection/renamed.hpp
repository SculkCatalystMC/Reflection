#pragma once
#include "jsonc/detail/reflection/fixed_string.hpp"

namespace jsonc::reflection {

template <typename T, FixedString AliasName>
class Renamed {
public:
    using value_type = T;

    [[nodiscard]] constexpr Renamed() noexcept = default;

    template <typename U>
        requires(std::convertible_to<T, U>)
    [[nodiscard]] constexpr Renamed(U&& value) noexcept : storage_(std::forward<U>(value)) {}

    template <typename U>
        requires(!std::convertible_to<T, U> && std::is_constructible_v<T, U>)
    [[nodiscard]] constexpr Renamed(U&& value) noexcept : storage_(std::forward<U>(value)) {}

    template <size_t N>
        requires(std::is_same_v<T, std::string>)
    [[nodiscard]] constexpr Renamed(const char (&str)[N]) noexcept : storage_(std::string(str)) {}

    template <typename U>
        requires std::is_nothrow_constructible_v<T, std::initializer_list<U>>
    [[nodiscard]] constexpr Renamed(std::initializer_list<U> init) noexcept : storage_(init) {}

    constexpr Renamed(T&& value) noexcept : storage_(std::move(value)) {}

    constexpr Renamed(const T& value) noexcept : storage_(value) {}

    constexpr Renamed& operator=(const T& value) noexcept {
        storage_ = value;
        return *this;
    }

    constexpr Renamed& operator=(T&& value) noexcept {
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

    friend std::ostream& operator<<(std::ostream& os, const Renamed& rename) noexcept { return os << rename.storage(); }

public:
    static constexpr std::string      name() noexcept { return AliasName.str(); }
    static constexpr std::string_view view() noexcept { return AliasName.view(); }

private:
    T storage_;
};

} // namespace jsonc::reflection

template <typename T, size_t N, jsonc::reflection::FixedString<N> AliasName>
struct std::formatter<jsonc::reflection::Renamed<T, AliasName>> : std::formatter<T> {
    template <typename FormatContext>
    auto format(const jsonc::reflection::Renamed<T, AliasName>& str, FormatContext& ctx) const {
        return formatter<T>::format(str.storage(), ctx);
    }
};

#if __has_include(<fmt/format.h>)
#include <fmt/format.h>
template <typename T, jsonc::reflection::FixedString AliasName>
struct fmt::formatter<jsonc::reflection::Renamed<T, AliasName>> : fmt::formatter<T> {
    template <typename FormatContext>
    auto format(const jsonc::reflection::Renamed<T, AliasName>& str, FormatContext& ctx) const {
        return formatter<T>::format(str.storage(), ctx);
    }
};
#endif