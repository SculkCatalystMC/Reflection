#pragma once
#ifndef JSONC_NO_EXCEPTION
#define JSONC_NO_EXCEPTION
#endif
#include <sculk/jsonc/jsonc.hpp>

namespace sculk::reflection::jsonc::detail {
template <bool O, bool A>
using basic_jsonc = ::sculk::jsonc::abi_v1_4_0::detail::basic_jsonc<O, A>;
}