#pragma once

namespace jsonc::reflection {

template <size_t N>
struct priority_tag : priority_tag<N - 1> {};
template <>
struct priority_tag<0> {};

} // namespace jsonc::reflection