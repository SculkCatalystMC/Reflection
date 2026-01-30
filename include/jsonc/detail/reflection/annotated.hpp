#pragma once
#include "jsonc/detail/reflection/fixed_string.hpp"
#include <vector>
#include <format>

namespace jsonc::reflection {

template <typename T, FixedString... Comments>
class Annotated {
public:
    using value_type = T;

    [[nodiscard]] constexpr Annotated() noexcept = default;

    template <typename U>
        requires(std::convertible_to<T, U>)
    [[nodiscard]] constexpr Annotated(U&& value) noexcept : storage_(std::forward<U>(value)) {}

    template <typename U>
        requires(!std::convertible_to<T, U> && std::is_nothrow_constructible_v<T, U>)
    [[nodiscard]] constexpr Annotated(U&& value) noexcept : storage_(std::forward<U>(value)) {}

    template <size_t N>
        requires(std::is_same_v<T, std::string>)
    [[nodiscard]] constexpr Annotated(const char (&str)[N]) noexcept : storage_(std::string(str)) {}

    template <typename U>
        requires std::is_constructible_v<T, std::initializer_list<U>>
    [[nodiscard]] constexpr Annotated(std::initializer_list<U> init) noexcept : storage_(init) {}

    constexpr Annotated(T&& value) noexcept : storage_(std::move(value)) {}

    constexpr Annotated(T const& value) noexcept : storage_(value) {}

    constexpr Annotated& operator=(T const& value) noexcept {
        storage_ = value;
        return *this;
    }

    constexpr Annotated& operator=(T&& value) noexcept {
        storage_ = std::move(value);
        return *this;
    }

    [[nodiscard]] constexpr T&       storage() noexcept { return storage_; }
    [[nodiscard]] constexpr T const& storage() const noexcept { return storage_; }

    [[nodiscard]] constexpr T&       operator*() noexcept { return storage_; }
    [[nodiscard]] constexpr T const& operator*() const noexcept { return storage_; }

    [[nodiscard]] constexpr T*       operator->() noexcept { return &storage_; }
    [[nodiscard]] constexpr T const* operator->() const noexcept { return &storage_; }

    [[nodiscard]] constexpr operator T const&() const noexcept { return storage_; }
    [[nodiscard]] constexpr operator T&() noexcept { return storage_; }

    template <typename U>
        requires(std::convertible_to<T, U> && !std::is_same_v<T, U>)
    [[nodiscard]] constexpr operator U() const noexcept {
        return static_cast<U>(storage_);
    }

    template <typename U>
        requires(std::is_same_v<T, U> && std::convertible_to<T, U>)
    [[nodiscard]] constexpr operator const U&() const noexcept {
        return storage_;
    }

    template <typename U>
        requires(std::is_same_v<T, U> && std::convertible_to<T, U>)
    [[nodiscard]] constexpr operator U&() noexcept {
        return storage_;
    }

    [[nodiscard]] constexpr bool has_comments() const noexcept { return comments_.size() > 0; }

    [[nodiscard]] constexpr std::vector<std::string>&       get_comments() noexcept { return comments_; }
    [[nodiscard]] constexpr const std::vector<std::string>& get_comments() const noexcept { return comments_; }

    constexpr void add_comment(std::string_view comment) noexcept { comments_.emplace_back(comment); }

    constexpr void set_comments(const std::vector<std::string>& comments) noexcept { comments_ = comments; }

    constexpr void set_comment(size_t index, std::string_view newComment) noexcept {
        if (index < comments_.size()) { comments_[index] = newComment; }
    }

    constexpr void remove_comment(size_t index) noexcept {
        if (index < comments_.size()) { comments_.erase(comments_.begin() + index); }
    }

    constexpr void clear_comments() noexcept { comments_.clear(); }

    friend std::ostream& operator<<(std::ostream& os, Annotated const& annotated) noexcept { return os << annotated.storage(); }

private:
    T                        storage_;
    std::vector<std::string> comments_{Comments.str()...};
};

} // namespace jsonc::reflection

template <typename T, jsonc::reflection::FixedString... Comments>
struct std::formatter<jsonc::reflection::Annotated<T, Comments...>> : std::formatter<T> {
    template <typename FormatContext>
    auto format(jsonc::reflection::Annotated<T, Comments...> const& annotated, FormatContext& ctx) const {
        return formatter<T>::format(annotated.storage(), ctx);
    }
};

#if __has_include(<fmt/format.h>)
#include <fmt/format.h>
template <typename T, jsonc::reflection::FixedString... Comments>
struct fmt::formatter<jsonc::reflection::Annotated<T, Comments...>> : fmt::formatter<T> {
    template <typename FormatContext>
    auto format(jsonc::reflection::Annotated<T, Comments...> const& annotated, FormatContext& ctx) const {
        return formatter<T>::format(annotated.storage(), ctx);
    }
};
#endif