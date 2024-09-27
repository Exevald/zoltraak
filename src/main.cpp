#include <SFML/Graphics.hpp>
#include <iostream>
#include "Entity.h"
#include "CGameController.h"

void handleEvents(sf::RenderWindow& window, CGameController& gameController)
{
	sf::Event event{};
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}

		if (event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
			gameController.handleClick(mousePos);
		}
	}
}

void updateSelectedEntity(CGameController& gameController)
{
	Entity* selectedEntity = gameController.GetSelectedEntity();
	if (selectedEntity)
	{
		auto& selectedPosition = selectedEntity->components.positions[selectedEntity->id];

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			selectedPosition.y -= VELOCITY;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			selectedPosition.y += VELOCITY;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			selectedPosition.x -= VELOCITY;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			selectedPosition.x += VELOCITY;
		}
	}
}


int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Zoltraak");
	CGameController gameController(window);

	Entity square(1, 400, 300, 0, 0);
	square.components.shapes[square.id] = { ShapeType::Rectangle, sf::Color::Red, { 50.f, 50.f }, 0 };

	Entity circle(2, 200, 200, 0, 0);
	circle.components.shapes[circle.id] = { ShapeType::Circle, sf::Color::Green, { 0, 0 }, 25.f };

	gameController.AddEntity(&square);
	gameController.AddEntity(&circle);

	while (window.isOpen())
	{
		handleEvents(window, gameController);

		updateSelectedEntity(gameController);

		gameController.Update();
		window.clear();
		gameController.Draw();
		window.display();
	}

	return EXIT_SUCCESS;
}
