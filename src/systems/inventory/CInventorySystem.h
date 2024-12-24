#pragma once

#include "../../common/Utils.h"
#include "actions/Actions.h"
#include <optional>
#include <vector>

class CInventorySystem
{
public:
	static void Init();
	static void Update();

private:
	static void UseItemFromInventory(int itemIndex, const InventoryAction& actionType);
	static void UseEffect(const InventoryItem& item);
	static void EquipItem(int ownerId, int equippedItemNumber, const ItemType& itemType);

	template <typename T>
	static std::optional<T> GetFilteredItem(const std::vector<T>& items, const T& activeItem, int index);
};