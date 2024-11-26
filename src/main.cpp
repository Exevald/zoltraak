#include "CEntityManager.h"
#include "CGameController.h"
#include "common/Utils.h"
#include "systems/event/CEventSystem.h"
#include <SFML/Graphics.hpp>

void HandleEvents(sf::RenderWindow& window, CGameController& gameController)
{
	sf::Event event{};
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
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
			CEventSystem::handleKeyPress(gameController, event.key.code);
		}
	}
}

int main()
{
	WindowSettings windowSettings;
	sf::RenderWindow window(
		sf::VideoMode(windowSettings.windowWidth, windowSettings.windowHeight),
		windowSettings.windowTitle);
	CGameController gameController;
	auto& entityManager = CEntityManager::GetInstance();

	EntityId camera = entityManager.CreateEntity();
	entityManager.AddComponent<CameraComponent>(camera, 0, 0);

	EntityId hero1 = entityManager.CreateEntity();
	entityManager.AddComponent<SelectionComponent>(hero1);
	entityManager.AddComponent<NameComponent>(hero1, "Kris");
	entityManager.AddComponent<ColorThemeComponent>(hero1, sf::Color(27, 255, 254));
	entityManager.AddComponent<PositionComponent>(hero1, 1000.0f, 800.0f);
	entityManager.AddComponent<VelocityComponent>(hero1, DefaultVelocity, DefaultVelocity);
	entityManager.AddComponent<ShapeComponent>(hero1, ShapeType::Rectangle, sf::Color::Red, sf::Vector2f(50, 50), 0.0f);
	entityManager.AddComponent<CollisionComponent>(hero1, CollisionType::Character);
	entityManager.AddComponent<MassComponent>(hero1, 50);
	entityManager.AddComponent<RotationComponent>(hero1, 0);
	entityManager.AddComponent<HealthComponent>(hero1, 80, 100);
	entityManager.AddComponent<ManaComponent>(hero1, 40, 100);
	entityManager.AddComponent<AvatarComponent>(hero1, "hero1_avatar.png");

	EntityId hero2 = entityManager.CreateEntity();
	entityManager.AddComponent<SelectionComponent>(hero2);
	entityManager.AddComponent<NameComponent>(hero2, "Raisel");
	entityManager.AddComponent<ColorThemeComponent>(hero2, sf::Color(25, 253, 16));
	entityManager.AddComponent<PositionComponent>(hero2, 1200.0f, 800.0f);
	entityManager.AddComponent<VelocityComponent>(hero2, DefaultVelocity, DefaultVelocity);
	entityManager.AddComponent<ShapeComponent>(hero2, ShapeType::Rectangle, sf::Color::Green, sf::Vector2f(50, 50), 0.0f);
	entityManager.AddComponent<CollisionComponent>(hero2, CollisionType::Character);
	entityManager.AddComponent<MassComponent>(hero2, 50);
	entityManager.AddComponent<RotationComponent>(hero2, 0);
	entityManager.AddComponent<HealthComponent>(hero2, 50, 100);
	entityManager.AddComponent<ManaComponent>(hero2, 90, 100);
	entityManager.AddComponent<AvatarComponent>(hero2, "hero2_avatar.png");

	EntityId box = entityManager.CreateEntity();
	entityManager.AddComponent<PositionComponent>(box, 1300.0f, 900.0f);
	entityManager.AddComponent<VelocityComponent>(box, 0, 0);
	entityManager.AddComponent<ShapeComponent>(box, ShapeType::Rectangle, sf::Color::Magenta, sf::Vector2f(50, 50), 0.0f);
	entityManager.AddComponent<CollisionComponent>(box, CollisionType::MovableObject);
	entityManager.AddComponent<MassComponent>(box, 1);
	entityManager.AddComponent<RotationComponent>(box, 0);

	CGameController::InitSystems();

	while (window.isOpen())
	{
		HandleEvents(window, gameController);

		window.clear();
		CGameController::Draw(window);
		window.display();
	}

	return EXIT_SUCCESS;
}
