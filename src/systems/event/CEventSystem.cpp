#include "event/CEventSystem.h"
#include "Utils.h"
#include "fight/CFightSystem.h"
#include <iostream>

InventoryState CEventSystem::m_currentInventoryState = InventoryState::MenuSectionSelection;

void CEventSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::InventoryStateChanged, [](const SEvent& event) {
		const auto& inventoryStateChangedEventData = std::get<InventoryStateChangedEventData>(event.data);
		m_currentInventoryState = inventoryStateChangedEventData.changedState;
	});
}

void CEventSystem::HandleMouseClick(CGameController& gameController, const sf::Vector2f& mousePos)
{
	EntityId clickedEntity = -1;
	auto& entityManager = CEntityManager::GetInstance();

	for (auto entityId : entityManager.GetEntitiesWithComponents<PositionComponent, SelectionComponent>())
	{
		auto* animationComp = entityManager.GetComponent<AnimationComponent>(entityId);
		auto* positionComp = entityManager.GetComponent<PositionComponent>(entityId);
		auto* selectionComp = entityManager.GetComponent<SelectionComponent>(entityId);
		auto* imageComp = entityManager.GetComponent<ImageComponent>(entityId);

		if (!positionComp || !selectionComp || (!animationComp && !imageComp))
		{
			continue;
		}

		sf::FloatRect bounds;
		if (animationComp)
		{
			bounds = animationComp->sprite.getGlobalBounds();
		}
		if (imageComp)
		{
			bounds = { positionComp->x, positionComp->y, 22, 22 };
		}

		if (bounds.contains(mousePos))
		{
			clickedEntity = entityId;
			selectionComp->isSelected = true;
		}
		else
		{
			selectionComp->isSelected = false;
		}
	}

	EntitySelectedEventData selectedData{};
	selectedData.id = clickedEntity;

	SEvent entitySelectedEvent;
	entitySelectedEvent.type = EventType::EntitySelected;
	entitySelectedEvent.data = selectedData;

	CEventDispatcher::GetInstance().Dispatch(entitySelectedEvent);

	gameController.SetSelectedEntityId(clickedEntity);
}

void CEventSystem::HandleKeyPress(CGameController& gameController, sf::Keyboard::Key key)
{
	auto& entityManager = CEntityManager::GetInstance();
	EntityId selectedEntityId = gameController.GetSelectedEntityId();
	auto* velocityComp = entityManager.GetComponent<VelocityComponent>(selectedEntityId);
	auto* animationComp = entityManager.GetComponent<AnimationComponent>(selectedEntityId);

	auto currentMainMenuOption = CGameController::GetCurrentMainMenuOption();
	auto currentPauseMenuOption = CGameController::GetCurrentPauseMenuOption();
	auto currentGameState = CGameController::GetCurrentGameState();
	auto currentGameSaveNumber = CGameController::GetCurrentGameSaveNumber();
	auto currentFightPhase = CGameController::GetCurrentFightPhase();

	if (currentGameState == CurrentState::Fight && currentFightPhase == FightPhase::CharactersTurn)
	{
		switch (key)
		{
		case sf::Keyboard::A: {
			auto newFightAction = CGameController::GetCurrentFightActionNumber() - 1;
			if (newFightAction < 0)
			{
				newFightAction = 0;
			}
			CGameController::SetCurrentFightActionNumber(newFightAction);
			break;
		}
		case sf::Keyboard::D: {
			auto newFightAction = CGameController::GetCurrentFightActionNumber() + 1;
			if (newFightAction > 4)
			{
				newFightAction = 4;
			}
			CGameController::SetCurrentFightActionNumber(newFightAction);
			break;
		}
		case sf::Keyboard::Enter: {
			FightAction selectedFightAction;
			switch (CGameController::GetCurrentFightActionNumber())
			{
			case 0: {
				selectedFightAction = FightAction::Attack;
				break;
			}
			case 1: {
				selectedFightAction = FightAction::Act;
				break;
			}
			case 2: {
				selectedFightAction = FightAction::Inventory;
				break;
			}
			case 3: {
				selectedFightAction = FightAction::Spare;
				break;
			}
			case 4: {
				selectedFightAction = FightAction::Magic;
				break;
			}
			default:
				break;
			}

			SEvent fightActionSelectedEvent;
			FightActionSelectedEventData eventData{};

			eventData.id = selectedEntityId;
			eventData.selectedAction = selectedFightAction;
			fightActionSelectedEvent.type = EventType::FightActionSelected;
			fightActionSelectedEvent.data = eventData;

			CEventDispatcher::GetInstance().Dispatch(fightActionSelectedEvent);
			break;
		}
		default:
			break;
		}
	}

	if (currentGameState == CurrentState::Vendor)
	{
		switch (key)
		{
		case sf::Keyboard::W: {
			if (CGameController::GetCurrentVendorState() == VendorState::None)
			{
				int newCurrentVendorAction = CGameController::GetCurrentVendorActionNumber() - 1;
				if (newCurrentVendorAction < 0)
				{
					newCurrentVendorAction = 0;
				}
				CGameController::SetCurrentVendorActionNumber(newCurrentVendorAction);
			}
			if (CGameController::GetCurrentVendorState() == VendorState::Buy)
			{
				int newCurrentVendorItemToBuyNumber = CGameController::GetCurrentVendorItemToBuyNumber() - 1;
				if (newCurrentVendorItemToBuyNumber < 0)
				{
					newCurrentVendorItemToBuyNumber = 0;
				}
				CGameController::SetCurrentVendorItemToBuyNumber(newCurrentVendorItemToBuyNumber);
			}
			if (CGameController::GetCurrentVendorState() == VendorState::Sell)
			{
				int newCurrentHeroItemToSellNumber = CGameController::GetCurrentHeroItemToSellNumber() - 1;
				if (newCurrentHeroItemToSellNumber < 0)
				{
					newCurrentHeroItemToSellNumber = 0;
				}
				CGameController::SetCurrentHeroItemToSellNumber(newCurrentHeroItemToSellNumber);
			}
			break;
		}
		case sf::Keyboard::S: {
			if (CGameController::GetCurrentVendorState() == VendorState::None)
			{
				int newCurrentVendorAction = CGameController::GetCurrentVendorActionNumber() + 1;
				if (newCurrentVendorAction > 3)
				{
					newCurrentVendorAction = 3;
				}
				CGameController::SetCurrentVendorActionNumber(newCurrentVendorAction);
			}
			if (CGameController::GetCurrentVendorState() == VendorState::Buy)
			{
				int newCurrentVendorItemToBuyNumber = CGameController::GetCurrentVendorItemToBuyNumber() + 1;
				if (newCurrentVendorItemToBuyNumber > 5)
				{
					newCurrentVendorItemToBuyNumber = 5;
				}
				CGameController::SetCurrentVendorItemToBuyNumber(newCurrentVendorItemToBuyNumber);
			}
			if (CGameController::GetCurrentVendorState() == VendorState::Sell)
			{
				int newCurrentHeroItemToSellNumber = CGameController::GetCurrentHeroItemToSellNumber() + 1;
				if (newCurrentHeroItemToSellNumber > 5)
				{
					newCurrentHeroItemToSellNumber = 5;
				}
				CGameController::SetCurrentHeroItemToSellNumber(newCurrentHeroItemToSellNumber);
			}
			break;
		}
		case sf::Keyboard::Enter: {
			if (CGameController::GetCurrentVendorState() == VendorState::Buy)
			{
				SEvent vendorItemBoughtEvent;
				VendorItemBoughtEventData eventData{};

				eventData.itemIndex = CGameController::GetCurrentVendorItemToBuyNumber();
				vendorItemBoughtEvent.type = EventType::VendorItemBought;
				vendorItemBoughtEvent.data = eventData;

				CEventDispatcher::GetInstance().Dispatch(vendorItemBoughtEvent);
			}
			if (CGameController::GetCurrentVendorState() == VendorState::Sell)
			{
				SEvent heroItemSoldEvent;
				HeroItemSoldEventData eventData{};

				eventData.itemIndex = CGameController::GetCurrentHeroItemToSellNumber();
				heroItemSoldEvent.type = EventType::HeroItemSold;
				heroItemSoldEvent.data = eventData;

				CEventDispatcher::GetInstance().Dispatch(heroItemSoldEvent);
			}
			if (CGameController::GetCurrentVendorActionNumber() == 3)
			{
				CGameController::SetGameState(CurrentState::Player);
			}
			if (CGameController::GetCurrentVendorActionNumber() == 0 || CGameController::GetCurrentVendorActionNumber() == 1)
			{
				VendorState newState;
				if (CGameController::GetCurrentVendorActionNumber() == 0)
				{
					newState = VendorState::Buy;
				}
				if (CGameController::GetCurrentVendorActionNumber() == 1)
				{
					newState = VendorState::Sell;
				}
				CGameController::SetCurrentVendorState(newState);
			}
			break;
		}
		case sf::Keyboard::C: {
			CGameController::SetCurrentVendorState(VendorState::None);
			break;
		}
		default:
			break;
		}
	}

	if (currentGameState == CurrentState::Inventory)
	{
		switch (key)
		{
		case sf::Keyboard::Up: {
			if (m_currentInventoryState == InventoryState::EquipmentTypeSelection)
			{
				CGameController::SetCurrentInventoryEquipmentType(EquipmentType::Weapon);
			}
			if (m_currentInventoryState == InventoryState::EquipmentItemSelection)
			{
				int newEquipmentItemNumber = CGameController::GetCurrentEquipmentItemNumber() - 1;
				if (newEquipmentItemNumber < 0)
				{
					newEquipmentItemNumber = 0;
				}
				CGameController::SetCurrentInventoryEquipmentItemNumber(newEquipmentItemNumber);
			}
			break;
		}
		case sf::Keyboard::Down: {
			if (m_currentInventoryState == InventoryState::EquipmentTypeSelection)
			{
				CGameController::SetCurrentInventoryEquipmentType(EquipmentType::Shield);
			}
			if (m_currentInventoryState == InventoryState::EquipmentItemSelection)
			{
				int newEquipmentItemNumber = CGameController::GetCurrentEquipmentItemNumber() + 1;
				if (newEquipmentItemNumber > 6)
				{
					newEquipmentItemNumber = 6;
				}
				CGameController::SetCurrentInventoryEquipmentItemNumber(newEquipmentItemNumber);
			}
			break;
		}
		case sf::Keyboard::W: {
			if (m_currentInventoryState != InventoryState::ItemSelection)
			{
				auto newInventoryMenuPosition = CGameController::GetCurrentInventoryMenuPosition() - 1;
				if (newInventoryMenuPosition < 0)
				{
					newInventoryMenuPosition = 0;
				}

				InventoryState state;
				switch (newInventoryMenuPosition)
				{
				case 0: {
					state = InventoryState::MenuSectionSelection;
					break;
				}
				case 1: {
					state = InventoryState::ItemActionSelection;
					break;
				}
				default: {
					break;
				}
				}
				SEvent inventoryStateChangedEvent;
				InventoryStateChangedEventData eventData{};

				eventData.changedState = state;
				inventoryStateChangedEvent.type = EventType::InventoryStateChanged;
				inventoryStateChangedEvent.data = eventData;

				CEventDispatcher::GetInstance().Dispatch(inventoryStateChangedEvent);

				CGameController::SetCurrentInventoryMenuPosition(newInventoryMenuPosition);
			}
			if (m_currentInventoryState == InventoryState::ItemSelection)
			{
				auto newSelectedInventoryItemNumber = CGameController::GetSelectedInventoryItemNumber() - 1;
				if (newSelectedInventoryItemNumber < 0)
				{
					newSelectedInventoryItemNumber = 0;
				}
				CGameController::SetSelectedInventoryItemNumber(newSelectedInventoryItemNumber);
			}

			break;
		}
		case sf::Keyboard::S: {
			if (m_currentInventoryState != InventoryState::ItemSelection)
			{
				auto newInventoryMenuPosition = CGameController::GetCurrentInventoryMenuPosition() + 1;
				if (newInventoryMenuPosition > 1)
				{
					newInventoryMenuPosition = 1;
				}

				InventoryState state;
				switch (newInventoryMenuPosition)
				{
				case 0: {
					state = InventoryState::MenuSectionSelection;
					break;
				}
				case 1: {
					state = InventoryState::ItemActionSelection;
					break;
				}
				default: {
					break;
				}
				}
				SEvent inventoryStateChangedEvent;
				InventoryStateChangedEventData eventData{};

				eventData.changedState = state;
				inventoryStateChangedEvent.type = EventType::InventoryStateChanged;
				inventoryStateChangedEvent.data = eventData;

				CEventDispatcher::GetInstance().Dispatch(inventoryStateChangedEvent);
				CGameController::SetCurrentInventoryMenuPosition(newInventoryMenuPosition);
			}
			if (m_currentInventoryState == InventoryState::ItemSelection)
			{
				unsigned long inventorySize = 0;
				for (const auto& [id, items] : CGameController::GetAllHeroesInventory())
				{
					inventorySize += items.size();
				}
				auto newSelectedInventoryItemNumber = CGameController::GetSelectedInventoryItemNumber() + 1;
				if (newSelectedInventoryItemNumber > int(inventorySize) - 1)
				{
					newSelectedInventoryItemNumber = int(inventorySize) - 1;
				}
				CGameController::SetSelectedInventoryItemNumber(newSelectedInventoryItemNumber);
			}
			break;
		}
		case sf::Keyboard::A: {
			if (CGameController::GetCurrentInventoryMenuPosition() == 0)
			{
				if (CGameController::GetCurrentInventorySectionNumber() == 1)
				{
					SEvent inventoryStateChangedEvent;
					InventoryStateChangedEventData eventData{};

					eventData.changedState = InventoryState::EquipmentTypeSelection;
					inventoryStateChangedEvent.type = EventType::InventoryStateChanged;
					inventoryStateChangedEvent.data = eventData;

					CEventDispatcher::GetInstance().Dispatch(inventoryStateChangedEvent);
				}

				auto newInventoryMenuSectionNumber = CGameController::GetCurrentInventorySectionNumber() - 1;
				if (newInventoryMenuSectionNumber < 0)
				{
					newInventoryMenuSectionNumber = 0;
				}
				CGameController::SetCurrentInventorySectionNumber(newInventoryMenuSectionNumber);
			}
			if (CGameController::GetCurrentInventoryMenuPosition() == 1)
			{
				SEvent inventoryActionChangedEvent;
				InventoryActionChangedEventData eventData{};

				eventData.selectedAction = InventoryAction::Use;
				inventoryActionChangedEvent.type = EventType::InventoryActionChanged;
				inventoryActionChangedEvent.data = eventData;

				CEventDispatcher::GetInstance().Dispatch(inventoryActionChangedEvent);
			}
			if (CGameController::GetCurrentInventoryMenuPosition() == 1 && (CGameController::GetCurrentInventorySectionNumber() == 1 || CGameController::GetCurrentInventorySectionNumber() == 2))
			{
				int newActiveInventoryCharacterNumber = CGameController::GetActiveInventoryCharacterNumber() - 1;
				if (newActiveInventoryCharacterNumber < 1)
				{
					newActiveInventoryCharacterNumber = 1;
				}
				CGameController::SetActiveInventoryCharacterNumber(newActiveInventoryCharacterNumber);
			}
			break;
		}
		case sf::Keyboard::D: {
			if (CGameController::GetCurrentInventoryMenuPosition() == 0)
			{
				if (CGameController::GetCurrentInventorySectionNumber() == 1)
				{
					SEvent inventoryStateChangedEvent;
					InventoryStateChangedEventData eventData{};

					eventData.changedState = InventoryState::EquipmentTypeSelection;
					inventoryStateChangedEvent.type = EventType::InventoryStateChanged;
					inventoryStateChangedEvent.data = eventData;

					CEventDispatcher::GetInstance().Dispatch(inventoryStateChangedEvent);
				}

				auto newInventoryMenuSectionNumber = CGameController::GetCurrentInventorySectionNumber() + 1;
				if (newInventoryMenuSectionNumber > 2)
				{
					newInventoryMenuSectionNumber = 2;
				}
				CGameController::SetCurrentInventorySectionNumber(newInventoryMenuSectionNumber);
			}
			if (CGameController::GetCurrentInventoryMenuPosition() == 1)
			{
				SEvent inventoryActionChangedEvent;
				InventoryActionChangedEventData eventData{};

				eventData.selectedAction = InventoryAction::Destroy;
				inventoryActionChangedEvent.type = EventType::InventoryActionChanged;
				inventoryActionChangedEvent.data = eventData;

				CEventDispatcher::GetInstance().Dispatch(inventoryActionChangedEvent);
			}
			if (CGameController::GetCurrentInventoryMenuPosition() == 1 && (CGameController::GetCurrentInventorySectionNumber() == 1 || CGameController::GetCurrentInventorySectionNumber() == 2))
			{
				int newActiveInventoryCharacterNumber = CGameController::GetActiveInventoryCharacterNumber() + 1;
				if (newActiveInventoryCharacterNumber > 2)
				{
					newActiveInventoryCharacterNumber = 2;
				}
				CGameController::SetActiveInventoryCharacterNumber(newActiveInventoryCharacterNumber);
			}
			break;
		}
		case sf::Keyboard::Enter: {
			if (m_currentInventoryState == InventoryState::EquipmentItemSelection)
			{
				SEvent inventoryItemEquipped;
				InventoryItemEquippedEventData eventData{};
				ItemType type;
				switch (CGameController::GetCurrentEquipmentType())
				{
				case EquipmentType::Weapon: {
					type = ItemType::Weapon;
					break;
				}
				case EquipmentType::Shield: {
					type = ItemType::Shield;
					break;
				}
				}

				eventData.ownerId = CGameController::GetActiveInventoryCharacterNumber();
				eventData.itemNumber = CGameController::GetCurrentEquipmentItemNumber();
				eventData.type = type;
				inventoryItemEquipped.type = EventType::InventoryItemEquipped;
				inventoryItemEquipped.data = eventData;

				CEventDispatcher::GetInstance().Dispatch(inventoryItemEquipped);
			}
			if (m_currentInventoryState == InventoryState::EquipmentTypeSelection)
			{
				SEvent inventoryStateChangedEvent;
				InventoryStateChangedEventData eventData{};

				eventData.changedState = InventoryState::EquipmentItemSelection;
				inventoryStateChangedEvent.type = EventType::InventoryStateChanged;
				inventoryStateChangedEvent.data = eventData;

				CEventDispatcher::GetInstance().Dispatch(inventoryStateChangedEvent);
			}
			if (m_currentInventoryState == InventoryState::ItemSelection)
			{
				auto itemActionEvent = CEventDispatcher::GetInstance().GetLastEvent(EventType::InventoryActionChanged);
				if (!itemActionEvent)
				{
					break;
				}
				const auto itemActionEventData = std::get<InventoryActionChangedEventData>(itemActionEvent->data);

				SEvent inventoryItemUsedEvent;
				InventoryItemUsedEventData eventData{};

				eventData.inventoryItemNumber = CGameController::GetSelectedInventoryItemNumber();
				eventData.usageAction = itemActionEventData.selectedAction;
				inventoryItemUsedEvent.type = EventType::InventoryItemUsed;
				inventoryItemUsedEvent.data = eventData;

				CEventDispatcher::GetInstance().Dispatch(inventoryItemUsedEvent);
			}
			if (CGameController::GetCurrentInventoryMenuPosition() == 1 && m_currentInventoryState != InventoryState::ItemSelection)
			{
				SEvent inventoryStateChangedEvent;
				InventoryStateChangedEventData inventoryStateChangedEventData{};

				inventoryStateChangedEventData.changedState = InventoryState::ItemSelection;
				inventoryStateChangedEvent.type = EventType::InventoryStateChanged;
				inventoryStateChangedEvent.data = inventoryStateChangedEventData;

				SEvent inventoryItemSelectedEvent;
				InventoryItemSelectedEventData itemSelectedEventData{};
				itemSelectedEventData.inventoryItemNumber = 0;
				inventoryItemSelectedEvent.type = EventType::InventoryItemSelected;
				inventoryItemSelectedEvent.data = itemSelectedEventData;

				CEventDispatcher::GetInstance().Dispatch(inventoryStateChangedEvent);
				CEventDispatcher::GetInstance().Dispatch(inventoryItemSelectedEvent);
			}
			break;
		}
		case sf::Keyboard::C: {
			if (m_currentInventoryState == InventoryState::ItemSelection)
			{
				SEvent inventoryStateChangedEvent;
				InventoryStateChangedEventData inventoryStateChangedEventData{};

				inventoryStateChangedEventData.changedState = InventoryState::ItemActionSelection;
				inventoryStateChangedEvent.type = EventType::InventoryStateChanged;
				inventoryStateChangedEvent.data = inventoryStateChangedEventData;

				CEventDispatcher::GetInstance().Dispatch(inventoryStateChangedEvent);
			}
			if (m_currentInventoryState == InventoryState::EquipmentItemSelection)
			{
				SEvent inventoryStateChangedEvent;
				InventoryStateChangedEventData inventoryStateChangedEventData{};

				inventoryStateChangedEventData.changedState = InventoryState::EquipmentTypeSelection;
				inventoryStateChangedEvent.type = EventType::InventoryStateChanged;
				inventoryStateChangedEvent.data = inventoryStateChangedEventData;

				CEventDispatcher::GetInstance().Dispatch(inventoryStateChangedEvent);
			}
			else
			{
				SEvent inventoryStateChangedEvent;
				InventoryStateChangedEventData inventoryStateChangedEventData{};

				inventoryStateChangedEventData.changedState = InventoryState::MenuSectionSelection;
				inventoryStateChangedEvent.type = EventType::InventoryStateChanged;
				inventoryStateChangedEvent.data = inventoryStateChangedEventData;

				CEventDispatcher::GetInstance().Dispatch(inventoryStateChangedEvent);
				CGameController::SetGameState(CurrentState::Player);
			}
			break;
		}
		default:
			break;
		}
	}

	if (currentGameState == CurrentState::PauseMenu)
	{
		switch (key)
		{
		case sf::Keyboard::Up: {
			auto newPauseMenuOption = CGameController::GetCurrentPauseMenuOption() - 1;
			if (newPauseMenuOption < 0)
			{
				newPauseMenuOption = 0;
			}
			CGameController::SetCurrentPauseMenuOption(newPauseMenuOption);
			break;
		}
		case sf::Keyboard::Down: {
			auto newPauseMenuOption = CGameController::GetCurrentPauseMenuOption() + 1;
			if (newPauseMenuOption > 2)
			{
				newPauseMenuOption = 2;
			}
			CGameController::SetCurrentPauseMenuOption(newPauseMenuOption);
			break;
		}
		case sf::Keyboard::Enter: {
			switch (currentPauseMenuOption)
			{
			case 0:
				CGameController::SetGameState(CurrentState::Player);
				break;
			case 1:
				CGameController::SetGameState(CurrentState::Settings);
				break;
			case 2: {
				CGameController::SetGameState(CurrentState::MainMenu);

				auto saveInfo = CGameController::GetSaveInfo(CGameController::GetCurrentGameSaveNumber());
				saveInfo.gameTime += CGameController::GetElapsedTIme();
				CGameController::SaveGameInfo(CGameController::GetCurrentGameSaveNumber(), saveInfo);
				break;
			}
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
	}

	if (currentGameState == CurrentState::Player)
	{
		switch (key)
		{
		case sf::Keyboard::Escape: {
			CGameController::SetGameState(CurrentState::PauseMenu);
			break;
		}
		case sf::Keyboard::I: {
			CGameController::SetGameState(CurrentState::Inventory);
			break;
		}
		default: {
			break;
		}
		}
	}

	if (currentGameState == CurrentState::LevelChoosing)
	{
		switch (key)
		{
		case sf::Keyboard::Up: {
			auto chosenGameSave = currentGameSaveNumber - 1;
			if (chosenGameSave < 0)
			{
				chosenGameSave = 0;
			}
			CGameController::SetCurrentGameSaveNumber(chosenGameSave);
			break;
		}
		case sf::Keyboard::Down: {
			auto chosenGameSave = currentGameSaveNumber + 1;
			if (chosenGameSave > 2)
			{
				chosenGameSave = 2;
			}
			CGameController::SetCurrentGameSaveNumber(chosenGameSave);
			break;
		}
		case sf::Keyboard::Enter: {
			switch (currentGameSaveNumber)
			{
			case 0:
				CGameController::SetGameState(CurrentState::Player);
				break;
			case 1:
				CGameController::SetGameState(CurrentState::Player);
				break;
			case 2:
				CGameController::SetGameState(CurrentState::Player);
				break;
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
	}

	if (selectedEntityId != -1)
	{
		if (!velocityComp)
		{
			return;
		}
		std::string direction;
		switch (key)
		{
		case sf::Keyboard::Enter: {
			if (currentGameState == CurrentState::MainMenu)
			{
				switch (currentMainMenuOption)
				{
				case 0:
					CGameController::SetGameState(CurrentState::LevelChoosing);
					break;
				case 1:
					CGameController::SetGameState(CurrentState::Settings);
					break;
				case 3:
					exit(0);
				default:
					break;
				}
				break;
			}
		}
		case sf::Keyboard::A: {
			if (animationComp)
			{
				animationComp->SetAnimation("walk_left");
			}
			direction = "left";
			velocityComp->vx = -DefaultVelocity;
			break;
		}
		case sf::Keyboard::D: {
			if (animationComp)
			{
				animationComp->SetAnimation("walk_right");
			}
			direction = "right";
			velocityComp->vx = DefaultVelocity;
			break;
		}
		case sf::Keyboard::W: {
			if (animationComp)
			{
				animationComp->SetAnimation("walk_up");
			}
			direction = "up";
			velocityComp->vy = -DefaultVelocity;
			break;
		}
		case sf::Keyboard::S: {
			if (animationComp)
			{
				animationComp->SetAnimation("walk_down");
			}
			direction = "down";
			velocityComp->vy = DefaultVelocity;
			break;
		}
		default:
			break;
		}

		if (!direction.empty())
		{
			EntityMovedEventData entityMovedEventData{};
			entityMovedEventData.id = selectedEntityId;
			entityMovedEventData.direction = direction;

			SEvent entityMovedEvent;
			entityMovedEvent.type = EventType::EntityMoved;
			entityMovedEvent.data = entityMovedEventData;

			CEventDispatcher::GetInstance().Dispatch(entityMovedEvent);
		}
	}
}