// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <cstdint>
#include <filesystem>
#include <string>

namespace sculk::reflection::jsonc {

struct options {
    enum class policy : std::uint8_t {
        always_rewrite    = 0,
        rewrite_on_format = 1,
        rewrite_on_error  = 2,
        never_rewrite     = 3,
    };

    bool   create_if_missing          = true;
    int    indent                     = 4;
    bool   ignore_comments            = false;
    bool   allow_trailing_comma       = true;
    bool   ensure_ascii               = false;
    bool   keep_null                  = false;
    bool   keep_extra_comments        = true;
    bool   enum_cast_prefer_string    = true;
    bool   enum_cast_case_sensitive   = false;
    bool   multi_line_comments_format = true;
    bool   float_keep_precision       = true;
    bool   check_missing_fields       = false;
    policy rewrite_condition          = policy::always_rewrite;
};

} // namespace sculk::reflection::jsonc
