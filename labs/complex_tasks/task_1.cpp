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
	return float(double(radians) * 180.0 / M_PI);
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
	arrow.setOrigin(-6.5, 0);

	arrow.setFillColor(sf::Color::Blue);
	arrow.setOutlineColor(sf::Color::Black);
	arrow.setOutlineThickness(3);
}

void OnMouseMove(const sf::Event::MouseMoveEvent& event, sf::Vector2f& mousePosition)
{
	mousePosition = { float(event.x), float(event.y) };
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

void UpdateMousePosition(const sf::Vector2f& mousePosition, sf::ConvexShape& arrow, sf::Clock& clock)
{
	const float dt = clock.restart().asSeconds();

	const sf::Vector2f deltaRotation = mousePosition - arrow.getPosition();
	float angle = ToDegrees(float(atan2(deltaRotation.y, deltaRotation.x)));
	if (angle < 0)
	{
		angle += 360;
	}

	float currentRotation = arrow.getRotation();
	float rotationSpeed = 100.0f;
	float rotationDirection = angle - currentRotation;

	if (rotationDirection < -180.0f)
	{
		rotationDirection += 360.0f;
	}
	if (rotationDirection > 180.0f)
	{
		rotationDirection -= 360.0f;
	}

	float rotation = rotationDirection * rotationSpeed * dt / 180.0f;
	arrow.rotate(rotation);

	const sf::Vector2f deltaPosition = mousePosition - arrow.getPosition();
	const float deltaLength = std::sqrt(deltaPosition.x * deltaPosition.x + deltaPosition.y * deltaPosition.y);

	const float speed = 150.0f;

	if (deltaLength > 1.0f)
	{
		const sf::Vector2f direction = { deltaPosition.x / deltaLength, deltaPosition.y / deltaLength };
		arrow.move(direction * speed * dt);
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
	sf::Clock clock;
	WindowSettings windowSettings;
	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;
	sf::RenderWindow window(
		sf::VideoMode({ windowSettings.windowWidth, windowSettings.windowHeight }),
		windowSettings.windowTitle, sf::Style::Default, contextSettings);
	sf::ConvexShape arrow;
	sf::Vector2f mousePosition;

	InitArrow(arrow);
	while (window.isOpen())
	{
		PollEvents(window, mousePosition);
		UpdateMousePosition(mousePosition, arrow, clock);
		DrawFrame(window, arrow);
	}
}