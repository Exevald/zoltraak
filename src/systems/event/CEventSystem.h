#pragma once

#include "../../CGameController.h"

class CEventSystem
{
public:
	static void Init();
	static void HandleMouseClick(CGameController& gameController, const sf::Vector2f& mousePos);
	static void HandleKeyPress(CGameController& gameController, sf::Keyboard::Key key);
private:
	static InventoryState m_currentInventoryState;
};