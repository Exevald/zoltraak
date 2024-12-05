#include "CEntityManager.h"
#include "CGameController.h"
#include "common/Utils.h"
#include "common/level_generator/CLevelGenerator.h"
#include "storage/CTextureStorage.h"
#include "systems/event/CEventSystem.h"
#include <SFML/Graphics.hpp>

void HandleEvents(sf::RenderWindow& window, CGameController& gameController)
{
	sf::Event event{};
	bool movementKeyPressed = false;

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}

		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
			CEventSystem::handleMouseClick(gameController, mousePos);
		}

		if (event.type == sf::Event::KeyPressed)
		{
			movementKeyPressed = true;
			CEventSystem::handleKeyPress(gameController, event.key.code);
		}
	}

	if (!movementKeyPressed)
	{
		EntityId selectedEntityId = gameController.GetSelectedEntityId();
		auto& entityManager = CEntityManager::GetInstance();
		auto* velocityComp = entityManager.GetComponent<VelocityComponent>(selectedEntityId);

		if (velocityComp)
		{
			velocityComp->vx = 0;
			velocityComp->vy = 0;
		}
	}
}

int main()
{
	WindowSettings windowSettings;
	sf::RenderWindow window(
		sf::VideoMode(windowSettings.windowWidth, windowSettings.windowHeight),
		windowSettings.windowTitle);
	window.setFramerateLimit(30);
	CGameController gameController;
	auto& entityManager = CEntityManager::GetInstance();

	EntityId camera = entityManager.CreateEntity();
	entityManager.AddComponent<CameraComponent>(camera, 0, 0);

	auto hero1Sprites = CTextureStorage::GetTexture("hero1_sprites.png");
	auto hero2Sprites = CTextureStorage::GetTexture("hero2_sprites.png");
	auto menuSoul = CTextureStorage::GetTexture("menu_soul.png");
	auto utilsWindows = CTextureStorage::GetTexture("utils_windows.png");

	auto mapTexture = CTextureStorage::GetTexture("map_fieldOfHopesAndDreams.png");
	auto level = CLevelGenerator::GenerateLevel("level1.txt");

	CGameController::SetGameState(CurrentState::Fight);

	EntityId hero1 = entityManager.CreateEntity();
	entityManager.AddComponent<SelectionComponent>(hero1);
	entityManager.AddComponent<NameComponent>(hero1, "Kris");
	entityManager.AddComponent<ColorThemeComponent>(hero1, sf::Color(27, 255, 254));
	entityManager.AddComponent<PositionComponent>(hero1, 1000.0f, 800.0f);
	entityManager.AddComponent<VelocityComponent>(hero1, 0, 0);
	entityManager.AddComponent<CollisionComponent>(hero1, CollisionType::Character);
	entityManager.AddComponent<MassComponent>(hero1, 50);
	entityManager.AddComponent<RotationComponent>(hero1, 0);
	entityManager.AddComponent<HealthComponent>(hero1, 80, 100);
	entityManager.AddComponent<ManaComponent>(hero1, 40, 100);
	entityManager.AddComponent<AvatarComponent>(hero1, "hero1_avatar.png");
	entityManager.AddComponent<ExperienceComponent>(hero1, 0, 100, 1);
	entityManager.AddComponent<AnimationComponent>(hero1, hero1Sprites, 101, 6, 4, sf::Vector2i(18, 37), 0.2f);
	entityManager.AddComponent<FightTurnComponent>(hero1, true);

	EntityId hero2 = entityManager.CreateEntity();
	entityManager.AddComponent<SelectionComponent>(hero2);
	entityManager.AddComponent<NameComponent>(hero2, "Raisel");
	entityManager.AddComponent<ColorThemeComponent>(hero2, sf::Color(25, 253, 16));
	entityManager.AddComponent<PositionComponent>(hero2, 1200.0f, 800.0f);
	entityManager.AddComponent<VelocityComponent>(hero2, 0, 0);
	entityManager.AddComponent<CollisionComponent>(hero2, CollisionType::Character);
	entityManager.AddComponent<MassComponent>(hero2, 50);
	entityManager.AddComponent<RotationComponent>(hero2, 0);
	entityManager.AddComponent<HealthComponent>(hero2, 50, 100);
	entityManager.AddComponent<ManaComponent>(hero2, 90, 100);
	entityManager.AddComponent<AvatarComponent>(hero2, "hero2_avatar.png");
	entityManager.AddComponent<ExperienceComponent>(hero2, 80, 100, 1);
	entityManager.AddComponent<AnimationComponent>(hero2, hero2Sprites, 5, 104, 4, sf::Vector2i(22, 42), 0.2f);
	entityManager.AddComponent<FightTurnComponent>(hero2, true);

	EntityId box = entityManager.CreateEntity();
	entityManager.AddComponent<PositionComponent>(box, 1300.0f, 900.0f);
	entityManager.AddComponent<VelocityComponent>(box, 0, 0);
	entityManager.AddComponent<CollisionComponent>(box, CollisionType::MovableObject);
	entityManager.AddComponent<MassComponent>(box, 1);
	entityManager.AddComponent<RotationComponent>(box, 0);
	entityManager.AddComponent<ImageComponent>(box, mapTexture, 547, 15, sf::Vector2i(20, 20));

	EntityId soul = entityManager.CreateEntity();
	entityManager.AddComponent<PositionComponent>(soul, 700, 700);
	entityManager.AddComponent<MenuSoulComponent>(soul);
	entityManager.AddComponent<VelocityComponent>(soul, 0, 0);
	entityManager.AddComponent<ImageComponent>(soul, menuSoul, 0, 0, sf::Vector2i(225, 225));
	entityManager.AddComponent<SelectionComponent>(soul);

	CGameController::InitSystems();
	CGameController::InitGameSettings(level);

	sf::Clock clock;

	while (window.isOpen())
	{
		HandleEvents(window, gameController);
		float deltaTime = clock.restart().asSeconds();
		if (CGameController::GetCurrentGameState() == CurrentState::Player)
		{
			CGameController::IncreaseElapsedTime(deltaTime);
		}

		window.clear();
		CGameController::Draw(window, level);
		CGameController::Update(deltaTime);
		window.display();
	}

	return EXIT_SUCCESS;
}
