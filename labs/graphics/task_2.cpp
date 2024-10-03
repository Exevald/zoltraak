#include <SFML/Graphics.hpp>
#include <iostream>

enum class LetterType
{
	Letter_K,
	Letter_O
};

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

void DrawLetter(sf::RenderWindow& window, const sf::Vector2<float>& coords, const LetterType& letterType,
		const sf::Color& color)
{
	switch (letterType)
	{
	case LetterType::Letter_K:
	{
		sf::RectangleShape verticalRect = CreateRect({ coords.x, coords.y }, { 20, 250 }, color);
		sf::RectangleShape upperRect = CreateRect({ coords.x + 20, coords.y + 120 }, { 20, 140 }, color);
		sf::RectangleShape downRect = CreateRect({ coords.x, coords.y + 120 }, { 20, 170 }, color);
		upperRect.setRotation(-140);
		downRect.setRotation(-40);

		window.draw(verticalRect);
		window.draw(upperRect);
		window.draw(downRect);
		break;
	}
	case LetterType::Letter_O:
	{
		sf::RectangleShape leftRect = CreateRect({ coords.x, coords.y }, { 20, 250 }, color);
		sf::RectangleShape downRect = CreateRect({ coords.x, coords.y + 230 }, { 120, 20 }, color);
		sf::RectangleShape rightRect = CreateRect({ coords.x + 100, coords.y}, { 20, 250 }, color);
		sf::RectangleShape upperRect = CreateRect({ coords.x, coords.y}, { 120, 20 }, color);

		window.draw(leftRect);
		window.draw(downRect);
		window.draw(rightRect);
		window.draw(upperRect);

		break;
	}
	default:
		throw std::invalid_argument("Invalid letter type");
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Task 1");


	while (window.isOpen())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}
		window.clear();

		try
		{
			DrawLetter(window, { 100, 100 }, LetterType::Letter_K, sf::Color::Blue);
			DrawLetter(window, { 300, 100 }, LetterType::Letter_K, sf::Color::Blue);
			DrawLetter(window, { 500, 100 }, LetterType::Letter_O, sf::Color::Blue);

		}
		catch (const std::exception& exception)
		{
			std::cout << exception.what() << std::endl;
			return EXIT_FAILURE;
		}

		window.display();
	}

	return EXIT_SUCCESS;
}