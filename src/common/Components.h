#pragma once

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

enum class CollisionType
{
	Character,
	MovableObject,
	StaticObject
};

struct SelectionComponent : IComponent
{
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

	VelocityComponent(float vx, float vy)
		: vx(vx)
		, vy(vy)
	{
	}
};

struct CameraComponent : IComponent
{
	float posX;
	float posY;

	CameraComponent(float posX, float posY)
		: posX(posX)
		, posY(posY)
	{
	}
};

struct CollisionComponent : IComponent
{
	CollisionType type;

	explicit CollisionComponent(CollisionType type)
		: type(type)
	{
	}
};

struct MassComponent : IComponent
{
	float mass;

	explicit MassComponent(float mass)
		: mass(mass)
	{
	}
};

struct RotationComponent
{
	float angle;

	explicit RotationComponent(float angle)
		: angle(angle)
	{
	}
};

struct InteractionComponent : IComponent
{
	bool canPush;
	bool canPickUp;

	InteractionComponent(bool canPush, bool canPickUp)
		: canPush(canPush)
		, canPickUp(canPickUp)
	{
	}
};
