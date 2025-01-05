#pragma once

#include "../../CGameController.h"

class CEventSystem
{
public:
	static void Init();
	static void HandleMouseClick(const sf::Vector2f& mousePos);
	static void HandleKeyPress(sf::Keyboard::Key key);
private:
	static InventoryState m_currentInventoryState;
};