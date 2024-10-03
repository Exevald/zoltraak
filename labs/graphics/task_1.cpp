#include <SFML/Graphics.hpp>

sf::CircleShape CreateCircle(const sf::Vector2<float>& coords, const float& radius, const sf::Color& color)
{
	sf::CircleShape circle;
	circle.setPosition(coords);
	circle.setRadius(radius);
	circle.setFillColor(color);

	return circle;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Task 1");

	sf::RectangleShape trafficLightBase;
	trafficLightBase.setPosition(
			{ static_cast<float>(window.getSize().x / 2.25), static_cast<float>(window.getSize().y / 4.0) }
	);
	trafficLightBase.setSize({ 100, 310 });
	trafficLightBase.setFillColor(sf::Color(0x373737FF));

	sf::CircleShape redLight = CreateCircle({ 360, 160 }, 45, sf::Color::Red);
	sf::CircleShape yellowLight = CreateCircle({ 360, 260 }, 45, sf::Color::Yellow);
	sf::CircleShape greenLight = CreateCircle({ 360, 360 }, 45, sf::Color::Green);

	while (window.isOpen())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}
		window.clear();

		window.draw(trafficLightBase);
		window.draw(redLight);
		window.draw(yellowLight);
		window.draw(greenLight);

		window.display();
	}

	return EXIT_SUCCESS;
}