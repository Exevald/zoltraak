#include "CGameController.h"
#include "Entity.h"
#include "events/CEventDispatcher.h"
#include "systems/event/CEventSystem.h"
#include <SFML/Graphics.hpp>
#include <iostream>

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
	CGameController gameController(window);

	Entity square(1, 400, 300, 0, 0);
	square.components.shapes[square.id] = { ShapeType::Rectangle, sf::Color::Red, { 50, 50 }, 0 };

	Entity circle(2, 200, 200, 0, 0);
	circle.components.shapes[circle.id] = { ShapeType::Circle, sf::Color::Green, { 0, 0 }, 25 };

	gameController.AddEntity(&square);
	gameController.AddEntity(&circle);

	CGameController::Init();

	while (window.isOpen())
	{
		HandleEvents(window, gameController);

		window.clear();
		gameController.Draw();
		window.display();
	}

	return EXIT_SUCCESS;
}
