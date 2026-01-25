#pragma once
#include <concepts>

namespace jsonc::reflection {

template <typename T, std::default_initializable _Listener, bool _CallInit = false>
class Dispatcher {
public:
    using storage_type  = T;
    using listener_type = _Listener;

    void call() { listener_.call(storage_); }

    template <class... Args>
    Dispatcher(Args&&... args) : storage_(std::forward<Args>(args)...),
                                 listener_() {
        if constexpr (_CallInit) { call(); }
    }

    Dispatcher& operator=(T const& other) {
        storage_ = other;
        call();
        return *this;
    }

    Dispatcher& operator=(T&& other) {
        storage_ = std::move(other);
        call();
        return *this;
    }

    operator T const&() const { return storage_; }

    operator T&() { return storage_; }

    T const& operator*() const { return storage_; }

    T& operator*() { return storage_; }

    T const* operator->() const { return &storage_; }

    T* operator->() { return &storage_; }

private:
    T         storage_;
    _Listener listener_;
};

} // namespace jsonc::reflection