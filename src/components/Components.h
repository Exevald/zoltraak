#pragma once

#include "Common.h"
#include <SFML/Graphics.hpp>

const float VELOCITY = 10;

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
	float vx;
	float vy;
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
