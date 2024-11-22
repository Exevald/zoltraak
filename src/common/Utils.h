#pragma once

#include <string>

const float DefaultVelocity = 10.f;

struct GameFieldSettings
{
	unsigned int gameWidth = 2000;
	unsigned int gameHeight = 2000;
};

struct WindowSettings
{
	unsigned int windowWidth = 1920;
	unsigned int windowHeight = 1080;
	std::string windowTitle = "Zoltraak";
};