#pragma once
#include <cstdint>

namespace jsonc::reflection {

enum class OverwritePolicy : uint8_t {
    Error   = 0,
    Always  = 1,
    Never   = 2,
    Default = Error,
};

struct Options {
    int             indent{4};
    bool            ignore_comments{false};
    bool            allow_trailing_comma{true};
    bool            ensure_ascii{false};
    bool            keep_null{false};
    OverwritePolicy overwrite_policy{OverwritePolicy::Default};
    bool            keep_extra_comments{true};
};

} // namespace jsonc::reflection
