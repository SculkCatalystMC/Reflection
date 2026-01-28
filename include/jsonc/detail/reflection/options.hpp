#pragma once
#include <cstdint>

namespace jsonc::reflection {

struct Options {
    int  indent{4};
    bool ignore_comments{false};
    bool allow_trailing_comma{true};
    bool ensure_ascii{false};
    bool keep_null{false};
    bool always_overwrite{true};
    bool overwrite_when_error{true};
    bool keep_extra_comments{true};
    bool enum_cast_prefer_string{true};
};

} // namespace jsonc::reflection
