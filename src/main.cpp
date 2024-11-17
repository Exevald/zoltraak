#include "CEntityManager.h"
#include "CGameController.h"
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
			sf::Vector2f mousePos(float(event.mouseButton.x), float(event.mouseButton.y));
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
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Zoltraak");
	CGameController gameController;
	auto& entityManager = CEntityManager::GetInstance();

	EntityId squareEntity = entityManager.CreateEntity();
	entityManager.AddComponent<PositionComponent>(squareEntity, 400.0f, 300.0f);
	entityManager.AddComponent<ShapeComponent>(squareEntity, ShapeType::Rectangle, sf::Color::Red, sf::Vector2f(50, 50), 0.0f);

	EntityId circleEntity = entityManager.CreateEntity();
	entityManager.AddComponent<PositionComponent>(circleEntity, 200.0f, 200.0f);
	entityManager.AddComponent<ShapeComponent>(circleEntity, ShapeType::Circle, sf::Color::Green, sf::Vector2f(0, 0), 25.0f);

	CGameController::Init();

	while (window.isOpen())
	{
		HandleEvents(window, gameController);

		window.clear();
		CGameController::Draw(window);
		window.display();
	}

	return EXIT_SUCCESS;
}
