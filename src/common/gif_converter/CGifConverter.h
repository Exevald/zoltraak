#pragma once

#include "SFML/Graphics.hpp"
#include "../gif_load.h"
#include <vector>


class CGifConverter
{
public:
	CGifConverter();
private:
	std::vector<sf::Texture> frames;
	std::vector<float> frameDurations;
	size_t currentFrame = 0;
	float currentTime = 0.f;
	sf::Sprite sprite;
};
