#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

struct WindowSettings
{
	std::string windowTitle = "Pointer follows mouse";
	unsigned int windowWidth = 800;
	unsigned int windowHeight = 600;
};

float NormalizeAngle(float angle)
{
	while (angle < 0)
	{
		angle += 360;
	}
	while (angle >= 360)
	{
		angle -= 360;
	}
	return angle;
}

void InitPointer(sf::ConvexShape& pointer)
{
	pointer.setPointCount(3);
	pointer.setPoint(0, { 40, 0 });
	pointer.setPoint(1, { -20, -20 });
	pointer.setPoint(2, { -20, 20 });
	pointer.setOrigin({-20, 0});
	pointer.setPosition({ 400, 300 });
	pointer.setFillColor(sf::Color(0xFF, 0x80, 0x00));
}

float ConvertRadiansToDegrees(float radians)
{
	return float(radians * 180.0f / M_PI);
}

void OnMouseMove(const sf::Event::MouseMoveEvent& event, sf::Vector2f& mousePosition)
{
	mousePosition = { static_cast<float>(event.x), static_cast<float>(event.y) };
}

void PollEvents(sf::RenderWindow& window, sf::Vector2f& mousePosition)
{
	sf::Event event{};
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			OnMouseMove(event.mouseMove, mousePosition);
		}
	}
}

void UpdateFrame(const sf::Vector2f& mousePosition, sf::ConvexShape& pointer, sf::Clock& clock)
{
	const sf::Vector2f deltaPos = mousePosition - pointer.getPosition();
	float targetAngle = ConvertRadiansToDegrees(float(atan2(deltaPos.y, deltaPos.x)));
	const float deltaTime = clock.restart().asSeconds();
	const float prevRotation = NormalizeAngle(pointer.getRotation());
	const float pointerSpeed = 15.0f;

	targetAngle = NormalizeAngle(targetAngle);
	float angleDifference = targetAngle - prevRotation;
	if (angleDifference > 180)
	{
		angleDifference -= 360;
	}
	if (angleDifference < -180)
	{
		angleDifference += 360;
	}

	const float maxRotation = pointerSpeed * deltaTime;
	const float rotation = std::clamp(angleDifference, -maxRotation, maxRotation);
	pointer.rotate(rotation);
}

void RedrawFrame(sf::RenderWindow& window, sf::ConvexShape& pointer)
{
	window.clear();
	window.draw(pointer);
	window.display();
}

int main()
{
	WindowSettings windowSettings;

	sf::Clock clock;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode({ windowSettings.windowWidth, windowSettings.windowHeight }),
			windowSettings.windowTitle,
			sf::Style::Default, settings);
	sf::ConvexShape pointer;
	sf::Vector2f mousePosition = { 400, 300 };

	InitPointer(pointer);
	while (window.isOpen())
	{
		PollEvents(window, mousePosition);
		UpdateFrame(mousePosition, pointer, clock);
		RedrawFrame(window, pointer);
	}

	return EXIT_SUCCESS;
}
