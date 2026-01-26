#pragma once
#include "jsonc/detail/reflection/annotated.hpp"
#include "jsonc/detail/reflection/deserialization.hpp"
#include "jsonc/detail/reflection/dispatcher.hpp"
#include "jsonc/detail/reflection/file_utils.hpp"
#include "jsonc/detail/reflection/fixed_number.hpp"
#include "jsonc/detail/reflection/fixed_string.hpp"
#include "jsonc/detail/reflection/renamed.hpp"
#include "jsonc/detail/reflection/serialization.hpp"

namespace jsonc::reflection {

template <typename T>
bool load_config(const T& t, const std::filesystem::path& path, const Options& options = {}) noexcept
    requires(
        traits::is_reflectable_v<std::remove_cvref_t<decltype(t)>>
        || (traits::is_reflectable_v<std::remove_cvref_t<decltype(*t)>> && traits::is_annotated_v<std::remove_cvref_t<decltype(t)>>)
    )
{
    JsoncType res = serialize(t, options);
    return detail::file_utils::write_file(path, res.dump(options.indent, options.ensure_ascii, options.ignore_comments));
}

} // namespace jsonc::reflection