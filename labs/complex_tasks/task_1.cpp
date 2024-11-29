#include <SFML/Graphics.hpp>
#include <cmath>

struct WindowSettings
{
	std::string windowTitle = "Arrow follows mouse";
	unsigned int windowWidth = 800;
	unsigned int windowHeight = 600;
};

float ToDegrees(float radians)
{
	return radians * 180.0f / float(M_PI);
}

void InitArrow(sf::ConvexShape& arrow)
{
	arrow.setPosition({ 400, 300 });
	arrow.setPointCount(7);

	arrow.setPoint(0, { -60, -30 });
	arrow.setPoint(1, { 0, -30 });
	arrow.setPoint(2, { 0, -60 });
	arrow.setPoint(3, { 47, 0 });
	arrow.setPoint(4, { 0, 60 });
	arrow.setPoint(5, { 0, 30 });
	arrow.setPoint(6, { -60, 30 });
	arrow.setOrigin(-6.5f, 0);
	arrow.setFillColor(sf::Color::Blue);
	arrow.setOutlineColor(sf::Color::Black);
	arrow.setOutlineThickness(3);
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
			mousePosition = { float(event.mouseMove.x), float(event.mouseMove.y) };
		}
	}
}

void UpdateArrow(const sf::Vector2f& mousePosition, sf::ConvexShape& arrow, sf::Clock& clock)
{
	const float dt = clock.restart().asSeconds();

	const sf::Vector2f delta = mousePosition - arrow.getPosition();
	float targetAngle = ToDegrees(std::atan2(delta.y, delta.x));
	float currentAngle = arrow.getRotation();

	float deltaAngle = targetAngle - currentAngle;
	if (deltaAngle < -180.0f)
	{
		deltaAngle += 360.0f;
	}
	if (deltaAngle > 180.0f)
	{
		deltaAngle -= 360.0f;
	}

	const float rotationSpeed = 90.0f;
	float maxRotation = rotationSpeed * dt;
	if (std::abs(deltaAngle) < maxRotation)
	{
		arrow.setRotation(targetAngle);
	}
	else
	{
		arrow.rotate((deltaAngle > 0 ? 1 : -1) * maxRotation);
	}

	const float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	const float moveSpeed = 60.0f;
	if (distance > 0.1f)
	{
		const sf::Vector2f direction = delta / distance;
		float moveDistance = std::min(moveSpeed * dt, distance);
		arrow.move(direction * moveDistance);
	}
}

void DrawFrame(sf::RenderWindow& window, sf::ConvexShape& arrow)
{
	window.clear(sf::Color::White);
	window.draw(arrow);
	window.display();
}

int main()
{
	WindowSettings windowSettings;
	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;

	sf::RenderWindow window(
		sf::VideoMode({ windowSettings.windowWidth, windowSettings.windowHeight }),
		windowSettings.windowTitle, sf::Style::Default, contextSettings);

	sf::ConvexShape arrow;
	sf::Vector2f mousePosition;
	InitArrow(arrow);

	sf::Clock clock;

	while (window.isOpen())
	{
		PollEvents(window, mousePosition);
		UpdateArrow(mousePosition, arrow, clock);
		DrawFrame(window, arrow);
	}

	return 0;
}
