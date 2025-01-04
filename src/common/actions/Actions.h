#pragma once

enum class MainMenuAction
{
};

enum class PauseMenuAction
{
};

enum class FightAction
{
	Info,
	Attack,
	Act,
	Inventory,
	Spare,
	Magic
};

enum class EquipmentType
{
	Weapon,
	Shield,
};

enum class InventoryState
{
	MenuSectionSelection,
	ItemActionSelection,
	ItemSelection,
	EquipmentTypeSelection,
	EquipmentItemSelection,
};

enum class InventoryAction
{
	Use,
	Destroy,
};

enum class VendorState
{
	None,
	Buy,
	Sell,
	Talk,
};

enum class SkillsState
{
	ChoosingSkillArea,
	ChoosingSkill,
};