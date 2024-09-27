#pragma once

#include <unordered_map>
#include <cstddef>

using EntityID = std::size_t;

template<typename Type>
using ComponentMap = std::unordered_map<EntityID, Type>;
