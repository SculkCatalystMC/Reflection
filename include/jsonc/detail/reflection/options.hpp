#pragma once

namespace jsonc::reflection {

struct Options {
    int  indent          = 4;
    bool ignore_comments = false;
    bool ensure_ascii    = false;
    bool keep_null       = false;
};

} // namespace jsonc::reflection
