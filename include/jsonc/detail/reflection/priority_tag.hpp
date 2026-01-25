#pragma once

namespace jsonc::reflection {

template <size_t N>
struct PriorityTag : PriorityTag<N - 1> {};
template <>
struct PriorityTag<0> {};

} // namespace jsonc::reflection