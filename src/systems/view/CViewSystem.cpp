#include "CViewSystem.h"
#include "CEntityManager.h"
#include "Utils.h"
#include "events/CEventDispatcher.h"
#include "storage/CFontStorage.h"
#include "storage/CTextureStorage.h"

void CViewSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::EntitySelected, [this](const SEvent& event) {
		const auto& selectedEventData = std::get<EntitySelectedEventData>(event.data);
		UpdateHeroCard(selectedEventData.id);
	});
}

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
		m_window.draw(m_heroCard.heroExperienceText);
	}
}

void CViewSystem::DrawField()
{
	const auto map = CTextureStorage::GetTexture("map_fieldOfHopesAndDreams.png");
	const auto commonMapItems = CTextureStorage::GetTexture("map_commonItems.png");

	for (int i = 0; i < m_level.size(); i++)
	{
		auto line = m_level[i];
		for (int j = 0; j < line.size(); j++)
		{
			auto ch = line[j];
			switch (ch)
			{
			case '0': {
				sf::RectangleShape rectangle;
				rectangle.setFillColor(sf::Color::Black);
				rectangle.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
				rectangle.setSize({ 60, 60 });
				m_window.draw(rectangle);
				break;
			}
			case '1': {
				sf::Sprite mapItemSprite;
				mapItemSprite.setTexture(map);
				mapItemSprite.setTextureRect(sf::IntRect(316, 15, 20, 20));
				mapItemSprite.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
				mapItemSprite.setScale(3.f, 3.f);
				m_window.draw(mapItemSprite);
				break;
			}
			case '2': {
				sf::Sprite mapItemSprite;
				mapItemSprite.setTexture(map);
				mapItemSprite.setTextureRect(sf::IntRect(22, 78, 20, 20));
				mapItemSprite.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
				mapItemSprite.setScale(3.f, 3.f);
				m_window.draw(mapItemSprite);
				break;
			}
			case '3': {
				sf::Sprite mapItemSprite;
				mapItemSprite.setTexture(map);
				mapItemSprite.setTextureRect(sf::IntRect(1, 78, 20, 20));
				mapItemSprite.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
				mapItemSprite.setScale(3.f, 3.f);
				m_window.draw(mapItemSprite);
				break;
			}
			case '4': {
				sf::Sprite mapItemSprite;
				mapItemSprite.setTexture(map);
				mapItemSprite.setTextureRect(sf::IntRect(85, 78, 20, 20));
				mapItemSprite.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
				mapItemSprite.setScale(3.f, 3.f);
				m_window.draw(mapItemSprite);
				break;
			}
			case '5': {
				sf::Sprite mapItemSprite;
				mapItemSprite.setTexture(map);
				mapItemSprite.setTextureRect(sf::IntRect(22, 36, 20, 20));
				mapItemSprite.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
				mapItemSprite.setScale(3.f, 3.f);
				m_window.draw(mapItemSprite);
				break;
			}
			case '6': {
				sf::Sprite mapItemSprite;
				mapItemSprite.setTexture(map);
				mapItemSprite.setTextureRect(sf::IntRect(64, 57, 20, 20));
				mapItemSprite.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
				mapItemSprite.setScale(3.f, 3.f);
				m_window.draw(mapItemSprite);
				break;
			}
			case '7': {
				sf::Sprite mapItemSprite;
				mapItemSprite.setTexture(map);
				mapItemSprite.setTextureRect(sf::IntRect(64, 36, 20, 20));
				mapItemSprite.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
				mapItemSprite.setScale(3.f, 3.f);
				m_window.draw(mapItemSprite);
				break;
			}
			case '8': {
				sf::Sprite mapItemSprite;
				mapItemSprite.setTexture(map);
				mapItemSprite.setTextureRect(sf::IntRect(1, 57, 20, 20));
				mapItemSprite.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
				mapItemSprite.setScale(3.f, 3.f);
				m_window.draw(mapItemSprite);
				break;
			}
			case '9': {
				sf::Sprite mapItemSprite;
				mapItemSprite.setTexture(map);
				mapItemSprite.setTextureRect(sf::IntRect(1, 36, 20, 20));
				mapItemSprite.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
				mapItemSprite.setScale(3.f, 3.f);
				m_window.draw(mapItemSprite);
				break;
			}
			default:
				break;
			}
		}
		for (int i = 0; i < m_level.size(); i++)
		{
			line = m_level[i];
			for (int j = 0; j < line.size(); j++)
			{
				auto ch = line[j];
				if (ch == '@')
				{
					sf::Sprite mapItemSprite;
					mapItemSprite.setTexture(map);
					mapItemSprite.setTextureRect(sf::IntRect(316, 78, 60, 60));
					mapItemSprite.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
					mapItemSprite.setScale(3.f, 3.f);
					m_window.draw(mapItemSprite);
				}
				if (ch == '!')
				{
					sf::Sprite mapItemSprite;
					mapItemSprite.setTexture(map);
					mapItemSprite.setTextureRect(sf::IntRect(22, 36, 20, 20));
					mapItemSprite.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
					mapItemSprite.setScale(3.f, 3.f);
					m_window.draw(mapItemSprite);
					mapItemSprite.setTextureRect(sf::IntRect(43, 36, 20, 20));
					mapItemSprite.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
					mapItemSprite.setScale(3.f, 3.f);
					m_window.draw(mapItemSprite);
				}
				if (ch == 'S')
				{
					sf::Sprite mapItemSprite;
					mapItemSprite.setTexture(map);
					mapItemSprite.setTextureRect(sf::IntRect(1, 141, 191, 109));
					mapItemSprite.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
					mapItemSprite.setScale(3.f, 3.f);
					m_window.draw(mapItemSprite);
				}
				if (ch == 'C')
				{
					sf::Sprite mapItemSprite;
					mapItemSprite.setTexture(commonMapItems);
					mapItemSprite.setTextureRect(sf::IntRect(42, 190, 40, 60));
					mapItemSprite.setPosition(float(j) * 20 * 3 - 60, float(i) * 20 * 3 - 60);
					mapItemSprite.setScale(3.f, 3.f);
					m_window.draw(mapItemSprite);
				}
			}
		}
	}
}

void CViewSystem::DrawItems()
{
	auto& entityManager = CEntityManager::GetInstance();
	auto entitiesWithAnimations = entityManager.GetEntitiesWithComponents<AnimationComponent>();
	auto entitiesWithImages = entityManager.GetEntitiesWithComponents<ImageComponent>();

	for (EntityId entityId : entitiesWithAnimations)
	{
		const auto* animComp = entityManager.GetComponent<AnimationComponent>(entityId);

		m_window.draw(animComp->sprite);
	}

	for (EntityId entityId : entitiesWithImages)
	{
		const auto* imageComp = entityManager.GetComponent<ImageComponent>(entityId);
		const auto* positionComp = entityManager.GetComponent<PositionComponent>(entityId);

		sf::Sprite sprite = imageComp->sprite;
		sprite.setPosition(positionComp->x, positionComp->y);

		m_window.draw(sprite);
	}
}

void CViewSystem::UpdateHeroCard(EntityId selectedEntity)
{
	auto& entityManager = CEntityManager::GetInstance();
	auto* healthComp = entityManager.GetComponent<HealthComponent>(selectedEntity);
	auto* manaComp = entityManager.GetComponent<ManaComponent>(selectedEntity);
	auto* colorThemeComp = entityManager.GetComponent<ColorThemeComponent>(selectedEntity);
	auto* avatarComp = entityManager.GetComponent<AvatarComponent>(selectedEntity);
	auto* experienceComp = entityManager.GetComponent<ExperienceComponent>(selectedEntity);

	if (!healthComp || !manaComp || !colorThemeComp || !avatarComp || !experienceComp)
	{
		m_heroCard.visible = false;
		return;
	}

	m_heroCard.visible = true;

	m_heroCard.shape.setSize({ 700, 200 });
	m_heroCard.shape.setFillColor(sf::Color::Black);
	m_heroCard.shape.setOutlineColor(colorThemeComp->colorTheme);
	m_heroCard.shape.setOutlineThickness(4);

	auto& texture = CTextureStorage::GetTexture(avatarComp->avatarFilePath);
	auto& font = CFontStorage::GetFont("8bitoperator_jve.ttf");

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

	m_heroCard.heroExperienceText.setFont(font);
	m_heroCard.heroExperienceText.setString("EXP:  " + std::to_string(experienceComp->experience) + "/" + std::to_string(experienceComp->levelExperienceLimit) + ",  LEVEL: " + std::to_string(experienceComp->currentHeroLevel));
	m_heroCard.heroExperienceText.setCharacterSize(30);
	m_heroCard.heroExperienceText.setFillColor(sf::Color::White);
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

	m_heroCard.heroExperienceText.setPosition(viewCenter.x - m_heroCard.shape.getSize().x / 5.6f,
		viewCenter.y + viewSize.y / 2 - m_heroCard.shape.getSize().y / 2.3f);

	m_heroCard.avatarSprite.setPosition(viewCenter.x - m_heroCard.shape.getSize().x / 2.4f,
		viewCenter.y + viewSize.y / 2 - m_heroCard.shape.getSize().y / 1.2f);
}
