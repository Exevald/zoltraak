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

enum class InventoryState
{
	MenuSectionSelection,
	ItemActionSelection,
	ItemSelection,
};

enum class InventoryAction
{
	Use,
	Drop,
};