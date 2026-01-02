#pragma once
#include <concepts>

namespace jsonc_reflection {

template <typename T, std::default_initializable Listener, bool CallInit = false>
class Dispatcher {
    T        mStorage;
    Listener mListener;

public:
    using storage_type  = T;
    using listener_type = Listener;

    void call() { mListener.call(mStorage); }

    template <class... Args>
    Dispatcher(Args&&... args) : mStorage(std::forward<Args>(args)...),
                                 mListener() {
        if constexpr (CallInit) { call(); }
    }

    Dispatcher& operator=(T const& other) {
        mStorage = other;
        call();
        return *this;
    }

    Dispatcher& operator=(T&& other) {
        mStorage = std::move(other);
        call();
        return *this;
    }

    operator T const&() const { return mStorage; }

    operator T&() { return mStorage; }

    T const& operator*() const { return mStorage; }

    T& operator*() { return mStorage; }

    T const* operator->() const { return &mStorage; }

    T* operator->() { return &mStorage; }
};

} // namespace jsonc_reflection