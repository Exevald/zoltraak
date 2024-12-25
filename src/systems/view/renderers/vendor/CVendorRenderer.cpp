#include "CVendorRenderer.h"
#include "../../../../CGameController.h"
#include "../../../../common/assets_storage/CTextureStorage.h"
#include "assets_storage/CFontStorage.h"

static auto& Font = CFontStorage::GetFont("8bitoperator_jve.ttf");
const int TextSize = 40;

void CVendorRenderer::Init()
{
}

void CVendorRenderer::Draw(sf::RenderWindow& window)
{
	if (CGameController::GetCurrentGameState() == CurrentState::Vendor)
	{
		CVendorRenderer::DrawVendorArea(window);
		CVendorRenderer::DrawVendorBackground(window);
		CVendorRenderer::DrawVendorIcon(window);
		CVendorRenderer::DrawShopArea(window);
		CVendorRenderer::DrawVendorActions(window);
		CVendorRenderer::DrawHeroesMoneyText(window);
		if (CGameController::GetCurrentVendorState() == VendorState::Buy)
		{
			CVendorRenderer::DrawVendorInventoryItems(window);
		}
		if (CGameController::GetCurrentVendorState() == VendorState::Sell)
		{
			CVendorRenderer::DrawHeroesInventoryItems(window);
		}
	}
}

void CVendorRenderer::DrawVendorArea(sf::RenderWindow& window)
{
	const float vendorAreaWidth = 1120;
	const float vendorAreaHeight = 768;

	sf::View view = window.getDefaultView();
	window.setView(view);

	m_vendor.area.setPosition(float(window.getSize().x) / 2 - vendorAreaWidth / 2, float(window.getSize().y) / 2 - vendorAreaHeight / 2);
	m_vendor.area.setSize({ vendorAreaWidth, vendorAreaHeight });
	m_vendor.area.setOutlineThickness(5);
	m_vendor.area.setOutlineColor(sf::Color::White);
	m_vendor.area.setFillColor(sf::Color::Black);

	window.draw(m_vendor.area);
}

void CVendorRenderer::DrawVendorBackground(sf::RenderWindow& window)
{
	m_vendor.background.setTexture(CTextureStorage::GetTexture("vendor_hero1.png"));
	m_vendor.background.setTextureRect(sf::IntRect(5, 419, 320, 120));
	m_vendor.background.setScale(3.5f, 3.5f);
	m_vendor.background.setPosition(m_vendor.area.getPosition().x, m_vendor.area.getPosition().y);

	window.draw(m_vendor.background);
}

void CVendorRenderer::DrawVendorIcon(sf::RenderWindow& window)
{
	m_vendor.vendorIcon.setTexture(CTextureStorage::GetTexture("vendor_hero1.png"));
	m_vendor.vendorIcon.setTextureRect(sf::IntRect(5, 24, 135, 108));
	m_vendor.vendorIcon.setScale(3.f, 3.f);
	m_vendor.vendorIcon.setPosition(m_vendor.area.getPosition().x + 350, m_vendor.area.getPosition().y + 95);

	window.draw(m_vendor.vendorIcon);
}

void CVendorRenderer::DrawShopArea(sf::RenderWindow& window)
{
	m_vendor.shopArea.setTexture(CTextureStorage::GetTexture("utils_windows.png"));
	m_vendor.shopArea.setTextureRect(sf::IntRect(216, 126, 320, 119));
	m_vendor.shopArea.setScale(3.6f, 3.f);
	m_vendor.shopArea.setPosition(m_vendor.area.getPosition().x - 20, m_vendor.area.getPosition().y + 420);

	window.draw(m_vendor.shopArea);
}

void CVendorRenderer::DrawVendorActions(sf::RenderWindow& window)
{
	m_vendor.vendorActions.clear();
	const float textPosX = m_vendor.shopArea.getPosition().x + 800;

	m_vendor.vendorActions.push_back(CVendorRenderer::CreateText(
		"Buy items",
		TextSize,
		textPosX,
		m_vendor.shopArea.getPosition().y + 50));
	m_vendor.vendorActions.push_back(CVendorRenderer::CreateText(
		"Sell items",
		TextSize,
		textPosX,
		m_vendor.shopArea.getPosition().y + 100));
	m_vendor.vendorActions.push_back(CVendorRenderer::CreateText(
		"Talk with vendor",
		TextSize,
		textPosX,
		m_vendor.shopArea.getPosition().y + 150));
	m_vendor.vendorActions.push_back(CVendorRenderer::CreateText(
		"Exit shop",
		TextSize,
		textPosX,
		m_vendor.shopArea.getPosition().y + 200));

	for (int i = 0; i < m_vendor.vendorActions.size(); i++)
	{
		auto action = m_vendor.vendorActions[i];
		if (i == CGameController::GetCurrentVendorActionNumber())
		{
			action.setFillColor(sf::Color::Yellow);
		}
		window.draw(action);
	}
}

void CVendorRenderer::DrawHeroesMoneyText(sf::RenderWindow& window)
{
	auto& entityManager = CEntityManager::GetInstance();
	auto charactersWithMoney = entityManager.GetEntitiesWithComponents<MoneyComponent>();
	const float textPosX = m_vendor.shopArea.getPosition().x + 800;
	int totalMoney = 0;
	for (int characterId : charactersWithMoney)
	{
		auto moneyComp = entityManager.GetComponent<MoneyComponent>(characterId);
		totalMoney += moneyComp->money;
	}

	m_vendor.heroesMoneyText = CreateText("$" + std::to_string(totalMoney), TextSize, textPosX, m_vendor.shopArea.getPosition().y + 250);
	window.draw(m_vendor.heroesMoneyText);
}

void CVendorRenderer::DrawVendorInventoryItems(sf::RenderWindow& window)
{
	auto& entityManager = CEntityManager::GetInstance();
	auto vendor = entityManager.GetEntitiesWithComponents<VendorComponent>().front();
	auto vendorComp = entityManager.GetComponent<VendorComponent>(vendor);
	m_vendor.vendorItems.clear();

	float yOffset = 0;
	for (const auto& item : vendorComp->items)
	{
		m_vendor.vendorItems.push_back(CVendorRenderer::CreateText(
			item.name + "   $" + std::to_string(item.cost),
			TextSize,
			m_vendor.shopArea.getPosition().x + 50,
			m_vendor.shopArea.getPosition().y + 50 + yOffset));
		yOffset += 50;
	}

	for (int i = 0; i < m_vendor.vendorItems.size(); i++)
	{
		auto item = m_vendor.vendorItems[i];
		if (i == CGameController::GetCurrentVendorItemToBuyNumber())
		{
			item.setFillColor(sf::Color::Yellow);
		}
		window.draw(item);
	}
}

void CVendorRenderer::DrawHeroesInventoryItems(sf::RenderWindow& window)
{
	auto& entityManager = CEntityManager::GetInstance();
	auto entitiesWithInventory = entityManager.GetEntitiesWithComponents<InventoryComponent>();
	m_vendor.heroItems.clear();

	std::vector<InventoryItem> commonHeroesInventory = CGameController::GetHeroesItemsToSell();

	float yOffset = 0;
	for (const auto& item : commonHeroesInventory)
	{
		m_vendor.heroItems.push_back(CVendorRenderer::CreateText(
			item.name + " $" + std::to_string(item.cost),
			TextSize,
			m_vendor.shopArea.getPosition().x + 50,
			m_vendor.shopArea.getPosition().y + 50 + yOffset));
		yOffset += 50;
	}

	for (int i = 0; i < m_vendor.heroItems.size(); i++)
	{
		auto item = m_vendor.heroItems[i];
		if (i == CGameController::GetCurrentHeroItemToSellNumber())
		{
			item.setFillColor(sf::Color::Yellow);
		}
		window.draw(item);
	}
}

sf::Text CVendorRenderer::CreateText(const std::string& textString, int characterSize, float posX, float posY)
{
	sf::Text text;
	text.setFont(Font);
	text.setString(textString);
	text.setCharacterSize(characterSize);
	text.setPosition(posX, posY);

	return text;
}