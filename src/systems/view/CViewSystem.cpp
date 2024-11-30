#include "CViewSystem.h"
#include "../CGameController.h"
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
	const auto currentState = CGameController::GetCurrentGameState();
	if (currentState == CurrentState::MainMenu)
	{
		sf::View view = m_window.getDefaultView();
		m_window.setView(view);

		CViewSystem::DrawMainMenu();
		CViewSystem::DrawMenuSoul();
	}
	if (currentState == CurrentState::LevelChoosing)
	{
		sf::View view = m_window.getDefaultView();
		m_window.setView(view);

		CViewSystem::DrawLevelChoosingMenu();
	}
	if (currentState == CurrentState::Player)
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
	if (currentState == CurrentState::PauseMenu)
	{
		sf::View view = m_window.getDefaultView();
		m_window.setView(view);

		CViewSystem::DrawField();
		CViewSystem::DrawPauseMenu();
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
		if (entityManager.HasComponent<MenuSoulComponent>(entityId))
		{
			continue;
		}
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

void CViewSystem::DrawMainMenu()
{
	auto& font = CFontStorage::GetFont("8bitoperator_jve.ttf");

	m_menu.backgroundTexture = CTextureStorage::GetTexture("menu_background.png");
	m_menu.backgroundSprite.setTexture(m_menu.backgroundTexture);

	if (m_menu.menuOptions.empty())
	{
		sf::Text startGameText;
		startGameText.setFont(font);
		startGameText.setString("Start game");
		startGameText.setPosition(500, 550);
		startGameText.setCharacterSize(50);
		m_menu.menuOptions.push_back(startGameText);

		sf::Text settingsText;
		settingsText.setFont(font);
		settingsText.setString("Settings");
		settingsText.setPosition(500, 650);
		settingsText.setCharacterSize(50);
		m_menu.menuOptions.push_back(settingsText);

		sf::Text instructionsText;
		instructionsText.setFont(font);
		instructionsText.setString("Guide");
		instructionsText.setPosition(1250, 550);
		instructionsText.setCharacterSize(50);
		m_menu.menuOptions.push_back(instructionsText);

		sf::Text exitText;
		exitText.setFont(font);
		exitText.setString("Exit");
		exitText.setPosition(1250, 650);
		exitText.setCharacterSize(50);
		m_menu.menuOptions.push_back(exitText);
	}

	m_window.draw(m_menu.backgroundSprite);

	for (int i = 0; i < m_menu.menuOptions.size(); i++)
	{
		if (m_menu.selectedMenuOption == i)
		{
			m_menu.menuOptions[i].setFillColor(sf::Color::Yellow);
			m_menu.menuOptions[i].setCharacterSize(60);
		}
		else
		{
			m_menu.menuOptions[i].setFillColor(sf::Color::White);
			m_menu.menuOptions[i].setCharacterSize(50);
		}

		m_window.draw(m_menu.menuOptions[i]);
	}
}

void CViewSystem::DrawMenuSoul()
{
	auto& entityManager = CEntityManager::GetInstance();
	auto soulId = entityManager.GetEntitiesWithComponents<MenuSoulComponent>().front();
	auto* imageComp = entityManager.GetComponent<ImageComponent>(soulId);
	auto* positionComp = entityManager.GetComponent<PositionComponent>(soulId);

	bool isOptionSelected = false;
	for (int i = 0; i < m_menu.menuOptions.size(); i++)
	{
		sf::FloatRect optionBoundary = m_menu.menuOptions[i].getGlobalBounds();
		sf::FloatRect soulBoundary = m_menu.menuSoul.getGlobalBounds();
		if (optionBoundary.intersects(soulBoundary))
		{
			m_menu.selectedMenuOption = i;
			CGameController::SetCurrentMainMenuOption(i);
			isOptionSelected = true;
		}
	}
	if (!isOptionSelected)
	{
		m_menu.selectedMenuOption = -1;
		CGameController::SetCurrentMainMenuOption(-1);
	}

	m_menu.menuSoul = imageComp->sprite;
	m_menu.menuSoul.setPosition(positionComp->x, positionComp->y);
	m_menu.menuSoul.setScale(0.15f, 0.15f);
	m_window.draw(m_menu.menuSoul);
}

void CViewSystem::DrawPauseMenu()
{
	m_pauseMenu.pauseMenuAreaTexture = CTextureStorage::GetTexture("utils_windows.png");
	m_pauseMenu.pauseMenuArea.setTexture(m_pauseMenu.pauseMenuAreaTexture);
	m_pauseMenu.pauseMenuArea.setTextureRect(sf::IntRect(841, 21, 260, 146));
	m_pauseMenu.pauseMenuArea.setPosition(m_window.getView().getCenter().x - 260.f * 3 / 2, m_window.getView().getCenter().y - 73.f * 3 / 2);
	m_pauseMenu.pauseMenuArea.setScale(3.f, 3.f);

	auto& font = CFontStorage::GetFont("8bitoperator_jve.ttf");
	m_pauseMenu.pauseMenuTitle.setFont(font);
	m_pauseMenu.pauseMenuTitle.setString("PAUSE");
	m_pauseMenu.pauseMenuTitle.setCharacterSize(40);

	const float titleX = m_pauseMenu.pauseMenuArea.getPosition().x + m_pauseMenu.pauseMenuArea.getScale().x * 260 / 2.2f;
	const float titleY = m_pauseMenu.pauseMenuArea.getPosition().y + 30;
	m_pauseMenu.pauseMenuTitle.setPosition(titleX, titleY);

	if (m_pauseMenu.menuOptions.empty())
	{
		sf::Text resumeGameText;
		resumeGameText.setFont(font);
		resumeGameText.setString("Resume game");
		resumeGameText.setPosition(m_pauseMenu.pauseMenuArea.getPosition().x + 80, m_pauseMenu.pauseMenuArea.getPosition().y + 100);
		resumeGameText.setCharacterSize(30);
		m_pauseMenu.menuOptions.push_back(resumeGameText);

		sf::Text settingsText;
		settingsText.setFont(font);
		settingsText.setString("Settings");
		settingsText.setPosition(m_pauseMenu.pauseMenuArea.getPosition().x + 80, m_pauseMenu.pauseMenuArea.getPosition().y + 150);
		settingsText.setCharacterSize(30);
		m_pauseMenu.menuOptions.push_back(settingsText);

		sf::Text exitText;
		exitText.setFont(font);
		exitText.setString("Exit to main menu");
		exitText.setPosition(m_pauseMenu.pauseMenuArea.getPosition().x + 80, m_pauseMenu.pauseMenuArea.getPosition().y + 200);
		exitText.setCharacterSize(30);
		m_pauseMenu.menuOptions.push_back(exitText);
	}

	m_window.draw(m_pauseMenu.pauseMenuArea);
	m_window.draw(m_pauseMenu.pauseMenuTitle);

	for (int i = 0; i < m_pauseMenu.menuOptions.size(); i++)
	{
		if (CGameController::GetCurrentPauseMenuOption() == i)
		{
			m_pauseMenu.menuOptions[i].setFillColor(sf::Color::Yellow);
			m_pauseMenu.menuOptions[i].setCharacterSize(35);
		}
		else
		{
			m_pauseMenu.menuOptions[i].setFillColor(sf::Color::White);
			m_pauseMenu.menuOptions[i].setCharacterSize(30);
		}
		m_window.draw(m_pauseMenu.menuOptions[i]);
	}
}

void CViewSystem::DrawLevelChoosingMenu()
{
	auto& font = CFontStorage::GetFont("8bitoperator_jve.ttf");

	m_levelChoosingMenu.levelChoosingMenuTitle.setFont(font);
	m_levelChoosingMenu.levelChoosingMenuTitle.setString("Please select a file:");
	m_levelChoosingMenu.levelChoosingMenuTitle.setPosition(m_window.getView().getCenter().x - 289.f * 3 / 2, m_window.getView().getCenter().y - 225);
	m_levelChoosingMenu.levelChoosingMenuTitle.setCharacterSize(40);

	m_levelChoosingMenu.backgroundTexture = CTextureStorage::GetTexture("menu_background.png");
	m_levelChoosingMenu.backgroundSprite.setTexture(m_levelChoosingMenu.backgroundTexture);

	sf::Sprite save1InfoCard;
	sf::Sprite save2InfoCard;
	sf::Sprite save3InfoCard;

	if (m_levelChoosingMenu.levelCards.empty())
	{
		save1InfoCard.setTexture(CTextureStorage::GetTexture("utils_windows.png"));
		save1InfoCard.setTextureRect(sf::IntRect(544, 21, 289, 76));
		save1InfoCard.setPosition(m_window.getView().getCenter().x - 289.f * 3 / 2, m_window.getView().getCenter().y - 150);
		save1InfoCard.setScale(2.f, 2.f);
		m_levelChoosingMenu.levelCards.push_back(save1InfoCard);

		save2InfoCard.setTexture(CTextureStorage::GetTexture("utils_windows.png"));
		save2InfoCard.setTextureRect(sf::IntRect(544, 21, 289, 76));
		save2InfoCard.setPosition(m_window.getView().getCenter().x - 289.f * 3 / 2, m_window.getView().getCenter().y + 25);
		save2InfoCard.setScale(2.f, 2.f);
		m_levelChoosingMenu.levelCards.push_back(save2InfoCard);

		save3InfoCard.setTexture(CTextureStorage::GetTexture("utils_windows.png"));
		save3InfoCard.setTextureRect(sf::IntRect(544, 21, 289, 76));
		save3InfoCard.setPosition(m_window.getView().getCenter().x - 289.f * 3 / 2, m_window.getView().getCenter().y + 200);
		save3InfoCard.setScale(2.f, 2.f);
		m_levelChoosingMenu.levelCards.push_back(save3InfoCard);
	}


	auto save1Info = CGameController::GetSaveInfo(0);
	auto save2Info = CGameController::GetSaveInfo(1);
	auto save3Info = CGameController::GetSaveInfo(2);

	if (m_levelChoosingMenu.levelCardsInfo.empty())
	{

		sf::Text playerName;
		playerName.setFont(font);
		playerName.setCharacterSize(50);
		playerName.setString(save1Info.PlayerName);
		playerName.setPosition(save1InfoCard.getPosition().x + 60, save1InfoCard.getPosition().y + 15);

		sf::Text currentLocation;
		currentLocation.setFont(font);
		currentLocation.setCharacterSize(40);
		currentLocation.setString(save1Info.currentLocation);
		currentLocation.setPosition(save1InfoCard.getPosition().x + 60, save1InfoCard.getPosition().y + 85);

		int minutes = static_cast<int>(save1Info.gameTime) / 60;
		int seconds = static_cast<int>(save1Info.gameTime) % 60;

		sf::Text passedTime;
		passedTime.setFont(font);
		passedTime.setCharacterSize(45);
		passedTime.setString("Time: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
		passedTime.setPosition(save1InfoCard.getPosition().x + 200, save1InfoCard.getPosition().y + 20);

		m_levelChoosingMenu.levelCardsInfo[0].push_back(playerName);
		m_levelChoosingMenu.levelCardsInfo[0].push_back(currentLocation);
		m_levelChoosingMenu.levelCardsInfo[0].push_back(passedTime);

		playerName.setString(save2Info.PlayerName);
		playerName.setPosition(save2InfoCard.getPosition().x + 60, save2InfoCard.getPosition().y + 15);

		currentLocation.setString(save2Info.currentLocation);
		currentLocation.setPosition(save2InfoCard.getPosition().x + 60, save2InfoCard.getPosition().y + 85);

		minutes = static_cast<int>(save2Info.gameTime) / 60;
		seconds = static_cast<int>(save2Info.gameTime) % 60;

		passedTime.setString("Time: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
		passedTime.setPosition(save2InfoCard.getPosition().x + 200, save2InfoCard.getPosition().y + 20);

		m_levelChoosingMenu.levelCardsInfo[1].push_back(playerName);
		m_levelChoosingMenu.levelCardsInfo[1].push_back(currentLocation);
		m_levelChoosingMenu.levelCardsInfo[1].push_back(passedTime);

		playerName.setString(save3Info.PlayerName);
		playerName.setPosition(save3InfoCard.getPosition().x + 60, save3InfoCard.getPosition().y + 15);

		currentLocation.setString(save3Info.currentLocation);
		currentLocation.setPosition(save3InfoCard.getPosition().x + 60, save3InfoCard.getPosition().y + 85);

		minutes = static_cast<int>(save3Info.gameTime) / 60;
		seconds = static_cast<int>(save3Info.gameTime) % 60;

		passedTime.setString("Time: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
		passedTime.setPosition(save3InfoCard.getPosition().x + 200, save3InfoCard.getPosition().y + 20);

		m_levelChoosingMenu.levelCardsInfo[2].push_back(playerName);
		m_levelChoosingMenu.levelCardsInfo[2].push_back(currentLocation);
		m_levelChoosingMenu.levelCardsInfo[2].push_back(passedTime);
	}

	m_window.draw(m_levelChoosingMenu.backgroundSprite);
	m_window.draw(m_levelChoosingMenu.levelChoosingMenuTitle);

	for (int i = 0; i < m_levelChoosingMenu.levelCards.size(); i++)
	{
		if (CGameController::GetCurrentGameSaveNumber() == i)
		{
			m_levelChoosingMenu.levelCards[i].setScale(2.1f, 2.1f);
		}
		else
		{
			m_levelChoosingMenu.levelCards[i].setScale(2.f, 2.f);
		}
		m_window.draw(m_levelChoosingMenu.levelCards[i]);
	}

	for (const auto& levelCardInfo : m_levelChoosingMenu.levelCardsInfo)
	{
		for (const auto& text : levelCardInfo.second)
		{
			m_window.draw(text);
		}
	}
}