#include "CEntityManager.h"
#include "CGameController.h"
#include "assets_storage/CTextureStorage.h"
#include "common/Utils.h"
#include "common/level_generator/CLevelGenerator.h"
#include "inventory/items_factory/CInventoryItemsFactory.h"
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
			CEventSystem::HandleMouseClick(gameController, mousePos);
		}

		if (event.type == sf::Event::KeyPressed)
		{
			movementKeyPressed = true;
			CEventSystem::HandleKeyPress(gameController, event.key.code);
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
	CInventoryItemsFactory factory;
	auto& entityManager = CEntityManager::GetInstance();

	EntityId camera = entityManager.CreateEntity();
	entityManager.AddComponent<CameraComponent>(camera, 0, 0);

	auto hero1Sprites = CTextureStorage::GetTexture("hero1_sprites.png");
	auto hero2Sprites = CTextureStorage::GetTexture("hero2_sprites.png");
	auto menuSoul = CTextureStorage::GetTexture("menu_soul.png");
	auto utilsWindows = CTextureStorage::GetTexture("utils_windows.png");

	auto mapTexture = CTextureStorage::GetTexture("map_fieldOfHopesAndDreams.png");
	auto level = CLevelGenerator::GenerateLevel("level1.txt");

	CGameController::SetGameState(CurrentState::Inventory);

	factory.RegisterItem(ItemType::HealingPotion, [](int ownerId) {
		return std::make_unique<InventoryItem>("Heal potion", ItemType::HealingPotion, ownerId);
	});
	factory.RegisterItem(ItemType::DamagePotion, [](int ownerId) {
		return std::make_unique<InventoryItem>("Damage potion", ItemType::DamagePotion, ownerId);
	});
	factory.RegisterItem(ItemType::DefensePotion, [](int ownerId) {
		return std::make_unique<InventoryItem>("Defence potion", ItemType::DefensePotion, ownerId);
	});
	factory.RegisterItem(ItemType::ManaPotion, [](int ownerId) {
		return std::make_unique<InventoryItem>("Mana potion", ItemType::ManaPotion, ownerId);
	});

	EntityId hero1 = entityManager.CreateEntity();
	entityManager.AddComponent<SelectionComponent>(hero1);
	entityManager.AddComponent<NameComponent>(hero1, "Kris");
	entityManager.AddComponent<ColorThemeComponent>(hero1, sf::Color(27, 255, 254));
	entityManager.AddComponent<PositionComponent>(hero1, 1000.0f, 800.0f);
	entityManager.AddComponent<VelocityComponent>(hero1, 0, 0);
	entityManager.AddComponent<CollisionComponent>(hero1, CollisionType::Character);
	entityManager.AddComponent<MassComponent>(hero1, 50);
	entityManager.AddComponent<RotationComponent>(hero1, 0);
	entityManager.AddComponent<HealthComponent>(hero1, 50, 100);
	entityManager.AddComponent<ManaComponent>(hero1, 40, 100);
	entityManager.AddComponent<AvatarComponent>(hero1, "hero1_avatar.png");
	entityManager.AddComponent<ExperienceComponent>(hero1, 0, 100, 1);
	entityManager.AddComponent<AnimationComponent>(hero1, hero1Sprites);
	entityManager.AddComponent<FightTurnComponent>(hero1, false, false);
	entityManager.AddComponent<MoneyComponent>(hero1, 100);

	std::vector<InventoryItem> hero1InventoryItems;
	hero1InventoryItems.push_back(*factory.CreateInventoryItem(ItemType::HealingPotion, hero1));
	hero1InventoryItems.push_back(*factory.CreateInventoryItem(ItemType::HealingPotion, hero1));

	entityManager.AddComponent<InventoryComponent>(hero1, hero1InventoryItems, 6);

	auto* hero1AnimComp = entityManager.GetComponent<AnimationComponent>(hero1);
	hero1AnimComp->AddAnimation("idle", 6, 1563, 6, sf::Vector2i(35, 37), 0.15f);
	hero1AnimComp->AddAnimation("walk_up", 101, 139, 4, sf::Vector2i(18, 37), 0.25f);
	hero1AnimComp->AddAnimation("walk_down", 101, 7, 4, sf::Vector2i(18, 37), 0.25f);
	hero1AnimComp->AddAnimation("walk_left", 101, 51, 4, sf::Vector2i(18, 37), 0.25f);
	hero1AnimComp->AddAnimation("walk_right", 101, 95, 4, sf::Vector2i(18, 37), 0.25f);
	hero1AnimComp->SetAnimation("idle");

	EntityId hero2 = entityManager.CreateEntity();
	entityManager.AddComponent<SelectionComponent>(hero2);
	entityManager.AddComponent<NameComponent>(hero2, "Ralsei");
	entityManager.AddComponent<ColorThemeComponent>(hero2, sf::Color(25, 253, 16));
	entityManager.AddComponent<PositionComponent>(hero2, 1200.0f, 800.0f);
	entityManager.AddComponent<VelocityComponent>(hero2, 0, 0);
	entityManager.AddComponent<CollisionComponent>(hero2, CollisionType::Character);
	entityManager.AddComponent<MassComponent>(hero2, 50);
	entityManager.AddComponent<RotationComponent>(hero2, 0);
	entityManager.AddComponent<HealthComponent>(hero2, 30, 100);
	entityManager.AddComponent<ManaComponent>(hero2, 90, 100);
	entityManager.AddComponent<AvatarComponent>(hero2, "hero2_avatar.png");
	entityManager.AddComponent<ExperienceComponent>(hero2, 80, 100, 1);
	entityManager.AddComponent<AnimationComponent>(hero2, hero2Sprites);
	entityManager.AddComponent<FightTurnComponent>(hero2, false, true);

	std::vector<InventoryItem> hero2InventoryItems;
	hero2InventoryItems.push_back(*factory.CreateInventoryItem(ItemType::ManaPotion, hero2));
	hero2InventoryItems.push_back(*factory.CreateInventoryItem(ItemType::DamagePotion, hero2));

	entityManager.AddComponent<InventoryComponent>(hero2, hero2InventoryItems, 6);

	auto* hero2AnimComp = entityManager.GetComponent<AnimationComponent>(hero2);
	hero2AnimComp->AddAnimation("idle", 5, 505, 5, sf::Vector2i(49, 40), 0.15f);
	hero2AnimComp->AddAnimation("walk_up", 0, 37, 4, sf::Vector2i(18, 37), 0.2f);
	hero2AnimComp->AddAnimation("walk_down", 5, 104, 4, sf::Vector2i(22, 42), 0.2f);
	hero2AnimComp->AddAnimation("walk_left", 0, 111, 4, sf::Vector2i(18, 37), 0.2f);
	hero2AnimComp->AddAnimation("walk_right", 0, 148, 4, sf::Vector2i(18, 37), 0.2f);
	hero2AnimComp->SetAnimation("idle");

	EntityId box = entityManager.CreateEntity();
	entityManager.AddComponent<PositionComponent>(box, 1300.0f, 900.0f);
	entityManager.AddComponent<VelocityComponent>(box, 0, 0);
	entityManager.AddComponent<CollisionComponent>(box, CollisionType::MovableObject);
	entityManager.AddComponent<MassComponent>(box, 1);
	entityManager.AddComponent<RotationComponent>(box, 0);
	entityManager.AddComponent<ImageComponent>(box, mapTexture, 547, 15, sf::Vector2i(20, 20));

	EntityId mainMenuSoul = entityManager.CreateEntity();
	entityManager.AddComponent<PositionComponent>(mainMenuSoul, window.getSize().x / 2 + 20, window.getSize().y / 2 + 140);
	entityManager.AddComponent<MenuSoulComponent>(mainMenuSoul);
	entityManager.AddComponent<VelocityComponent>(mainMenuSoul, 0, 0);
	entityManager.AddComponent<ImageComponent>(mainMenuSoul, menuSoul, 0, 0, sf::Vector2i(225, 225));

	EntityId fightSoul = entityManager.CreateEntity();
	entityManager.AddComponent<PositionComponent>(fightSoul, 890, 418);
	entityManager.AddComponent<VelocityComponent>(fightSoul, 0, 0);
	entityManager.AddComponent<ImageComponent>(fightSoul, menuSoul, 0, 0, sf::Vector2i(225, 225));
	entityManager.AddComponent<FightSoulComponent>(fightSoul);

	CGameController::InitSystems();
	CGameController::InitGameSettings(level);

	sf::Clock clock;

	while (window.isOpen())
	{
		if (CGameController::GetCurrentGameState() == CurrentState::MainMenu)
		{
			auto playerSoulId = entityManager.GetEntitiesWithComponents<MenuSoulComponent>().front();
			gameController.SetSelectedEntityId(playerSoulId);
		}
		if (CGameController::GetCurrentGameState() == CurrentState::Fight)
		{
			auto fightSoulId = entityManager.GetEntitiesWithComponents<FightSoulComponent>().front();
			gameController.SetSelectedEntityId(fightSoulId);
		}

		HandleEvents(window, gameController);
		CGameController::UpdateDeltaTime();
		if (CGameController::GetCurrentGameState() == CurrentState::Player)
		{
			CGameController::IncreaseElapsedTime();
		}

		window.clear();
		CGameController::Draw(window, level);
		CGameController::Update();
		window.display();
	}

	return EXIT_SUCCESS;
}
