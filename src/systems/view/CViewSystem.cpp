#include "CViewSystem.h"
#include "CEntityManager.h"
#include "Utils.h"
#include "events/CEventDispatcher.h"
#include "storage/CFontStorage.h"
#include "storage/CTextureStorage.h"
#include <iostream>

void CViewSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::EntitySelected, [this](const SEvent& event) {
		const auto& selectedEventData = std::get<EntitySelectedEventData>(event.data);
		UpdateHeroCard(selectedEventData.id);
	});
	CEventDispatcher::GetInstance().Subscribe(EventType::FightActionSelected, [this](const SEvent& event) {
		const auto& fightActionSelectedEventData = std::get<FightActionSelectedEventData>(event.data);
		m_currentFightAction = fightActionSelectedEventData.selectedAction;
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
	if (currentState == CurrentState::Fight)
	{
		sf::View view = m_window.getDefaultView();
		m_window.setView(view);

		CViewSystem::DrawFightScene();
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
				CViewSystem::DrawMapItem(j, i, map, sf::IntRect(316, 15, 20, 20));
				break;
			}
			case '2': {
				CViewSystem::DrawMapItem(j, i, map, sf::IntRect(22, 78, 20, 20));
				break;
			}
			case '3': {
				CViewSystem::DrawMapItem(j, i, map, sf::IntRect(1, 78, 20, 20));
				break;
			}
			case '4': {
				CViewSystem::DrawMapItem(j, i, map, sf::IntRect(85, 78, 20, 20));
				break;
			}
			case '5': {
				CViewSystem::DrawMapItem(j, i, map, sf::IntRect(22, 36, 20, 20));
				break;
			}
			case '6': {
				CViewSystem::DrawMapItem(j, i, map, sf::IntRect(64, 57, 20, 20));
				break;
			}
			case '7': {
				CViewSystem::DrawMapItem(j, i, map, sf::IntRect(64, 36, 20, 20));
				break;
			}
			case '8': {
				CViewSystem::DrawMapItem(j, i, map, sf::IntRect(1, 57, 20, 20));
				break;
			}
			case '9': {
				CViewSystem::DrawMapItem(j, i, map, sf::IntRect(1, 36, 20, 20));
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
				switch (ch)
				{
				case '@': {
					CViewSystem::DrawMapItem(j, i, map, sf::IntRect(316, 78, 60, 60));
					break;
				}
				case '!': {
					CViewSystem::DrawMapItem(j, i, map, sf::IntRect(22, 36, 20, 20));
					CViewSystem::DrawMapItem(j, i, map, sf::IntRect(43, 36, 20, 20));
					break;
				}
				case 'S': {
					CViewSystem::DrawMapItem(j, i, map, sf::IntRect(1, 141, 191, 109));
					break;
				}
				case 'C': {
					CViewSystem::DrawMapItem(j, i, map, sf::IntRect(42, 190, 40, 60));
					break;
				}
				default:
					break;
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

	CViewSystem::UpdateHeroCardBackground(colorThemeComp);
	CViewSystem::UpdateHeroAvatar(avatarComp);
	CViewSystem::UpdateHeroHealthBar(healthComp, colorThemeComp);
	CViewSystem::UpdateHeroManaBar(manaComp);
	CViewSystem::UpdateHeroInfoText(experienceComp);
}

void CViewSystem::UpdateHeroCardBackground(const ColorThemeComponent* colorThemeComp)
{
	m_heroCard.shape.setSize({ 700, 200 });
	m_heroCard.shape.setFillColor(sf::Color::Black);
	m_heroCard.shape.setOutlineColor(colorThemeComp->colorTheme);
	m_heroCard.shape.setOutlineThickness(4);
}

void CViewSystem::UpdateHeroAvatar(const AvatarComponent* avatarComp)
{
	auto& texture = CTextureStorage::GetTexture(avatarComp->avatarFilePath);
	m_heroCard.avatarSprite.setTexture(texture);
	m_heroCard.avatarSprite.setScale(5.0f, 5.0f);
}

void CViewSystem::UpdateHeroHealthBar(const HealthComponent* healthComp, const ColorThemeComponent* colorThemeComp)
{
	const float maxHealthBarWidth = 200;
	m_heroCard.maxHealthBar.setSize({ maxHealthBarWidth, 20 });
	m_heroCard.maxHealthBar.setFillColor(sf::Color(117, 2, 7));

	m_heroCard.healthBar.setSize({ maxHealthBarWidth * (healthComp->currentHealth / healthComp->maxHealth), 20 });
	m_heroCard.healthBar.setFillColor(colorThemeComp->colorTheme);
}

void CViewSystem::UpdateHeroManaBar(const ManaComponent* manaComp)
{
	const float maxManaBarWidth = 200;
	m_heroCard.maxManaBar.setSize({ maxManaBarWidth, 20 });
	m_heroCard.maxManaBar.setFillColor(sf::Color(117, 2, 7));

	m_heroCard.manaBar.setSize({ maxManaBarWidth * (manaComp->currentMana / manaComp->maxMana), 20 });
	m_heroCard.manaBar.setFillColor(sf::Color(248, 158, 69));
}

void CViewSystem::UpdateHeroInfoText(const ExperienceComponent* experienceComp)
{
	auto& font = CFontStorage::GetFont("8bitoperator_jve.ttf");

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
	auto mainMenuSoulId = entityManager.GetEntitiesWithComponents<MenuSoulComponent>().front();
	auto* imageComp = entityManager.GetComponent<ImageComponent>(mainMenuSoulId);
	auto* positionComp = entityManager.GetComponent<PositionComponent>(mainMenuSoulId);

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
	CViewSystem::SetupPauseMenuArea();
	CViewSystem::SetupPauseMenuTitle();
	CViewSystem::SetupPauseMenuOptions();

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

void CViewSystem::SetupPauseMenuArea()
{
	m_pauseMenu.pauseMenuAreaTexture = CTextureStorage::GetTexture("utils_windows.png");
	m_pauseMenu.pauseMenuArea.setTexture(m_pauseMenu.pauseMenuAreaTexture);
	m_pauseMenu.pauseMenuArea.setTextureRect(sf::IntRect(841, 21, 260, 146));
	m_pauseMenu.pauseMenuArea.setPosition(m_window.getView().getCenter().x - 260.f * 3 / 2, m_window.getView().getCenter().y - 73.f * 3 / 2);
	m_pauseMenu.pauseMenuArea.setScale(3.f, 3.f);
}

void CViewSystem::SetupPauseMenuTitle()
{
	auto& font = CFontStorage::GetFont("8bitoperator_jve.ttf");
	m_pauseMenu.pauseMenuTitle.setFont(font);
	m_pauseMenu.pauseMenuTitle.setString("PAUSE");
	m_pauseMenu.pauseMenuTitle.setCharacterSize(40);

	const float titleX = m_pauseMenu.pauseMenuArea.getPosition().x + m_pauseMenu.pauseMenuArea.getScale().x * 260 / 2.2f;
	const float titleY = m_pauseMenu.pauseMenuArea.getPosition().y + 30;
	m_pauseMenu.pauseMenuTitle.setPosition(titleX, titleY);
}

void CViewSystem::SetupPauseMenuOptions()
{
	auto& font = CFontStorage::GetFont("8bitoperator_jve.ttf");

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
}

void CViewSystem::DrawLevelChoosingMenu()
{
	auto& font = CFontStorage::GetFont("8bitoperator_jve.ttf");

	DrawLevelChoosingTitle(font);
	DrawLevelChoosingBackground();

	if (m_levelChoosingMenu.levelCards.empty())
	{
		CreateLevelSaveCards();
	}

	DrawLevelSaveCards();
	DrawLevelSaveCardsInfo(font);
}

void CViewSystem::DrawLevelChoosingTitle(sf::Font& font)
{
	m_levelChoosingMenu.levelChoosingMenuTitle.setFont(font);
	m_levelChoosingMenu.levelChoosingMenuTitle.setString("Please select a file:");
	m_levelChoosingMenu.levelChoosingMenuTitle.setPosition(m_window.getView().getCenter().x - 289.f * 3 / 2, m_window.getView().getCenter().y - 225);
	m_levelChoosingMenu.levelChoosingMenuTitle.setCharacterSize(40);
	m_window.draw(m_levelChoosingMenu.levelChoosingMenuTitle);
}

void CViewSystem::DrawLevelChoosingBackground()
{
	m_levelChoosingMenu.backgroundTexture = CTextureStorage::GetTexture("menu_background.png");
	m_levelChoosingMenu.backgroundSprite.setTexture(m_levelChoosingMenu.backgroundTexture);
	m_window.draw(m_levelChoosingMenu.backgroundSprite);
}

void CViewSystem::CreateLevelSaveCards()
{
	sf::Sprite save1InfoCard, save2InfoCard, save3InfoCard;

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

void CViewSystem::DrawLevelSaveCards()
{
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
}

void CViewSystem::DrawLevelSaveCardsInfo(sf::Font& font)
{
	auto save1Info = CGameController::GetSaveInfo(0);
	auto save2Info = CGameController::GetSaveInfo(1);
	auto save3Info = CGameController::GetSaveInfo(2);

	if (m_levelChoosingMenu.levelCardsInfo.empty())
	{
		CreateLevelSaveCardInfo(0, save1Info, font);
		CreateLevelSaveCardInfo(1, save2Info, font);
		CreateLevelSaveCardInfo(2, save3Info, font);
	}

	for (const auto& levelCardInfo : m_levelChoosingMenu.levelCardsInfo)
	{
		for (const auto& text : levelCardInfo.second)
		{
			m_window.draw(text);
		}
	}
}

void CViewSystem::CreateLevelSaveCardInfo(int saveIndex, const SaveInfo& saveInfo, sf::Font& font)
{
	sf::Text playerName;
	playerName.setFont(font);
	playerName.setCharacterSize(50);
	playerName.setString(saveInfo.PlayerName);
	playerName.setPosition(m_levelChoosingMenu.levelCards[saveIndex].getPosition().x + 60, m_levelChoosingMenu.levelCards[saveIndex].getPosition().y + 15);

	sf::Text currentLocation;
	currentLocation.setFont(font);
	currentLocation.setCharacterSize(40);
	currentLocation.setString(saveInfo.currentLocation);
	currentLocation.setPosition(m_levelChoosingMenu.levelCards[saveIndex].getPosition().x + 60, m_levelChoosingMenu.levelCards[saveIndex].getPosition().y + 85);

	int minutes = static_cast<int>(saveInfo.gameTime) / 60;
	int seconds = static_cast<int>(saveInfo.gameTime) % 60;

	sf::Text passedTime;
	passedTime.setFont(font);
	passedTime.setCharacterSize(45);
	passedTime.setString("Time: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
	passedTime.setPosition(m_levelChoosingMenu.levelCards[saveIndex].getPosition().x + 200, m_levelChoosingMenu.levelCards[saveIndex].getPosition().y + 20);

	m_levelChoosingMenu.levelCardsInfo[saveIndex].push_back(playerName);
	m_levelChoosingMenu.levelCardsInfo[saveIndex].push_back(currentLocation);
	m_levelChoosingMenu.levelCardsInfo[saveIndex].push_back(passedTime);
}

void CViewSystem::DrawMapItem(int x, int y, const sf::Texture& texture, const sf::IntRect& textureRect)
{
	sf::Sprite mapItemSprite;
	mapItemSprite.setTexture(texture);
	mapItemSprite.setTextureRect(textureRect);
	mapItemSprite.setPosition(float(x) * 20 * 3 - 60, float(y) * 20 * 3 - 60);
	mapItemSprite.setScale(3.f, 3.f);
	m_window.draw(mapItemSprite);
}

void CViewSystem::DrawFightScene()
{
	const BattleAreaSettings battleAreaSettings;
	auto& heroCardFont = CFontStorage::GetFont("8bitoperator_jve.ttf");

	auto& entityManager = CEntityManager::GetInstance();
	auto charactersIds = entityManager.GetEntitiesWithComponents<NameComponent>();
	auto currentFightPhase = CGameController::GetFightPhase();

	m_fightingScene.backgroundTexture = CTextureStorage::GetTexture("fight_background.png");
	m_fightingScene.background.setTexture(m_fightingScene.backgroundTexture);
	m_fightingScene.background.setScale(6.f, 5.f);

	m_fightingScene.fightInfoCard.setTexture(CTextureStorage::GetTexture("utils_windows.png"));
	m_fightingScene.fightInfoCard.setTextureRect(sf::IntRect(544, 21, 289, 76));
	m_fightingScene.fightInfoCard.setPosition(float(m_window.getSize().x) / 2 - 289.f * 3 / 2, float(m_window.getSize().y) - 76.f * 3);
	m_fightingScene.fightInfoCard.setScale(3.f, 3.f);

	m_fightingScene.fightInfoText.setFont(heroCardFont);
	m_fightingScene.fightInfoText.setCharacterSize(40);

	switch (m_currentFightAction)
	{
	case FightAction::Info: {
		m_fightingScene.fightInfoText.setString("* Bimbim and Bambam blocked the way!");
		m_fightingScene.fightInfoText.setPosition(m_fightingScene.fightInfoCard.getPosition().x + 50, m_fightingScene.fightInfoCard.getPosition().y + 76.f / 2);
		break;
	}
	case FightAction::Attack: {
		m_fightingScene.fightInfoText.setString("* Attacking!");
		m_fightingScene.fightInfoText.setPosition(m_fightingScene.fightInfoCard.getPosition().x + 50, m_fightingScene.fightInfoCard.getPosition().y + 76.f / 2);
		break;
	}
	case FightAction::Act: {
		m_fightingScene.fightInfoText.setString("* Acting!");
		m_fightingScene.fightInfoText.setPosition(m_fightingScene.fightInfoCard.getPosition().x + 50, m_fightingScene.fightInfoCard.getPosition().y + 76.f / 2);
		break;
	}
	case FightAction::Inventory: {
		m_fightingScene.fightInfoText.setString("* Inventory!");
		m_fightingScene.fightInfoText.setPosition(m_fightingScene.fightInfoCard.getPosition().x + 50, m_fightingScene.fightInfoCard.getPosition().y + 76.f / 2);
		break;
	}
	case FightAction::Spare: {
		m_fightingScene.fightInfoText.setString("* Spare!");
		m_fightingScene.fightInfoText.setPosition(m_fightingScene.fightInfoCard.getPosition().x + 50, m_fightingScene.fightInfoCard.getPosition().y + 76.f / 2);
		break;
	}
	case FightAction::Magic: {
		break;
	}
	}

	m_fightingScene.battleArea.setTexture(CTextureStorage::GetTexture("fight_utils.png", sf::Color(0, 148, 255)));
	m_fightingScene.battleArea.setTextureRect(sf::IntRect(227, 568, battleAreaSettings.areaWidth, battleAreaSettings.areaHeight));
	m_fightingScene.battleArea.setPosition(float(m_window.getSize().x) / 2 - 150, float(m_window.getSize().y) / 2 - 200);
	m_fightingScene.battleArea.setScale(2.f, 2.f);

	auto fightSoul = entityManager.GetEntitiesWithComponents<FightSoulComponent>().front();
	auto soulPositionComp = entityManager.GetComponent<PositionComponent>(fightSoul);

	m_fightingScene.heroSoul.setTexture(CTextureStorage::GetTexture("menu_soul.png"));
	m_fightingScene.heroSoul.setPosition(soulPositionComp->x, soulPositionComp->y);
	m_fightingScene.heroSoul.setScale(0.125f, 0.125f);

	float totalWidth = 289.f * 3;
	float cardWidth = totalWidth / float(charactersIds.size()) - 10;

	float xOffset = 0.f;
	float spriteYOffset = 0.f;
	const float maxHealthBarWidth = 150;

	m_window.draw(m_fightingScene.background);
	m_window.draw(m_fightingScene.fightInfoCard);
	m_window.draw(m_fightingScene.fightInfoText);

	if (currentFightPhase == FightPhase::EnemiesTurn)
	{
		m_window.draw(m_fightingScene.battleArea);
		m_window.draw(m_fightingScene.heroSoul);
	}

	for (auto id : charactersIds)
	{
		auto fightTurnComp = entityManager.GetComponent<FightTurnComponent>(id);
		auto colorThemeComp = entityManager.GetComponent<ColorThemeComponent>(id);
		auto avatarComp = entityManager.GetComponent<AvatarComponent>(id);
		auto nameComp = entityManager.GetComponent<NameComponent>(id);
		auto healthComp = entityManager.GetComponent<HealthComponent>(id);
		auto animComp = entityManager.GetComponent<AnimationComponent>(id);

		HeroFightCard heroFightCard;
		heroFightCard.area.setSize({ cardWidth, 70.f });
		heroFightCard.area.setPosition({ m_fightingScene.fightInfoCard.getPosition().x + xOffset,
			m_fightingScene.fightInfoCard.getPosition().y - 70 });
		heroFightCard.area.setFillColor(sf::Color::Black);

		if (fightTurnComp->isEntityTurn)
		{
			heroFightCard.area.setPosition({ m_fightingScene.fightInfoCard.getPosition().x + xOffset,
				m_fightingScene.fightInfoCard.getPosition().y - 140 });

			heroFightCard.area.setOutlineColor(colorThemeComp->colorTheme);
			heroFightCard.area.setOutlineThickness(2);

			heroFightCard.actionsArea.setPosition({ m_fightingScene.fightInfoCard.getPosition().x + xOffset,
				m_fightingScene.fightInfoCard.getPosition().y - 70 });
			heroFightCard.actionsArea.setSize({ cardWidth, 70.f });
			heroFightCard.actionsArea.setFillColor(sf::Color::Black);
			heroFightCard.actionsArea.setOutlineColor(colorThemeComp->colorTheme);
			heroFightCard.actionsArea.setOutlineThickness(2);

			if (heroFightCard.actions.empty())
			{
				sf::Sprite attackAction, actAction, itemAction, spareAction, magicAction;

				attackAction.setTexture(CTextureStorage::GetTexture("fight_utils.png", sf::Color(0, 148, 255)));
				attackAction.setTextureRect(sf::IntRect(1183, 326, 30, 26));
				attackAction.setPosition(heroFightCard.actionsArea.getPosition().x + 50, heroFightCard.actionsArea.getPosition().y + 10);
				attackAction.setScale(1.5f, 1.5f);
				heroFightCard.actions.push_back(attackAction);

				actAction.setTexture(CTextureStorage::GetTexture("fight_utils.png", sf::Color(0, 148, 255)));
				actAction.setTextureRect(sf::IntRect(1214, 326, 31, 26));
				actAction.setPosition(heroFightCard.actionsArea.getPosition().x + 120, heroFightCard.actionsArea.getPosition().y + 10);
				actAction.setScale(1.5f, 1.5f);
				heroFightCard.actions.push_back(actAction);

				itemAction.setTexture(CTextureStorage::GetTexture("fight_utils.png", sf::Color(0, 148, 255)));
				itemAction.setTextureRect(sf::IntRect(1247, 326, 30, 26));
				itemAction.setPosition(heroFightCard.actionsArea.getPosition().x + 190, heroFightCard.actionsArea.getPosition().y + 10);
				itemAction.setScale(1.5f, 1.5f);
				heroFightCard.actions.push_back(itemAction);

				spareAction.setTexture(CTextureStorage::GetTexture("fight_utils.png", sf::Color(0, 148, 255)));
				spareAction.setTextureRect(sf::IntRect(1279, 326, 30, 26));
				spareAction.setPosition(heroFightCard.actionsArea.getPosition().x + 260, heroFightCard.actionsArea.getPosition().y + 10);
				spareAction.setScale(1.5f, 1.5f);
				heroFightCard.actions.push_back(spareAction);

				magicAction.setTexture(CTextureStorage::GetTexture("fight_utils.png", sf::Color(0, 148, 255)));
				magicAction.setTextureRect(sf::IntRect(1343, 326, 30, 26));
				magicAction.setPosition(heroFightCard.actionsArea.getPosition().x + 330, heroFightCard.actionsArea.getPosition().y + 10);
				magicAction.setScale(1.5f, 1.5f);
				heroFightCard.actions.push_back(magicAction);
			}
		}

		heroFightCard.heroIcon.setTexture(CTextureStorage::GetTexture(avatarComp->avatarFilePath));
		heroFightCard.heroIcon.setPosition(heroFightCard.area.getPosition().x + 20, heroFightCard.area.getPosition().y + 10);
		heroFightCard.heroIcon.setScale(2.25f, 2.25f);

		heroFightCard.heroName.setFont(heroCardFont);
		heroFightCard.heroName.setString(nameComp->name);
		heroFightCard.heroName.setCharacterSize(30);
		heroFightCard.heroName.setPosition(heroFightCard.area.getPosition().x + 90, heroFightCard.area.getPosition().y + 20);

		heroFightCard.heroMaxHealthBar.setSize({ maxHealthBarWidth, 20 });
		heroFightCard.heroMaxHealthBar.setPosition({ heroFightCard.area.getPosition().x + 250, heroFightCard.area.getPosition().y + 35 });
		heroFightCard.heroMaxHealthBar.setFillColor(sf::Color(117, 2, 7));

		heroFightCard.heroHealthBar.setSize({ maxHealthBarWidth * (healthComp->currentHealth / healthComp->maxHealth), 20 });
		heroFightCard.heroHealthBar.setPosition(heroFightCard.heroMaxHealthBar.getPosition().x, heroFightCard.heroMaxHealthBar.getPosition().y);
		heroFightCard.heroHealthBar.setFillColor(colorThemeComp->colorTheme);

		heroFightCard.heroHealthText.setFont(heroCardFont);
		heroFightCard.heroHealthText.setString("HP");
		heroFightCard.heroHealthText.setCharacterSize(28);
		heroFightCard.heroHealthText.setPosition({ heroFightCard.area.getPosition().x + 215, heroFightCard.area.getPosition().y + 25 });

		heroFightCard.heroHealthValue.setFont(heroCardFont);
		heroFightCard.heroHealthValue.setString(std::to_string(int(healthComp->currentHealth)) + "/  " + std::to_string(int(healthComp->maxHealth)));
		heroFightCard.heroHealthValue.setCharacterSize(28);
		heroFightCard.heroHealthValue.setPosition({ heroFightCard.area.getPosition().x + 315, heroFightCard.area.getPosition().y - 5 });

		if (animComp)
		{
			animComp->sprite.setPosition(300, 350 + spriteYOffset);
			animComp->sprite.setScale(4.f, 4.f);
			m_window.draw(animComp->sprite);
		}

		m_fightingScene.heroesFightInfo[id] = heroFightCard;
		xOffset += cardWidth + 20;
		spriteYOffset += 150;
	}

	for (const auto& info : m_fightingScene.heroesFightInfo)
	{
		m_window.draw(info.second.area);
		m_window.draw(info.second.heroIcon);
		m_window.draw(info.second.actionsArea);
		m_window.draw(info.second.heroName);
		m_window.draw(info.second.heroMaxHealthBar);
		m_window.draw(info.second.heroHealthBar);
		m_window.draw(info.second.heroHealthText);
		m_window.draw(info.second.heroHealthValue);

		for (int i = 0; i < info.second.actions.size(); i++)
		{
			auto action = info.second.actions[i];
			const sf::IntRect spriteBounds = action.getTextureRect();
			if (CGameController::GetCurrentFightActionNumber() == i)
			{
				action.setTextureRect(sf::IntRect(
					spriteBounds.left,
					261,
					spriteBounds.width,
					spriteBounds.height + 5));
			}

			m_window.draw(action);
		}
	}
}