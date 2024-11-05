#pragma once

#include <cstddef>
#include <unordered_map>

using EntityID = std::size_t;

template <typename Type>
using ComponentMap = std::unordered_map<EntityID, Type>;
