#include "CViewSystem.h"
#include "CEntityManager.h"
#include "Utils.h"
#include "events/CEventDispatcher.h"
#include "storage/CAssetsStorage.h"

void CViewSystem::Draw()
{
	CViewSystem::DrawField();
	CViewSystem::DrawItems();

	if (m_heroCard.visible)
	{
		UpdateHeroCardPosition();

		m_window.draw(m_heroCard.shape);
		m_window.draw(m_heroCard.maxHealthBar);
		m_window.draw(m_heroCard.healthBar);
		m_window.draw(m_heroCard.heroHealthText);
		m_window.draw(m_heroCard.maxManaBar);
		m_window.draw(m_heroCard.manaBar);
		m_window.draw(m_heroCard.heroManaText);
		m_window.draw(m_heroCard.avatarSprite);
	}
}

void CViewSystem::DrawField()
{
	GameFieldSettings gameFieldSettings;
	static sf::RectangleShape map(sf::Vector2f(float(gameFieldSettings.gameWidth), float(gameFieldSettings.gameHeight)));
	map.setFillColor(sf::Color::Black);

	m_window.draw(map);
}

void CViewSystem::DrawItems()
{
	auto& entityManager = CEntityManager::GetInstance();
	auto entitiesWithShapes = entityManager.GetEntitiesWithComponents<ShapeComponent, PositionComponent>();

	for (EntityId entityId : entitiesWithShapes)
	{
		const auto* shapeComp = entityManager.GetComponent<ShapeComponent>(entityId);
		const auto* positionComp = entityManager.GetComponent<PositionComponent>(entityId);
		const auto* rotationComp = entityManager.GetComponent<RotationComponent>(entityId);

		if (shapeComp && positionComp)
		{
			if (shapeComp->type == ShapeType::Rectangle)
			{
				sf::RectangleShape rectangleShape(shapeComp->size);
				rectangleShape.setFillColor(shapeComp->color);
				rectangleShape.setPosition(positionComp->x, positionComp->y);
				if (rotationComp)
					rectangleShape.setRotation(rotationComp->angle);
				m_window.draw(rectangleShape);
			}
			else if (shapeComp->type == ShapeType::Circle)
			{
				sf::CircleShape circleShape(shapeComp->radius);
				circleShape.setFillColor(shapeComp->color);
				circleShape.setPosition(positionComp->x - shapeComp->radius,
					positionComp->y - shapeComp->radius);
				if (rotationComp)
					circleShape.setRotation(rotationComp->angle);
				m_window.draw(circleShape);
			}
		}
	}
}

void CViewSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::EntitySelected, [this](const SEvent& event) {
		const auto& selectedEventData = std::get<EntitySelectedEventData>(event.data);
		UpdateHeroCard(selectedEventData.id);
	});
}

void CViewSystem::UpdateHeroCard(EntityId selectedEntity)
{
	auto& entityManager = CEntityManager::GetInstance();
	auto* healthComp = entityManager.GetComponent<HealthComponent>(selectedEntity);
	auto* manaComp = entityManager.GetComponent<ManaComponent>(selectedEntity);
	auto* colorThemeComp = entityManager.GetComponent<ColorThemeComponent>(selectedEntity);
	auto* avatarComp = entityManager.GetComponent<AvatarComponent>(selectedEntity);

	if (!healthComp || !manaComp || !colorThemeComp || !avatarComp)
	{
		m_heroCard.visible = false;
		return;
	}

	m_heroCard.visible = true;

	m_heroCard.shape.setSize({ 700, 200 });
	m_heroCard.shape.setFillColor(sf::Color::Black);
	m_heroCard.shape.setOutlineColor(colorThemeComp->colorTheme);
	m_heroCard.shape.setOutlineThickness(4);

	auto& texture = CAssetsStorage<sf::Texture>::GetAsset(avatarComp->avatarFilePath);
	auto& font = CAssetsStorage<sf::Font>::GetAsset("8bitoperator_jve.ttf");

	m_heroCard.avatarSprite.setTexture(texture);
	m_heroCard.avatarSprite.setScale(5.0f, 5.0f);

	const float maxHealthBarWidth = 200;
	const float maxManaBarWidth = 200;

	m_heroCard.maxHealthBar.setSize({ maxHealthBarWidth, 20 });
	m_heroCard.maxHealthBar.setFillColor(sf::Color(117, 2, 7));

	m_heroCard.healthBar.setSize({ maxHealthBarWidth * (healthComp->currentHealth / healthComp->maxHealth), 20 });
	m_heroCard.healthBar.setFillColor(colorThemeComp->colorTheme);

	m_heroCard.maxManaBar.setSize({ maxManaBarWidth, 20 });
	m_heroCard.maxManaBar.setFillColor(sf::Color(117, 2, 7));

	m_heroCard.manaBar.setSize({ maxManaBarWidth * (manaComp->currentMana / manaComp->maxMana), 20 });
	m_heroCard.manaBar.setFillColor(sf::Color(248, 158, 69));

	m_heroCard.heroHealthText.setFont(font);
	m_heroCard.heroHealthText.setString("HP");
	m_heroCard.heroHealthText.setCharacterSize(30);
	m_heroCard.heroHealthText.setFillColor(sf::Color::White);

	m_heroCard.heroManaText.setFont(font);
	m_heroCard.heroManaText.setString("MANA");
	m_heroCard.heroManaText.setCharacterSize(30);
	m_heroCard.heroManaText.setFillColor(sf::Color::White);
}

void CViewSystem::UpdateHeroCardPosition()
{
	const sf::View& currentView = m_window.getView();
	sf::Vector2f viewCenter = currentView.getCenter();
	sf::Vector2f viewSize = currentView.getSize();

	m_heroCard.shape.setPosition(viewCenter.x - m_heroCard.shape.getSize().x / 2 + 10.f,
		viewCenter.y + viewSize.y / 2 - m_heroCard.shape.getSize().y - 10.f);

	m_heroCard.healthBar.setPosition(viewCenter.x - m_heroCard.shape.getSize().x / 10,
		viewCenter.y + viewSize.y / 2 - m_heroCard.shape.getSize().y / 1.2f);
	m_heroCard.maxHealthBar.setPosition(m_heroCard.healthBar.getPosition());
	m_heroCard.heroHealthText.setPosition(viewCenter.x - m_heroCard.shape.getSize().x / 6.5f,
		viewCenter.y + viewSize.y / 2 - m_heroCard.shape.getSize().y / 1.12f);

	m_heroCard.manaBar.setPosition(viewCenter.x - m_heroCard.shape.getSize().x / 10,
		viewCenter.y + viewSize.y / 2 - m_heroCard.shape.getSize().y / 1.7f);
	m_heroCard.maxManaBar.setPosition(m_heroCard.manaBar.getPosition());
	m_heroCard.heroManaText.setPosition(viewCenter.x - m_heroCard.shape.getSize().x / 5.2f,
		viewCenter.y + viewSize.y / 2 - m_heroCard.shape.getSize().y / 1.54f);

	m_heroCard.avatarSprite.setPosition(viewCenter.x - m_heroCard.shape.getSize().x / 2.4f,
		viewCenter.y + viewSize.y / 2 - m_heroCard.shape.getSize().y / 1.2f);
}
