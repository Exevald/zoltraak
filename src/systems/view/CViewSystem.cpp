#include "CViewSystem.h"
#include "CEntityManager.h"
#include "Utils.h"
#include "assets_storage/CFontStorage.h"
#include "assets_storage/CTextureStorage.h"
#include "events/CEventDispatcher.h"
#include <iostream>

static auto& font = CFontStorage::GetFont("8bitoperator_jve.ttf");

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

	CEventDispatcher::GetInstance().Subscribe(EventType::ActiveFightCharacterUpdated, [this](const SEvent& event) {
		const auto& activeFightCharacterUpdatedEventData = std::get<ActiveFightCharacterUpdatedEventData>(event.data);
		CViewSystem::UpdateActiveFightUserInfo(activeFightCharacterUpdatedEventData.id);
	});

	CEventDispatcher::GetInstance().Subscribe(EventType::InventoryActionChanged, [this](const SEvent& event) {
		const auto& inventoryActionChangedEventData = std::get<InventoryActionChangedEventData>(event.data);
		m_currentInventoryItemAction = inventoryActionChangedEventData.selectedAction;
	});

	CEventDispatcher::GetInstance().Subscribe(EventType::InventoryStateChanged, [this](const SEvent& event) {
		const auto& inventoryStateChangedEventData = std::get<InventoryStateChangedEventData>(event.data);
		m_currentInventoryState = inventoryStateChangedEventData.changedState;
	});

	CEventDispatcher::GetInstance().Subscribe(EventType::InventoryItemUsed, [this](const SEvent& event) {
		CViewSystem::UpdateInventoryItems();
	});

	for (const auto& renderer : m_renderers)
	{
		renderer->Init();
	}
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

		if (m_heroCard.isVisible)
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
	if (currentState == CurrentState::Inventory)
	{
		sf::View view = m_window.getDefaultView();
		m_window.setView(view);

		CViewSystem::DrawInventory();
	}

	for (const auto& renderer : m_renderers)
	{
		renderer->Draw(m_window);
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
		if (entityManager.HasComponent<MenuSoulComponent>(entityId) || entityManager.HasComponent<FightSoulComponent>(entityId))
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
		m_heroCard.isVisible = false;
		return;
	}

	m_heroCard.isVisible = true;

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
	m_pauseMenu.pauseMenuTitle.setFont(font);
	m_pauseMenu.pauseMenuTitle.setString("PAUSE");
	m_pauseMenu.pauseMenuTitle.setCharacterSize(40);

	const float titleX = m_pauseMenu.pauseMenuArea.getPosition().x + m_pauseMenu.pauseMenuArea.getScale().x * 260 / 2.2f;
	const float titleY = m_pauseMenu.pauseMenuArea.getPosition().y + 30;
	m_pauseMenu.pauseMenuTitle.setPosition(titleX, titleY);
}

void CViewSystem::SetupPauseMenuOptions()
{
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
	DrawLevelChoosingTitle();
	DrawLevelChoosingBackground();

	if (m_levelChoosingMenu.levelCards.empty())
	{
		CreateLevelSaveCards();
	}

	DrawLevelSaveCards();
	DrawLevelSaveCardsInfo();
}

void CViewSystem::DrawLevelChoosingTitle()
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

void CViewSystem::DrawLevelSaveCardsInfo()
{
	auto save1Info = CGameController::GetSaveInfo(0);
	auto save2Info = CGameController::GetSaveInfo(1);
	auto save3Info = CGameController::GetSaveInfo(2);

	if (m_levelChoosingMenu.levelCardsInfo.empty())
	{
		CreateLevelSaveCardInfo(0, save1Info);
		CreateLevelSaveCardInfo(1, save2Info);
		CreateLevelSaveCardInfo(2, save3Info);
	}

	for (const auto& levelCardInfo : m_levelChoosingMenu.levelCardsInfo)
	{
		for (const auto& text : levelCardInfo.second)
		{
			m_window.draw(text);
		}
	}
}

void CViewSystem::CreateLevelSaveCardInfo(int saveIndex, const SaveInfo& saveInfo)
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
	auto& entityManager = CEntityManager::GetInstance();

	auto charactersIds = entityManager.GetEntitiesWithComponents<NameComponent>();
	auto currentFightPhase = CGameController::GetCurrentFightPhase();

	m_fightingScene.backgroundTexture = CTextureStorage::GetTexture("fight_background.png");
	m_fightingScene.background.setTexture(m_fightingScene.backgroundTexture);
	m_fightingScene.background.setScale(6.f, 5.f);

	m_fightingScene.fightInfoCard.setTexture(CTextureStorage::GetTexture("utils_windows.png"));
	m_fightingScene.fightInfoCard.setTextureRect(sf::IntRect(544, 21, 289, 76));
	m_fightingScene.fightInfoCard.setPosition(float(m_window.getSize().x) / 2 - 289.f * 3 / 2, float(m_window.getSize().y) - 76.f * 3);
	m_fightingScene.fightInfoCard.setScale(3.f, 3.f);

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

	switch (m_currentFightAction)
	{
	case FightAction::Info: {
		CViewSystem::SetFightInfo();
		break;
	}
	case FightAction::Attack: {
		m_isAttackBarActive = true;
		CViewSystem::SetAttackBar(m_fightingScene.fightInfoCard.getPosition());
		break;
	}
	case FightAction::Act: {
		CViewSystem::SetActActions();
		break;
	}
	case FightAction::Inventory: {
		CViewSystem::SetFightInventory();
		break;
	}
	case FightAction::Spare: {
		CViewSystem::SetSpareText();
		break;
	}
	case FightAction::Magic: {
		CViewSystem::SetMagicText();
		break;
	}
	}

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

		HeroInfoCard heroFightCard;
		heroFightCard.area.setSize({ cardWidth, 70.f });
		heroFightCard.area.setPosition({ m_fightingScene.fightInfoCard.getPosition().x + xOffset,
			m_fightingScene.fightInfoCard.getPosition().y - 70 });
		heroFightCard.area.setFillColor(sf::Color::Black);

		if (fightTurnComp->isHeroTurn)
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

		heroFightCard.heroName.setFont(font);
		heroFightCard.heroName.setString(nameComp->name);
		heroFightCard.heroName.setCharacterSize(30);
		heroFightCard.heroName.setPosition(heroFightCard.area.getPosition().x + 90, heroFightCard.area.getPosition().y + 20);

		heroFightCard.heroMaxHealthBar.setSize({ maxHealthBarWidth, 20 });
		heroFightCard.heroMaxHealthBar.setPosition({ heroFightCard.area.getPosition().x + 250, heroFightCard.area.getPosition().y + 35 });
		heroFightCard.heroMaxHealthBar.setFillColor(sf::Color(117, 2, 7));

		heroFightCard.heroHealthBar.setSize({ maxHealthBarWidth * (healthComp->currentHealth / healthComp->maxHealth), 20 });
		heroFightCard.heroHealthBar.setPosition(heroFightCard.heroMaxHealthBar.getPosition().x, heroFightCard.heroMaxHealthBar.getPosition().y);
		heroFightCard.heroHealthBar.setFillColor(colorThemeComp->colorTheme);

		heroFightCard.heroHealthText.setFont(font);
		heroFightCard.heroHealthText.setString("HP");
		heroFightCard.heroHealthText.setCharacterSize(28);
		heroFightCard.heroHealthText.setPosition({ heroFightCard.area.getPosition().x + 215, heroFightCard.area.getPosition().y + 25 });

		heroFightCard.heroHealthValue.setFont(font);
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

void CViewSystem::UpdateActiveFightUserInfo(EntityId entity)
{
	auto& entityManager = CEntityManager::GetInstance();
	auto inventoryComp = entityManager.GetComponent<InventoryComponent>(entity);

	if (!inventoryComp)
	{
		return;
	}

	m_activeFightHero = entity;
}

void CViewSystem::SetAttackBar(const sf::Vector2f& fightInfoCardPosition)
{
	static sf::RectangleShape attackBar;
	static sf::RectangleShape movingIndicator;
	static float indicatorSpeed = 500.f;
	static float barWidth = 300.f;
	static float barHeight = 50.f;

	auto& entityManager = CEntityManager::GetInstance();
	auto colorThemeComp = entityManager.GetComponent<ColorThemeComponent>(m_activeFightHero);

	if (!m_isAttackBarActive)
	{
		return;
	}

	if (attackBar.getSize().x == 0)
	{
		attackBar.setSize({ barWidth, barHeight });
		attackBar.setFillColor(sf::Color::Black);
		attackBar.setOutlineColor(colorThemeComp->colorTheme);
		attackBar.setOutlineThickness(2);
		attackBar.setPosition(
			fightInfoCardPosition.x + 20,
			fightInfoCardPosition.y + barHeight / 2);

		movingIndicator.setSize({ 10.f, barHeight });
		movingIndicator.setFillColor(sf::Color::White);
		movingIndicator.setPosition(attackBar.getPosition());
	}

	movingIndicator.move(indicatorSpeed * CGameController::GetDeltaTime(), 0);
	if (movingIndicator.getPosition().x > attackBar.getPosition().x + barWidth || movingIndicator.getPosition().x < attackBar.getPosition().x)
	{
		indicatorSpeed = -indicatorSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_isAttackBarActive = false;

		float indicatorPos = movingIndicator.getPosition().x - attackBar.getPosition().x;
		float damageMultiplier = 0.5f;

		if (indicatorPos > barWidth * 0.4f && indicatorPos < barWidth * 0.6f)
		{
			damageMultiplier = 2.f;
		}
		else if (indicatorPos > barWidth * 0.3f && indicatorPos < barWidth * 0.7f)
		{
			damageMultiplier = 1.f;
		}

		std::cout << "Damage multiplier: " << damageMultiplier << std::endl;
		m_currentFightAction = FightAction::Info;
	}

	m_window.draw(attackBar);
	m_window.draw(movingIndicator);
}

void CViewSystem::SetActActions()
{
	auto& entityManager = CEntityManager::GetInstance();
	std::cout << "Act\n";
}

void CViewSystem::SetFightInventory()
{
	auto& entityManager = CEntityManager::GetInstance();
	std::cout << "Inventory\n";
}

void CViewSystem::SetFightInfo()
{
	auto& entityManager = CEntityManager::GetInstance();
	std::cout << "Fight info\n";
}

void CViewSystem::SetSpareText()
{
	auto& entityManager = CEntityManager::GetInstance();
	std::cout << "Spare\n";
}

void CViewSystem::SetMagicText()
{
	auto& entityManager = CEntityManager::GetInstance();
	std::cout << "Magic\n";
}

void CViewSystem::DrawInventory()
{
	auto& entityManager = CEntityManager::GetInstance();

	auto characters = entityManager.GetEntitiesWithComponents<NameComponent>();
	auto currentInventorySectionNumber = CGameController::GetCurrentInventorySectionNumber();
	auto entityWithMoneyId = entityManager.GetEntitiesWithComponents<MoneyComponent>().front();
	auto moneyComp = entityManager.GetComponent<MoneyComponent>(entityWithMoneyId);

	m_inventory.sectionTitle.setTexture(CTextureStorage::GetTexture("utils_windows.png"));
	m_inventory.sectionTitle.setPosition(float(m_window.getSize().x) / 5, 50);
	m_inventory.sectionTitle.setScale(3.f, 3.f);

	m_inventory.money.setFont(font);
	m_inventory.money.setString("MONEY:   $ " + std::to_string(moneyComp->money));
	m_inventory.money.setCharacterSize(50);
	m_inventory.money.setPosition(float(m_window.getSize().x) * 3 / 5 + 100, 50);

	if (m_inventory.inventorySections.empty())
	{
		sf::Sprite itemsInventorySection;
		itemsInventorySection.setTexture(CTextureStorage::GetTexture("utils_windows.png"));
		itemsInventorySection.setTextureRect(sf::IntRect(80, 75, 33, 24));
		itemsInventorySection.setPosition(float(m_window.getSize().x) / 5 + 250, 45);
		itemsInventorySection.setScale(3.f, 3.f);

		sf::Sprite equipInventorySection;
		equipInventorySection.setTexture(CTextureStorage::GetTexture("utils_windows.png"));
		equipInventorySection.setTextureRect(sf::IntRect(80, 100, 33, 24));
		equipInventorySection.setPosition(itemsInventorySection.getPosition().x + 200, 45);
		equipInventorySection.setScale(3.f, 3.f);

		sf::Sprite powerInventorySection;
		powerInventorySection.setTexture(CTextureStorage::GetTexture("utils_windows.png"));
		powerInventorySection.setTextureRect(sf::IntRect(80, 125, 33, 24));
		powerInventorySection.setPosition(equipInventorySection.getPosition().x + 200, 45);
		powerInventorySection.setScale(3.f, 3.f);

		m_inventory.inventorySections.push_back(itemsInventorySection);
		m_inventory.inventorySections.push_back(equipInventorySection);
		m_inventory.inventorySections.push_back(powerInventorySection);
	}

	m_inventory.inventoryMenu.setTexture(CTextureStorage::GetTexture("utils_windows.png"));
	m_inventory.inventoryMenu.setPosition(float(m_window.getSize().x) / 4.5f, 200);
	m_inventory.inventoryMenu.setScale(4.f, 4.f);

	m_inventory.inventoryMenuSoul.setTexture(CTextureStorage::GetTexture("menu_soul.png"));
	m_inventory.inventoryMenuSoul.setScale(0.125f, 0.125f);

	auto activeCharacterNameComp = entityManager.GetComponent<NameComponent>(CGameController::GetActiveInventoryCharacterNumber());
	m_inventory.heroName.setFont(font);
	m_inventory.heroName.setCharacterSize(45);
	m_inventory.heroName.setString(activeCharacterNameComp->name);
	m_inventory.heroName.setPosition(m_inventory.inventoryMenu.getPosition().x + 180, m_inventory.inventoryMenu.getPosition().y + 60);

	switch (currentInventorySectionNumber)
	{
	case 0: {
		m_inventory.sectionTitle.setTextureRect(sf::IntRect(117, 78, 35, 18));
		m_inventory.inventoryMenu.setTextureRect(sf::IntRect(841, 21, 260, 146));
		if (m_currentInventoryState == InventoryState::ItemActionSelection || m_currentInventoryState == InventoryState::ItemSelection)
		{
			if (m_currentInventoryItemAction == InventoryAction::Use)
			{
				m_inventory.inventoryMenuSoul.setPosition(m_inventory.inventoryMenu.getPosition().x + 310, m_inventory.inventoryMenu.getPosition().y + 92);
			}
			if (m_currentInventoryItemAction == InventoryAction::Destroy)
			{
				m_inventory.inventoryMenuSoul.setPosition(m_inventory.inventoryMenu.getPosition().x + 530, m_inventory.inventoryMenu.getPosition().y + 92);
			}
		}
		m_inventory.heroName.setPosition(-200, -200);
		break;
	}
	case 1: {
		m_inventory.sectionTitle.setTextureRect(sf::IntRect(117, 103, 35, 18));
		m_inventory.inventoryMenu.setTextureRect(sf::IntRect(841, 171, 270, 170));
		m_inventory.inventoryMenuSoul.setPosition(-200, -200);
		break;
	}
	case 2: {
		m_inventory.sectionTitle.setTextureRect(sf::IntRect(117, 128, 35, 18));
		m_inventory.inventoryMenu.setTextureRect(sf::IntRect(841, 346, 270, 170));
		m_inventory.inventoryMenuSoul.setPosition(-200, -200);
		break;
	}
	default:
		break;
	}

	if (m_inventory.inventoryItemMenuActions.empty())
	{
		sf::Text useInventoryItemText;
		useInventoryItemText.setFont(font);
		useInventoryItemText.setCharacterSize(50);
		useInventoryItemText.setString("USE");
		useInventoryItemText.setPosition(m_inventory.inventoryMenu.getPosition().x + 350, m_inventory.inventoryMenu.getPosition().y + 70);

		sf::Text destroyInventoryItemText;
		destroyInventoryItemText.setFont(font);
		destroyInventoryItemText.setCharacterSize(50);
		destroyInventoryItemText.setString("DESTROY");
		destroyInventoryItemText.setPosition(m_inventory.inventoryMenu.getPosition().x + 570, m_inventory.inventoryMenu.getPosition().y + 70);

		m_inventory.inventoryItemMenuActions.push_back(useInventoryItemText);
		m_inventory.inventoryItemMenuActions.push_back(destroyInventoryItemText);
	}

	float offsetX = 0;
	if (m_inventory.heroIcons.empty())
	{
		auto charactersWithIcons = entityManager.GetEntitiesWithComponents<AvatarComponent>();
		std::sort(charactersWithIcons.begin(), charactersWithIcons.end());

		for (const auto& characterId : charactersWithIcons)
		{
			auto characterIconComp = entityManager.GetComponent<AvatarComponent>(characterId);
			if (!characterIconComp)
			{
				continue;
			}
			sf::Sprite heroIcon;
			heroIcon.setTexture(CTextureStorage::GetTexture(characterIconComp->avatarFilePath));
			heroIcon.setPosition(m_inventory.inventoryMenu.getPosition().x + 110 + offsetX, m_inventory.inventoryMenu.getPosition().y + 160);
			heroIcon.setScale(3.5f, 3.5f);
			heroIcon.setColor(sf::Color(255, 255, 255, 80));

			m_inventory.heroIcons[characterId] = heroIcon;
			offsetX += 150;
		}
	}

	const float totalWidth = 289 * 3;
	const float cardWidth = totalWidth / float(characters.size()) - 10;
	float xOffset = 0;
	const float maxHealthBarWidth = 150;

	for (auto id : characters)
	{
		auto colorThemeComp = entityManager.GetComponent<ColorThemeComponent>(id);
		auto avatarComp = entityManager.GetComponent<AvatarComponent>(id);
		auto nameComp = entityManager.GetComponent<NameComponent>(id);
		auto healthComp = entityManager.GetComponent<HealthComponent>(id);

		HeroInfoCard heroFightCard;
		heroFightCard.area.setSize({ cardWidth, 70.f });
		heroFightCard.area.setPosition({ m_inventory.inventoryMenu.getPosition().x + 80 + xOffset,
			m_inventory.inventoryMenu.getPosition().y + 720 });
		heroFightCard.area.setFillColor(sf::Color::Black);
		heroFightCard.area.setOutlineThickness(2);
		heroFightCard.area.setOutlineColor(colorThemeComp->colorTheme);

		heroFightCard.heroIcon.setTexture(CTextureStorage::GetTexture(avatarComp->avatarFilePath));
		heroFightCard.heroIcon.setPosition(heroFightCard.area.getPosition().x + 20, heroFightCard.area.getPosition().y + 10);
		heroFightCard.heroIcon.setScale(2.25f, 2.25f);

		heroFightCard.heroName.setFont(font);
		heroFightCard.heroName.setString(nameComp->name);
		heroFightCard.heroName.setCharacterSize(30);
		heroFightCard.heroName.setPosition(heroFightCard.area.getPosition().x + 90, heroFightCard.area.getPosition().y + 20);

		heroFightCard.heroMaxHealthBar.setSize({ maxHealthBarWidth, 20 });
		heroFightCard.heroMaxHealthBar.setPosition({ heroFightCard.area.getPosition().x + 250, heroFightCard.area.getPosition().y + 35 });
		heroFightCard.heroMaxHealthBar.setFillColor(sf::Color(117, 2, 7));

		heroFightCard.heroHealthBar.setSize({ maxHealthBarWidth * (healthComp->currentHealth / healthComp->maxHealth), 20 });
		heroFightCard.heroHealthBar.setPosition(heroFightCard.heroMaxHealthBar.getPosition().x, heroFightCard.heroMaxHealthBar.getPosition().y);
		heroFightCard.heroHealthBar.setFillColor(colorThemeComp->colorTheme);

		heroFightCard.heroHealthText.setFont(font);
		heroFightCard.heroHealthText.setString("HP");
		heroFightCard.heroHealthText.setCharacterSize(28);
		heroFightCard.heroHealthText.setPosition({ heroFightCard.area.getPosition().x + 215, heroFightCard.area.getPosition().y + 25 });

		heroFightCard.heroHealthValue.setFont(font);
		heroFightCard.heroHealthValue.setString(std::to_string(int(healthComp->currentHealth)) + "/  " + std::to_string(int(healthComp->maxHealth)));
		heroFightCard.heroHealthValue.setCharacterSize(28);
		heroFightCard.heroHealthValue.setPosition({ heroFightCard.area.getPosition().x + 315, heroFightCard.area.getPosition().y - 5 });

		m_inventory.heroesInfo[id] = heroFightCard;
		xOffset += cardWidth + 50;
	}

	CViewSystem::UpdateInventoryItems();
	CViewSystem::UpdateHeroStats();
	CViewSystem::UpdateHeroEquipment();

	for (int i = 0; i < m_inventory.inventorySections.size(); i++)
	{
		auto section = m_inventory.inventorySections[i];
		const sf::IntRect spriteBounds = section.getTextureRect();
		int newSpiteLeftBound = spriteBounds.left;
		if (CGameController::GetCurrentInventorySectionNumber() == i)
		{
			newSpiteLeftBound -= 37;
			if (CGameController::GetCurrentInventoryMenuPosition() == 0)
			{
				newSpiteLeftBound -= 37;
			}
		}

		section.setTextureRect(sf::IntRect(
			newSpiteLeftBound,
			spriteBounds.top,
			spriteBounds.width,
			spriteBounds.height));

		m_window.draw(section);
	}

	m_window.draw(m_inventory.inventoryMenu);
	m_window.draw(m_inventory.sectionTitle);
	m_window.draw(m_inventory.money);

	if (m_currentInventoryState == InventoryState::ItemActionSelection)
	{
		m_window.draw(m_inventory.inventoryMenuSoul);
	}

	if (currentInventorySectionNumber == 0)
	{
		for (const auto& inventoryItemMenuAction : m_inventory.inventoryItemMenuActions)
		{
			m_window.draw(inventoryItemMenuAction);
		}

		for (int i = 0; i < m_inventory.inventoryItems.size(); i++)
		{
			auto inventoryItem = m_inventory.inventoryItems[i];
			if (m_currentInventoryState == InventoryState::ItemSelection)
			{
				if (CGameController::GetSelectedInventoryItemNumber() == i)
				{
					inventoryItem.setFillColor(sf::Color::Yellow);
				}
			}

			m_window.draw(inventoryItem);
		}
	}

	if (currentInventorySectionNumber == 1 || currentInventorySectionNumber == 2)
	{
		m_window.draw(m_inventory.heroName);

		for (const auto& statText : m_inventory.heroStats)
		{
			m_window.draw(statText);
		}

		for (auto [characterId, icon] : m_inventory.heroIcons)
		{
			if (characterId == CGameController::GetActiveInventoryCharacterNumber())
			{
				icon.setScale(4.f, 4.f);
				icon.setColor(sf::Color(255, 255, 255, 255));
			}
			m_window.draw(icon);
		}
	}

	if (currentInventorySectionNumber == 1)
	{
		m_window.draw(m_inventory.activeWeaponInfo);
		m_window.draw(m_inventory.activeShieldInfo);

		switch (CGameController::GetCurrentEquipmentType())
		{
		case EquipmentType::Weapon: {
			for (int i = 0; i < m_inventory.characterWeapons.size(); i++)
			{
				auto weapon = m_inventory.characterWeapons[i];
				if (i == CGameController::GetCurrentEquipmentItemNumber() && m_currentInventoryState == InventoryState::EquipmentItemSelection)
				{
					weapon.setFillColor(sf::Color::Yellow);
				}

				m_window.draw(weapon);
			}
			break;
		}
		case EquipmentType::Shield: {
			for (int i = 0; i < m_inventory.characterShields.size(); i++)
			{
				auto shield = m_inventory.characterShields[i];
				if (i == CGameController::GetCurrentEquipmentItemNumber() && m_currentInventoryState == InventoryState::EquipmentItemSelection)
				{
					shield.setFillColor(sf::Color::Yellow);
				}

				m_window.draw(shield);
			}
			break;
		}
		}
	}

	if (currentInventorySectionNumber == 2)
	{
		CViewSystem::DrawHeroInfo();
		CViewSystem::DrawHeroSpells();
	}

	for (const auto& info : m_inventory.heroesInfo)
	{
		m_window.draw(info.second.area);
		m_window.draw(info.second.heroIcon);
		m_window.draw(info.second.heroName);
		m_window.draw(info.second.heroMaxHealthBar);
		m_window.draw(info.second.heroHealthBar);
		m_window.draw(info.second.heroHealthText);
		m_window.draw(info.second.heroHealthValue);
	}
}

void CViewSystem::UpdateInventoryItems()
{
	float row = 0;
	float column = 0;
	int itemIndex = 0;
	m_inventory.inventoryItems.clear();
	int inventoryItemsSize = 0;
	std::vector<InventoryItem> commonHeroesInventory;

	for (const auto& [entityId, charactersInventory] : CGameController::GetAllHeroesInventory())
	{
		for (const auto& item : charactersInventory)
		{
			if (item.type != ItemType::Weapon && item.type != ItemType::Shield)
			{
				commonHeroesInventory.push_back(item);
			}
		}
	}

	for (const auto& inventoryItem : commonHeroesInventory)
	{
		sf::Text inventoryItemText;
		inventoryItemText.setFont(font);
		inventoryItemText.setString(inventoryItem.name);
		inventoryItemText.setCharacterSize(45);
		inventoryItemText.setPosition(m_inventory.inventoryMenu.getPosition().x + 200 + column * 370, m_inventory.inventoryMenu.getPosition().y + 150 + row * 50);
		inventoryItemText.setFillColor(sf::Color(125, 125, 125));

		m_inventory.inventoryItems[itemIndex] = inventoryItemText;

		row++;
		itemIndex++;
		inventoryItemsSize++;
		if (row > 5)
		{
			row = 0;
			column++;
		}
	}
}

void CViewSystem::UpdateHeroStats()
{
	m_inventory.heroStats.clear();

	auto& entityManager = CEntityManager::GetInstance();
	auto attackComp = entityManager.GetComponent<AttackComponent>(CGameController::GetActiveInventoryCharacterNumber());
	auto defenseComp = entityManager.GetComponent<DefenseComponent>(CGameController::GetActiveInventoryCharacterNumber());
	auto magicComp = entityManager.GetComponent<MagicComponent>(CGameController::GetActiveInventoryCharacterNumber());

	if (!attackComp || !defenseComp || !magicComp)
	{
		return;
	}

	sf::Text attackInfo;
	attackInfo.setFont(font);
	attackInfo.setCharacterSize(45);
	attackInfo.setString("Attack:     " + std::to_string(attackComp->attackValue));
	attackInfo.setPosition(m_inventory.inventoryMenu.getPosition().x + 60, m_inventory.inventoryMenu.getPosition().x + 100);

	sf::Text defenseInfo;
	defenseInfo.setFont(font);
	defenseInfo.setCharacterSize(45);
	defenseInfo.setString("Defense:   " + std::to_string(defenseComp->defenseValue));
	defenseInfo.setPosition(m_inventory.inventoryMenu.getPosition().x + 60, m_inventory.inventoryMenu.getPosition().x + 150);

	sf::Text magicInfo;
	magicInfo.setFont(font);
	magicInfo.setCharacterSize(45);
	magicInfo.setString("Magic:        " + std::to_string(magicComp->magicValue));
	magicInfo.setPosition(m_inventory.inventoryMenu.getPosition().x + 60, m_inventory.inventoryMenu.getPosition().x + 200);

	m_inventory.heroStats.push_back(attackInfo);
	m_inventory.heroStats.push_back(defenseInfo);
	m_inventory.heroStats.push_back(magicInfo);
}

void CViewSystem::UpdateHeroEquipment()
{
	auto& entityManager = CEntityManager::GetInstance();
	auto selectedCharacterInventoryComp = entityManager.GetComponent<InventoryComponent>(CGameController::GetActiveInventoryCharacterNumber());
	if (!selectedCharacterInventoryComp)
	{
		return;
	}

	m_inventory.activeWeaponInfo.setFont(font);
	m_inventory.activeWeaponInfo.setCharacterSize(40);
	m_inventory.activeWeaponInfo.setString("Weapon: " + selectedCharacterInventoryComp->activeWeapon.name);
	m_inventory.activeWeaponInfo.setPosition(m_inventory.inventoryMenu.getPosition().x + 500, m_inventory.inventoryMenu.getPosition().y + 60);
	m_inventory.activeWeaponInfo.setFillColor(sf::Color::White);

	m_inventory.activeShieldInfo.setFont(font);
	m_inventory.activeShieldInfo.setCharacterSize(40);
	m_inventory.activeShieldInfo.setString("Shield: " + selectedCharacterInventoryComp->activeShield.name);
	m_inventory.activeShieldInfo.setPosition(m_inventory.inventoryMenu.getPosition().x + 500, m_inventory.inventoryMenu.getPosition().y + 110);
	m_inventory.activeShieldInfo.setFillColor(sf::Color::White);

	switch (CGameController::GetCurrentEquipmentType())
	{
	case EquipmentType::Weapon: {
		m_inventory.activeWeaponInfo.setFillColor(sf::Color::Yellow);
		break;
	}
	case EquipmentType::Shield: {
		m_inventory.activeShieldInfo.setFillColor(sf::Color::Yellow);
		break;
	}
	}

	m_inventory.characterWeapons.clear();
	m_inventory.characterShields.clear();
	float yOffset = 0;

	for (const auto& weapon : selectedCharacterInventoryComp->weapons)
	{
		if (weapon.name == selectedCharacterInventoryComp->activeWeapon.name && weapon.ownerId == selectedCharacterInventoryComp->activeWeapon.ownerId)
		{
			continue;
		}
		sf::Text weaponText;
		weaponText.setFont(font);
		weaponText.setCharacterSize(40);
		weaponText.setString(weapon.name);
		weaponText.setPosition(m_inventory.inventoryMenu.getPosition().x + 600, m_inventory.inventoryMenu.getPosition().y + 320 + yOffset);

		yOffset += 50;
		m_inventory.characterWeapons.push_back(weaponText);
	}

	yOffset = 0;
	for (const auto& shield : selectedCharacterInventoryComp->shields)
	{
		if (shield.name == selectedCharacterInventoryComp->activeShield.name && shield.ownerId == selectedCharacterInventoryComp->activeShield.ownerId)
		{
			continue;
		}
		sf::Text shieldText;
		shieldText.setFont(font);
		shieldText.setCharacterSize(40);
		shieldText.setString(shield.name);
		shieldText.setPosition(m_inventory.inventoryMenu.getPosition().x + 600, m_inventory.inventoryMenu.getPosition().y + 320 + yOffset);

		yOffset += 50;
		m_inventory.characterShields.push_back(shieldText);
	}
}

void CViewSystem::DrawHeroInfo()
{
	auto& entityManager = CEntityManager::GetInstance();
	auto descriptionComp = entityManager.GetComponent<DescriptionComponent>(CGameController::GetActiveInventoryCharacterNumber());
	auto experienceComp = entityManager.GetComponent<ExperienceComponent>(CGameController::GetActiveInventoryCharacterNumber());
	if (!descriptionComp || !experienceComp)
	{
		return;
	}

	m_inventory.heroLevel.setFont(font);
	m_inventory.heroLevel.setCharacterSize(40);
	m_inventory.heroLevel.setString("LEVEL: " + std::to_string(experienceComp->currentHeroLevel));
	m_inventory.heroLevel.setPosition(m_inventory.inventoryMenu.getPosition().x + 500, m_inventory.inventoryMenu.getPosition().y + 65);

	m_inventory.heroDescription.setFont(font);
	m_inventory.heroDescription.setCharacterSize(40);
	m_inventory.heroDescription.setString("INFO: " + descriptionComp->description);
	m_inventory.heroDescription.setPosition(m_inventory.inventoryMenu.getPosition().x + 500, m_inventory.inventoryMenu.getPosition().y + 120);

	m_window.draw(m_inventory.heroLevel);
	m_window.draw(m_inventory.heroDescription);
}

void CViewSystem::DrawHeroSpells()
{
	auto& entityManager = CEntityManager::GetInstance();
	auto spellComp = entityManager.GetComponent<SpellComponent>(CGameController::GetActiveInventoryCharacterNumber());
	if (!spellComp)
	{
		return;
	}

	float yOffset = 0;
	for (const auto& spell : spellComp->spells)
	{
		sf::Text spellName;
		spellName.setFont(font);
		spellName.setCharacterSize(40);
		spellName.setString("LW: " + std::to_string(spell.level) + " " + spell.name);
		spellName.setPosition(m_inventory.inventoryMenu.getPosition().x + 550, m_inventory.inventoryMenu.getPosition().y + 320 + yOffset);

		yOffset += 50;
		m_window.draw(spellName);
	}
}
