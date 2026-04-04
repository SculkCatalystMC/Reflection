// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#ifndef JSONC_NO_EXCEPTION
#define JSONC_NO_EXCEPTION
#endif
#include <sculk/jsonc/jsonc.hpp>

namespace sculk::reflection::jsonc::detail {
template <bool O, bool A>
using basic_jsonc = ::sculk::jsonc::abi_v1_4_0::detail::basic_jsonc<O, A>;
}