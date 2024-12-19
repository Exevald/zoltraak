#pragma once

#include "../../common/Utils.h"
#include "actions/Actions.h"

class CInventorySystem
{
public:
	static void Init();
	static void Update();

private:
	static void UseItemFromInventory(int itemIndex, const InventoryAction& actionType);
	static void UseEffect(const InventoryItem& item);
};