#pragma once

#include "actions/Actions.h"
#include "view/renderers/IRenderer.h"

struct Vendor
{
	sf::RectangleShape area;
	sf::Sprite background;
	sf::Sprite vendorIcon;
	sf::Sprite shopArea;
	std::vector<sf::Text> vendorActions;
	std::vector<sf::Text> vendorItems;
	std::vector<sf::Text> heroItems;
	sf::Text vendorText;
	sf::Sprite menuSoul;
	sf::Text heroesMoneyText;
};

class CVendorRenderer : public IRenderer
{
public:
	explicit CVendorRenderer(Vendor& vendor)
		: m_vendor(vendor)
	{
	}

	void Init() override;
	void Draw(sf::RenderWindow& window) override;

private:
	void DrawVendorArea(sf::RenderWindow& window);
	void DrawVendorBackground(sf::RenderWindow& window);
	void DrawVendorIcon(sf::RenderWindow& window);
	void DrawShopArea(sf::RenderWindow& window);
	void DrawVendorActions(sf::RenderWindow& window);
	void DrawHeroesMoneyText(sf::RenderWindow& window);
	void DrawVendorInventoryItems(sf::RenderWindow& window);
	void DrawHeroesInventoryItems(sf::RenderWindow& window);

	static sf::Text CreateText(const std::string& textString, int characterSize, float posX, float posY);

	Vendor& m_vendor;
};