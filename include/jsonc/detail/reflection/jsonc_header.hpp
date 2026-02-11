#pragma once
#ifndef JSONC_NO_EXCEPTION
#define JSONC_NO_EXCEPTION
#endif
#include <jsonc/jsonc.hpp>

namespace jsonc::reflection::detail {
template <bool O, bool A>
using basic_jsonc = abi_v1_3_0::detail::basic_jsonc<O, A>;
}