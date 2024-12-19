#pragma once

#include "../../../common/Utils.h"
#include <functional>
#include <memory>
#include <unordered_map>

class CInventoryItemsFactory
{
public:
	std::unique_ptr<InventoryItem> CreateInventoryItem(const ItemType& itemType, int ownerId) const
	{
		auto it = creators.find(itemType);
		if (it != creators.end())
		{
			return it->second(ownerId);
		}
		return nullptr;
	}

	void RegisterItem(const ItemType& itemType, std::function<std::unique_ptr<InventoryItem>(int)> creator)
	{
		creators[itemType] = std::move(creator);
	}

	~CInventoryItemsFactory() = default;

private:
	std::unordered_map<ItemType, std::function<std::unique_ptr<InventoryItem>(int)>> creators;
};
