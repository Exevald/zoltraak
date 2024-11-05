#pragma once

#include "../../CGameController.h"

class CEventSystem
{
public:
	static void handleMouseClick(CGameController& gameController, const sf::Vector2f& mousePos);

	static void handleKeyPress(CGameController& gameController, sf::Keyboard::Key key);
};