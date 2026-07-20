// Copyright © 2026 SculkCatalystMC. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include "sculk/reflection/annotated.hpp"
#include "sculk/reflection/dispatcher.hpp"
#include "sculk/reflection/fixed_number.hpp"
#include "sculk/reflection/fixed_string.hpp"
#include "sculk/reflection/jsonc/detail/deserialization.hpp"
#include "sculk/reflection/jsonc/detail/serialization.hpp"
#include "sculk/reflection/renamed.hpp"
#include "sculk/reflection/utils/file_utils.hpp"

namespace sculk::reflection::jsonc {

template <
    bool IsOrdered     = true,
    bool AllowComments = true,
    typename T,
    concepts::is_key_formatter          F,
    concepts::is_optional_error_handler E = std::nullptr_t>
std::expected<void, std::string>
load_file(T& t, const std::filesystem::path& path, F&& key_formatter, const options& options = {}, E&& error_handler = nullptr) {
    detail::basic_jsonc<IsOrdered, AllowComments> data{};

    std::expected<void, std::string> result{};

    std::optional<std::string> content = file_utils::read_file(path);
    if (content) {
        if (auto value = detail::basic_jsonc<IsOrdered, AllowComments>::parse(
                *content,
                options.allow_trailing_comma,
                options.ignore_comments,
                options.float_keep_precision
            )) {
            data   = *value;
            result = deserialize<IsOrdered, AllowComments>(t, data, std::forward<F>(key_formatter), options);
        } else {
            result = std::unexpected("invalid file format");
        }
    } else {
        result = std::unexpected("file not found");
        if (!options.create_if_missing) { return result; }
    }

    if (options.rewrite_condition == options::policy::always_rewrite || (options.rewrite_condition == options::policy::rewrite_on_error && !result)
        || options.rewrite_condition == options::policy::rewrite_on_format) {
        detail::basic_jsonc<IsOrdered, AllowComments> res = serialize<IsOrdered, AllowComments>(t, std::forward<F>(key_formatter), options);
        if constexpr (AllowComments) {
            if (options.keep_extra_comments && !data.is_null() && !options.ignore_comments) {
                data.move_comments_to_before();
                res.merge_comments(data);
            }
        }

        if constexpr (concepts::is_error_handler<E>) {
            if (content && !content->empty() && !result) { std::invoke(error_handler, *content); }
        }

        auto file = res.dump(options.indent, options.ensure_ascii, options.ignore_comments, options.multi_line_comments_format);
        if (options.rewrite_condition == options::policy::rewrite_on_format && content) {
            if (file != *content) { file_utils::write_file(path, file); }
        } else {
            file_utils::write_file(path, file);
        }
        if (!content) { result = {}; }
    }

    return result;
}

template <bool IsOrdered = true, bool AllowComments = true, typename T, concepts::is_optional_error_handler E = std::nullptr_t>
std::expected<void, std::string> load_file(T& t, const std::filesystem::path& path, const options& options = {}, E&& error_handler = nullptr) {
    return load_file<IsOrdered, AllowComments>(t, path, builtin_key_formatter::default_key_formatter, options, std::forward<E>(error_handler));
}

template <bool IsOrdered = true, bool AllowComments = true, typename T, concepts::is_key_formatter F>
bool save_file(const T& t, const std::filesystem::path& path, F&& key_formatter, const options& options = {}) {
    detail::basic_jsonc<IsOrdered, AllowComments> res = serialize<IsOrdered, AllowComments>(t, std::forward<F>(key_formatter), options);
    if (options.keep_extra_comments && !options.ignore_comments) {
        if (auto content = file_utils::read_file(path)) {
            if (auto data = detail::basic_jsonc<IsOrdered, AllowComments>::parse(
                    *content,
                    options.allow_trailing_comma,
                    options.ignore_comments,
                    options.float_keep_precision
                )) {
                data->move_comments_to_before();
                res.merge_comments(*data);
            };
        }
    }
    return file_utils::write_file(path, res.dump(options.indent, options.ensure_ascii, options.ignore_comments, options.multi_line_comments_format));
}

template <bool IsOrdered = true, bool AllowComments = true, typename T>
bool save_file(const T& t, const std::filesystem::path& path, const options& options = {}) {
    return save_file<IsOrdered, AllowComments>(t, path, builtin_key_formatter::default_key_formatter, options);
}

} // namespace sculk::reflection::jsonc