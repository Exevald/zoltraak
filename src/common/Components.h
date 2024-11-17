#include <SFML/Graphics.hpp>

struct IComponent
{
	virtual ~IComponent() = default;
};

enum class ShapeType
{
	Rectangle,
	Circle
};

struct ShapeComponent : IComponent
{
	ShapeType type = ShapeType::Rectangle;
	sf::Color color;
	sf::Vector2f size;
	float radius{};

	ShapeComponent(ShapeType type, const sf::Color& color, const sf::Vector2f& size, float radius)
		: type(type)
		, color(color)
		, size(size)
		, radius(radius)
	{
	}
};

struct PositionComponent : IComponent
{
	float x;
	float y;

	PositionComponent(float x, float y)
		: x(x)
		, y(y)
	{
	}
};

struct VelocityComponent : IComponent
{
	float vx;
	float vy;
};

const float defaultVelocity = 20.f;