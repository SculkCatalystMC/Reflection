#pragma once
#include "FixedString.hpp"
#include <vector>

namespace jsonc_reflection {

template <typename T, FixedString... Comments>
class Annotated {
    T                        mStorage;
    std::vector<std::string> mComments{Comments.str()...};

public:
    [[nodiscard]] constexpr Annotated() = default;

    template <typename U>
        requires(std::convertible_to<T, U>)
    [[nodiscard]] constexpr Annotated(U&& value) : mStorage(std::forward<U>(value)) {}

    template <typename U>
        requires(!std::convertible_to<T, U> && std::is_constructible_v<T, U>)
    [[nodiscard]] constexpr Annotated(U&& value) : mStorage(std::forward<U>(value)) {}

    template <size_t N>
        requires(std::is_same_v<T, std::string>)
    [[nodiscard]] constexpr Annotated(const char (&str)[N]) : mStorage(std::string(str)) {}

    template <typename U>
        requires std::is_constructible_v<T, std::initializer_list<U>>
    [[nodiscard]] constexpr Annotated(std::initializer_list<U> init) : mStorage(init) {}

    constexpr Annotated(T&& value) : mStorage(std::move(value)) {}

    constexpr Annotated(T const& value) : mStorage(value) {}

    constexpr Annotated& operator=(T const& value) {
        mStorage = value;
        return *this;
    }

    constexpr Annotated& operator=(T&& value) {
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

    [[nodiscard]] constexpr bool hasComments() const noexcept { return mComments.size() > 0; }

    [[nodiscard]] constexpr std::vector<std::string>&       getComments() noexcept { return mComments; }
    [[nodiscard]] constexpr const std::vector<std::string>& getComments() const noexcept { return mComments; }

    constexpr void addComment(std::string_view comment) { mComments.emplace_back(comment); }

    constexpr void setComment(size_t index, std::string_view newComment) {
        if (index < mComments.size()) { mComments[index] = newComment; }
    }

    constexpr void removeComment(size_t index) {
        if (index < mComments.size()) { mComments.erase(mComments.begin() + index); }
    }

    constexpr void clearComments() { mComments.clear(); }
};

} // namespace jsonc_reflection

namespace std {
template <typename T, jsonc_reflection::FixedString... Comments>
struct formatter<jsonc_reflection::Annotated<T, Comments...>> : formatter<T> {
    auto format(const jsonc_reflection::Annotated<T, Comments...>& obj, format_context& ctx) const {
        return formatter<T>::format(obj.storage(), ctx);
    }
};
} // namespace std
