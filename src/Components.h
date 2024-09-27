#pragma once

#include "Common.h"

const float VELOCITY = 0.1;

enum class ShapeType
{
	Rectangle,
	Circle
};

struct ShapeComponent
{
	ShapeType type = ShapeType::Rectangle;
	sf::Color color;
	sf::Vector2f size;
	float radius{};
};

struct PositionComponent
{
	float x;
	float y;
};

struct VelocityComponent
{
	float x;
	float y;
};

using Positions = ComponentMap<PositionComponent>;
using Velocities = ComponentMap<VelocityComponent>;
using Shapes = ComponentMap<ShapeComponent>;

struct Components
{
	Positions positions;
	Velocities velocities;
	Shapes shapes;
};
