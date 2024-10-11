#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

struct WindowSettings
{
	std::string windowTitle = "Ellipse";
	unsigned int windowWidth = 800;
	unsigned int windowHeight = 600;
	int pointCount = 360;
};

int main()
{
	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;
	WindowSettings windowSettings;
	sf::RenderWindow window(
			sf::VideoMode({ windowSettings.windowWidth, windowSettings.windowHeight }),
			windowSettings.windowTitle,
			sf::Style::Default, contextSettings);

	sf::ConvexShape shape;
	shape.setFillColor(sf::Color::White);
	shape.setPointCount(windowSettings.pointCount);
	for (int pointN = 0; pointN < windowSettings.pointCount; pointN++)
	{
		float angle = float(2 * M_PI * pointN) / float(windowSettings.pointCount);
		float radius = 200 * std::sin(6 * angle);
		sf::Vector2f point = {
				radius * std::sin(angle),
				radius * std::cos(angle)
		};
		shape.setPoint(pointN, point);
	}
	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}
		}

		const sf::Vector2f center = { float(windowSettings.windowWidth) / 2, float(windowSettings.windowHeight) / 2 };
		shape.setPosition(center + sf::Vector2f(
				center.x * std::sin(clock.getElapsedTime().asSeconds() * 2),
				center.y * std::cos(clock.getElapsedTime().asSeconds() * 2)
		));

		window.clear();
		window.draw(shape);
		window.display();
	}

	return EXIT_SUCCESS;
}
