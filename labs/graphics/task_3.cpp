#include <SFML/Graphics.hpp>

sf::CircleShape CreateCircle(const sf::Vector2<float>& coords, const float& radius, const sf::Color& color)
{
	sf::CircleShape circle;
	circle.setPosition(coords);
	circle.setRadius(radius);
	circle.setFillColor(color);

	return circle;
}

sf::RectangleShape CreateRect(const sf::Vector2<float>& coords, const sf::Vector2<float>& size, const sf::Color& color)
{
	sf::RectangleShape rectangle;
	rectangle.setPosition(coords);
	rectangle.setSize(size);
	rectangle.setFillColor(color);

	return rectangle;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Task 1");

	sf::RectangleShape houseBase = CreateRect({ 200, 300 }, { 400, 250 }, sf::Color::White);
	sf::RectangleShape houseDoor = CreateRect({ 250, 400 }, { 80, 150 }, sf::Color::Magenta);
	sf::RectangleShape houseWindowBase = CreateRect({ 400, 400 }, { 100, 100 }, sf::Color::Black);
	sf::RectangleShape houseWindowStroke = CreateRect({ 410, 410 }, { 80, 80 }, sf::Color::Cyan);
	sf::RectangleShape smokeFlue = CreateRect({500, 200}, {30, 60}, sf::Color::White);

	sf::ConvexShape houseRoof;
	houseRoof.setFillColor(sf::Color::Red);
	houseRoof.setPosition(400, 220);
	houseRoof.setPointCount(4);
	houseRoof.setPoint(0, { -120, 0 });
	houseRoof.setPoint(1, { +120, 0 });
	houseRoof.setPoint(2, { +200, 80 });
	houseRoof.setPoint(3, { -200, 80 });

	sf::CircleShape smoke1 = CreateCircle({500, 170}, 20, sf::Color(0x373737FF));
	sf::CircleShape smoke2 = CreateCircle({515, 150}, 25, sf::Color(0x373737FF));

	while (window.isOpen())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}
		window.clear();
		window.draw(houseBase);
		window.draw(houseDoor);
		window.draw(houseWindowBase);
		window.draw(houseWindowStroke);
		window.draw(houseRoof);
		window.draw(smokeFlue);
		window.draw(smoke1);
		window.draw(smoke2);

		window.display();
	}

	return EXIT_SUCCESS;
}