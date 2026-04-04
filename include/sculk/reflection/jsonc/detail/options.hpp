#pragma once
#include <cstdint>
#include <filesystem>
#include <string>

namespace sculk::reflection {

enum class rewrite_policy {
    always = 0,
    format = 1,
    error  = 2,
    never  = 3,
};

struct options {
    int                   indent{4};
    bool                  ignore_comments{false};
    bool                  allow_trailing_comma{true};
    bool                  ensure_ascii{false};
    bool                  keep_null{false};
    bool                  keep_extra_comments{true};
    bool                  enum_cast_prefer_string{true};
    bool                  multi_line_comments_format{true};
    bool                  float_keep_precision{true};
    rewrite_policy        rewrite_policy{};
    bool                  back_up_file_on_error{false};
    std::filesystem::path back_up_file_folder{};
    std::string           back_up_time_suffix{"-%Y%m%d-%H_%M_%S"};
};

} // namespace sculk::reflection
