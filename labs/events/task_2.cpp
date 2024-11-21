#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

struct WindowSettings
{
	std::string windowTitle = "Eyes follow mouse";
	unsigned int windowWidth = 800;
	unsigned int windowHeight = 600;
};

struct Eye
{
	sf::ConvexShape pupil;
	sf::ConvexShape ocular;
	sf::CircleShape highlight;
	sf::Vector2f highlightVelocity;
	sf::Vector2f highlightAcceleration;
};

const sf::Vector2f PupilSize = { 50.f, 60.f };
const sf::Vector2f OcularSize = { 95.f, 150.f };
constexpr int PointCount = 200;
const float HighlightRadius = 10.f;
const float HighlightSpeed = 100.f;
const float HighlightAcceleration = 10.f;
const float Friction = 0.9f;
const float Gravity = 0.2f;
const float BounceFactor = 1.0f;

sf::Vector2f GetPupilPosition(const sf::Vector2f& eyeCenter, const sf::Vector2f& mousePos)
{
	sf::Vector2f direction = mousePos - eyeCenter;
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (distance <= (OcularSize.x - PupilSize.x) && distance <= (OcularSize.y - PupilSize.y))
	{
		return mousePos;
	}

	float angle = std::atan2(direction.y, direction.x);
	float maxDistanceX = OcularSize.x - PupilSize.x;
	float maxDistanceY = OcularSize.y - PupilSize.y;

	float maxDistance = std::min(maxDistanceX, maxDistanceY);

	if (distance > maxDistance)
	{
		direction.x = std::cos(angle) * maxDistance;
		direction.y = std::sin(angle) * maxDistance;
	}

	return eyeCenter + direction;
}

void InitEye(Eye& eye, const sf::Vector2f& position, const sf::Vector2f& radius, const sf::Vector2f& scleraRadius)
{
	eye.pupil.setPosition(position);
	eye.pupil.setFillColor(sf::Color::Black);
	eye.pupil.setPointCount(PointCount);

	eye.ocular.setPosition(position);
	eye.ocular.setFillColor(sf::Color::White);
	eye.ocular.setPointCount(PointCount);

	eye.highlight.setRadius(HighlightRadius);
	eye.highlight.setFillColor(sf::Color::White);

	sf::Vector2f pupilCenter = position - sf::Vector2f(-radius.x, -radius.y);
	eye.highlight.setPosition(pupilCenter);

	eye.highlightVelocity = sf::Vector2f(HighlightSpeed, HighlightSpeed);
	eye.highlightAcceleration = sf::Vector2f(HighlightAcceleration, HighlightAcceleration);

	for (int pointNo = 0; pointNo < PointCount; ++pointNo)
	{
		const auto angle = float(2.0f * M_PI * pointNo / PointCount);
		const sf::Vector2f pupilPoint = {
			radius.x * std::sin(angle),
			radius.y * std::cos(angle)
		};
		eye.pupil.setPoint(pointNo, pupilPoint);

		const sf::Vector2f ocularPoint = {
			scleraRadius.x * std::sin(angle),
			scleraRadius.y * std::cos(angle)
		};
		eye.ocular.setPoint(pointNo, ocularPoint);
	}
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

void UpdatePupilsPosition(const sf::Vector2f& mousePosition, Eye& leftEye, Eye& rightEye)
{
	leftEye.pupil.setPosition(GetPupilPosition(leftEye.ocular.getPosition(), mousePosition));
	rightEye.pupil.setPosition(GetPupilPosition(rightEye.ocular.getPosition(), mousePosition));
}

void ApplyPhysics(Eye& eye)
{
	eye.highlightVelocity += eye.highlightAcceleration;
	eye.highlightVelocity *= Friction;
	eye.highlight.move(eye.highlightVelocity);
	eye.highlightAcceleration = sf::Vector2f(0.f, Gravity);
}

void CheckBounds(Eye& eye)
{
	sf::Vector2f pupilCenter = eye.pupil.getPosition();
	sf::Vector2f highlightPosition = eye.highlight.getPosition();

	sf::Vector2f direction = highlightPosition - pupilCenter;
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	float maxDistance = PupilSize.x / 2.f - HighlightRadius;

	if (distance + HighlightRadius > maxDistance)
	{
		sf::Vector2f deltaPos = highlightPosition - pupilCenter;
		deltaPos /= std::sqrt(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y);

		highlightPosition = pupilCenter + deltaPos * (maxDistance - HighlightRadius);
		eye.highlight.setPosition(highlightPosition);

		eye.highlightVelocity = -eye.highlightVelocity * BounceFactor;
	}
}

void UpdateHighlight(Eye& eye)
{
	ApplyPhysics(eye);
	CheckBounds(eye);
}

void DrawFrame(sf::RenderWindow& window, Eye& leftEye, Eye& rightEye)
{
	window.clear();
	window.draw(leftEye.ocular);
	window.draw(rightEye.ocular);
	window.draw(leftEye.pupil, sf::RenderStates(sf::BlendAlpha));
	window.draw(rightEye.pupil, sf::RenderStates(sf::BlendAlpha));
	window.draw(leftEye.highlight);
	window.draw(rightEye.highlight);
	window.display();
}

int main()
{
	const WindowSettings windowSettings;

	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;

	sf::RenderWindow window(
		sf::VideoMode(windowSettings.windowWidth, windowSettings.windowHeight),
		windowSettings.windowTitle,
		sf::Style::Default, contextSettings);

	Eye leftEye, rightEye;
	sf::Vector2f mousePosition;

	InitEye(leftEye, { 300.f, 300.f }, PupilSize, OcularSize);
	InitEye(rightEye, { 500.f, 300.f }, PupilSize, OcularSize);

	while (window.isOpen())
	{
		PollEvents(window, mousePosition);
		UpdatePupilsPosition(mousePosition, leftEye, rightEye);
		UpdateHighlight(leftEye);
		UpdateHighlight(rightEye);
		DrawFrame(window, leftEye, rightEye);
	}

	return EXIT_SUCCESS;
}
