#include "event/CEventSystem.h"
#include "Utils.h"
#include <iostream>

void CEventSystem::handleMouseClick(CGameController& gameController, const sf::Vector2f& mousePos)
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

void CEventSystem::handleKeyPress(CGameController& gameController, sf::Keyboard::Key key)
{
	auto& entityManager = CEntityManager::GetInstance();
	EntityId selectedEntityId = gameController.GetSelectedEntityId();
	auto* velocityComp = entityManager.GetComponent<VelocityComponent>(selectedEntityId);

	auto currentMainMenuOption = CGameController::GetCurrentMainMenuOption();
	auto currentPauseMenuOption = CGameController::GetCurrentPauseMenuOption();
	auto currentGameState = CGameController::GetCurrentGameState();
	auto currentGameSaveNumber = CGameController::GetCurrentGameSaveNumber();

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
		if (key == sf::Keyboard::Escape)
		{
			CGameController::SetGameState(CurrentState::PauseMenu);
			return;
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
			direction = "left";
			velocityComp->vx = -DefaultVelocity;
			break;
		}
		case sf::Keyboard::D: {
			direction = "right";
			velocityComp->vx = DefaultVelocity;
			break;
		}
		case sf::Keyboard::W: {
			direction = "up";
			velocityComp->vy = -DefaultVelocity;
			break;
		}
		case sf::Keyboard::S: {
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
