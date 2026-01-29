#pragma once
#include "jsonc/detail/reflection/concepts.hpp"

namespace jsonc::reflection {

template <typename T, concepts::is_dispatcher_listener<T> L, bool _CallInit = false>
class Dispatcher {
public:
    using storage_type  = T;
    using listener_type = L;

    void call() noexcept { listener_.call(storage_); }

    template <class... Args>
    Dispatcher(Args&&... args) noexcept : storage_(std::forward<Args>(args)...),
                                          listener_() {
        if constexpr (_CallInit) { call(); }
    }

    Dispatcher& operator=(T const& other) noexcept {
        storage_ = other;
        call();
        return *this;
    }

    Dispatcher& operator=(T&& other) noexcept {
        storage_ = std::move(other);
        call();
        return *this;
    }

    operator T const&() const noexcept { return storage_; }

    operator T&() noexcept { return storage_; }

    T const& operator*() const noexcept { return storage_; }

    T& operator*() noexcept { return storage_; }

    T const* operator->() const noexcept { return &storage_; }

    T* operator->() noexcept { return &storage_; }

private:
    T storage_;
    L listener_;
};

} // namespace jsonc::reflection