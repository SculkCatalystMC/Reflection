#pragma once
#include "sculk/reflection/concepts.hpp"
#include <format>

namespace sculk::reflection {

template <typename T, concepts::is_dispatcher_listener<T> L, bool _CallInit = false>
class dispatcher {
public:
    using storage_type  = T;
    using listener_type = L;

    void call() noexcept {
        if constexpr (concepts::has_call_func_with_arg<L, T>) {
            listener_.call(storage_);
        } else {
            listener_.call();
        }
    }

    template <typename... Args>
        requires std::constructible_from<T, Args...>
    dispatcher(Args&&... args) noexcept : storage_(std::forward<Args>(args)...),
                                          listener_() {
        if constexpr (_CallInit) { call(); }
    }

    dispatcher& operator=(const T& other) noexcept {
        storage_ = other;
        call();
        return *this;
    }

    dispatcher& operator=(T&& other) noexcept {
        storage_ = std::move(other);
        call();
        return *this;
    }

    operator const T&() const noexcept { return storage_; }

    operator T&() noexcept { return storage_; }

    const T& operator*() const noexcept { return storage_; }

    T& operator*() noexcept { return storage_; }

    T const* operator->() const noexcept { return &storage_; }

    T* operator->() noexcept { return &storage_; }

    T& storage() noexcept { return storage_; }

    const T& storage() const noexcept { return storage_; }

    L& listener() noexcept { return listener_; }

    const L& listener() const noexcept { return listener_; }

    friend std::ostream& operator<<(std::ostream& os, const dispatcher& dispatcher) noexcept { return os << dispatcher.storage(); }

private:
    T storage_;
    L listener_;
};

} // namespace sculk::reflection

template <typename T, sculk::reflection::concepts::is_dispatcher_listener<T> L, bool _CallInit>
struct std::formatter<sculk::reflection::dispatcher<T, L, _CallInit>> : std::formatter<T> {
    template <typename FormatContext>
    auto format(const sculk::reflection::dispatcher<T, L, _CallInit>& dispatcher, FormatContext& ctx) const {
        return formatter<T>::format(dispatcher.storage(), ctx);
    }
};

#if __has_include(<fmt/format.h>)
#include <fmt/format.h>
template <typename T, sculk::reflection::concepts::is_dispatcher_listener<T> L, bool _CallInit>
struct fmt::formatter<sculk::reflection::dispatcher<T, L, _CallInit>> : fmt::formatter<T> {
    template <typename FormatContext>
    auto format(const sculk::reflection::dispatcher<T, L, _CallInit>& dispatcher, FormatContext& ctx) const {
        return formatter<T>::format(dispatcher.storage(), ctx);
    }
};
#endif