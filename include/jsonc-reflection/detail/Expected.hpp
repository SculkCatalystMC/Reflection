#pragma once
#include <exception>
#include <expected>
#include <string>

namespace jsonc_reflection {

template <class T = void>
using Expected   = std::expected<T, std::string>;
using Unexpected = std::unexpected<std::string>;

Unexpected makeStringError(std::string_view str) noexcept { return std::unexpected(std::string(str)); }

template <typename... Args>
inline Unexpected makeStringError(std::format_string<Args...> fmt, Args&&... args) {
    return makeStringError(std::vformat(fmt.get(), std::make_format_args(args...)));
}

Unexpected makeErrorCodeError(std::error_code ec) noexcept { return std::unexpected(ec.message()); }

Unexpected makeErrorCodeError(std::errc ec) noexcept { return std::unexpected(make_error_code(ec).message()); }

Unexpected makeExceptionError(std::exception_ptr const& exc = std::current_exception()) noexcept try {
    std::rethrow_exception(exc);
} catch (std::exception const& e) { return std::unexpected(std::string(e.what())); } catch (...) {
    return std::unexpected(std::string("unknown exception"));
}


} // namespace jsonc_reflection