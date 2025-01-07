#include "CEntityManager.h"
#include "CGameController.h"
#include "assets_storage/CTextureStorage.h"
#include "common/Utils.h"
#include "common/level_generator/CLevelGenerator.h"
#include "inventory/item_factory/CInventoryItemFactory.h"
#include "skill/CSkillFactory.h"
#include "spells/CSpellFactory.h"
#include "systems/event/CEventSystem.h"
#include <SFML/Graphics.hpp>

void HandleEvents(sf::RenderWindow& window)
{
	sf::Event event{};
	bool movementKeyPressed = false;

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}

		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
			CEventSystem::HandleMouseClick(mousePos);
		}

		if (event.type == sf::Event::KeyPressed)
		{
			movementKeyPressed = true;
			CEventSystem::HandleKeyPress(event.key.code);
		}
	}

	if (!movementKeyPressed)
	{
		EntityId selectedEntityId = CGameController::GetSelectedEntityId();
		auto& entityManager = CEntityManager::GetInstance();
		auto* velocityComp = entityManager.GetComponent<VelocityComponent>(selectedEntityId);

		if (velocityComp)
		{
			velocityComp->vx = 0;
			velocityComp->vy = 0;
		}
	}
}

int main()
{
	WindowSettings windowSettings;
	sf::RenderWindow window(
		sf::VideoMode(windowSettings.windowWidth, windowSettings.windowHeight),
		windowSettings.windowTitle);
	window.setFramerateLimit(30);
	CGameController gameController;
	CInventoryItemFactory inventoryItemFactory;
	CSpellFactory spellFactory;
	CSkillFactory skillFactory;
	auto& entityManager = CEntityManager::GetInstance();

	EntityId camera = entityManager.CreateEntity();
	entityManager.AddComponent<CameraComponent>(camera, 0, 0);

	auto hero1Sprites = CTextureStorage::GetTexture("hero1_sprites.png");
	auto hero2Sprites = CTextureStorage::GetTexture("hero2_sprites.png");
	auto menuSoul = CTextureStorage::GetTexture("menu_soul.png");
	auto utilsWindows = CTextureStorage::GetTexture("utils_windows.png");
	auto enemySprites = CTextureStorage::GetTexture("enemy_sprite1.png");

	auto mapTexture = CTextureStorage::GetTexture("map_fieldOfHopesAndDreams.png");
	auto level = CLevelGenerator::GenerateLevel("level1.txt");

	CGameController::SetGameState(CurrentState::Player);

	inventoryItemFactory.RegisterItem<HealPotionItem>("Small heal potion", [](int ownerId) {
		return std::make_unique<HealPotionItem>(ownerId, "Small heal potion", 10, 25);
	});
	inventoryItemFactory.RegisterItem<DamagePotionItem>("Damage potion", [](int ownerId) {
		return std::make_unique<DamagePotionItem>(ownerId, "Damage potion", 1, 25);
	});
	inventoryItemFactory.RegisterItem<DefensePotionItem>("DefensePotionItem", [](int ownerId) {
		return std::make_unique<DefensePotionItem>(ownerId, "Defense potion", 1, 25);
	});
	inventoryItemFactory.RegisterItem<ManaPotionItem>("Small mana potion", [](int ownerId) {
		return std::make_unique<ManaPotionItem>(ownerId, "Small mana potion", 10, 25);
	});

	inventoryItemFactory.RegisterItem<WeaponItem>("Empty Fist", [](int ownerId) {
		const std::unordered_map<BonusType, int> itemBonuses;

		return std::make_unique<WeaponItem>(ownerId, "Empty Fist", itemBonuses, 0);
	});

	inventoryItemFactory.RegisterItem<WeaponItem>("Wood Blade", [](int ownerId) {
		const int damageBonus = 1;
		const std::unordered_map<BonusType, int> itemBonuses = {
			{ BonusType::DamageBonus, damageBonus },
		};

		return std::make_unique<WeaponItem>(ownerId, "Wood Blade", itemBonuses, 30);
	});

	inventoryItemFactory.RegisterItem<WeaponItem>("Bounce Blade", [](int ownerId) {
		const int damageBonus = 2;
		const int armorBonus = 1;
		const std::unordered_map<BonusType, int> itemBonuses = {
			{ BonusType::DamageBonus, damageBonus },
			{ BonusType::ArmorBonus, armorBonus },
		};

		return std::make_unique<WeaponItem>(ownerId, "Bounce Blade", itemBonuses, 30);
	});

	inventoryItemFactory.RegisterItem<WeaponItem>("Mecha Saber", [](int ownerId) {
		const int damageBonus = 2;
		const int armorBonus = 1;
		const std::unordered_map<BonusType, int> itemBonuses = {
			{ BonusType::DamageBonus, damageBonus },
			{ BonusType::ArmorBonus, armorBonus },
		};

		return std::make_unique<WeaponItem>(ownerId, "Mecha Saber", itemBonuses, 30);
	});

	inventoryItemFactory.RegisterItem<ShieldItem>("Poor man shield", [](int ownerId) {
		const std::unordered_map<BonusType, int> itemBonuses;

		return std::make_unique<ShieldItem>(ownerId, "Poor man shield", itemBonuses, 0);
	});

	inventoryItemFactory.RegisterItem<ShieldItem>("White Ribbon", [](int ownerId) {
		const int armorBonus = 2;
		const std::unordered_map<BonusType, int> itemBonuses = {
			{ BonusType::ArmorBonus, armorBonus },
		};

		return std::make_unique<ShieldItem>(ownerId, "White Ribbon", itemBonuses, 30);
	});

	inventoryItemFactory.RegisterItem<ShieldItem>("Amber Card", [](int ownerId) {
		const int armorBonus = 1;
		const std::unordered_map<BonusType, int> itemBonuses = {
			{ BonusType::ArmorBonus, armorBonus },
		};

		return std::make_unique<ShieldItem>(ownerId, "Amber Card", itemBonuses, 30);
	});

	inventoryItemFactory.RegisterItem<ShieldItem>("Iron Shackle", [](int ownerId) {
		const int damageBonus = 1;
		const int armorBonus = 2;
		const std::unordered_map<BonusType, int> itemBonuses = {
			{ BonusType::DamageBonus, damageBonus },
			{ BonusType::ArmorBonus, armorBonus },
		};

		return std::make_unique<ShieldItem>(ownerId, "Iron Shackle", itemBonuses, 30);
	});

	inventoryItemFactory.RegisterItem<ShieldItem>("Mouse Token", [](int ownerId) {
		const int armorBonus = 2;
		const int magicBonus = 1;
		const std::unordered_map<BonusType, int> itemBonuses = {
			{ BonusType::ArmorBonus, armorBonus },
			{ BonusType::MagicBonus, magicBonus },
		};

		return std::make_unique<ShieldItem>(ownerId, "Mouse Token", itemBonuses, 30);
	});

	spellFactory.RegisterSpell<HealSpell>("Heal Prayer", [](int ownerId) {
		return std::make_unique<HealSpell>(ownerId, "Heal Prayer", 30, 25, 1);
	});

	spellFactory.RegisterSpell<HealSpell>("Dual Heal", [](int ownerId) {
		return std::make_unique<HealSpell>(ownerId, "Dual Heal", 50, 30, 1);
	});

	spellFactory.RegisterSpell<DamageSpell>("Rude Sword", [](int ownerId) {
		return std::make_unique<DamageSpell>(ownerId, "Rude Sword", 30, 25, 1);
	});

	spellFactory.RegisterSpell<DamageSpell>("Dark Wave", [](int ownerId) {
		return std::make_unique<DamageSpell>(ownerId, "Dark Wave", 50, 30, 1);
	});

	skillFactory.RegisterSkill<AttackSkill>("Crimson aura", [](int ownerId) {
		return std::make_unique<AttackSkill>(ownerId, "Crimson aura", "Increases hero pure damage on 2 points per level", 2, 2);
	});

	skillFactory.RegisterSkill<AttackSkill>("Venom aura", [](int ownerId) {
		return std::make_unique<AttackSkill>(ownerId, "Venom aura", "Decreases enemies pure damage on 2 points per level", 2, 1);
	});

	skillFactory.RegisterSkill<DefenceSkill>("Sigma Barrier", [](int ownerId) {
		return std::make_unique<DefenceSkill>(ownerId, "Sigma Barrier", "Increases hero defence on 2 points per level", 2, 2);
	});

	skillFactory.RegisterSkill<DefenceSkill>("Poison touch", [](int ownerId) {
		return std::make_unique<DefenceSkill>(ownerId, "Poison touch", "Decreases enemies defence on 2 points per level", 2, 1);
	});

	EntityId hero1 = entityManager.CreateEntity();
	entityManager.AddComponent<SelectionComponent>(hero1);
	entityManager.AddComponent<NameComponent>(hero1, "Kris");
	entityManager.AddComponent<ColorThemeComponent>(hero1, sf::Color(27, 255, 254));
	entityManager.AddComponent<PositionComponent>(hero1, 1000.0f, 800.0f);
	entityManager.AddComponent<VelocityComponent>(hero1, 0, 0);
	entityManager.AddComponent<CollisionComponent>(hero1, CollisionType::Character);
	entityManager.AddComponent<MassComponent>(hero1, 50);
	entityManager.AddComponent<RotationComponent>(hero1, 0);
	entityManager.AddComponent<HealthComponent>(hero1, 50, 100);
	entityManager.AddComponent<ManaComponent>(hero1, 40, 100);
	entityManager.AddComponent<AvatarComponent>(hero1, "hero1_avatar.png");
	entityManager.AddComponent<ExperienceComponent>(hero1, 0, 100, 1, 2);
	entityManager.AddComponent<AnimationComponent>(hero1, hero1Sprites);
	entityManager.AddComponent<FightTurnComponent>(hero1, false, false);
	entityManager.AddComponent<MoneyComponent>(hero1, 100);
	entityManager.AddComponent<AttackComponent>(hero1, 10);
	entityManager.AddComponent<DefenceComponent>(hero1, 2);
	entityManager.AddComponent<MagicComponent>(hero1, 0);
	entityManager.AddComponent<DescriptionComponent>(hero1, "Commands the party");

	std::vector<InventoryItem> hero1InventoryItems;
	hero1InventoryItems.push_back(*inventoryItemFactory.CreateInventoryItem<HealPotionItem>("Small heal potion", hero1));
	hero1InventoryItems.push_back(*inventoryItemFactory.CreateInventoryItem<HealPotionItem>("Small heal potion", hero1));

	std::vector<WeaponItem> hero1Weapons;
	auto hero1EmptyFistWeapon = *inventoryItemFactory.CreateInventoryItem<WeaponItem>("Empty Fist", hero1);

	hero1Weapons.push_back(hero1EmptyFistWeapon);
	hero1Weapons.push_back(*inventoryItemFactory.CreateInventoryItem<WeaponItem>("Wood Blade", hero1));
	hero1Weapons.push_back(*inventoryItemFactory.CreateInventoryItem<WeaponItem>("Bounce Blade", hero1));

	std::vector<ShieldItem> hero1Shields;
	auto hero1PoorManShield = *inventoryItemFactory.CreateInventoryItem<ShieldItem>("Poor man shield", hero1);

	hero1Shields.push_back(hero1PoorManShield);
	hero1Shields.push_back(*inventoryItemFactory.CreateInventoryItem<ShieldItem>("White Ribbon", hero1));

	entityManager.AddComponent<InventoryComponent>(hero1, hero1EmptyFistWeapon, hero1PoorManShield, hero1Weapons, 6, hero1Shields, 6, hero1InventoryItems, 6);

	std::vector<Spell> hero1Spells;
	hero1Spells.push_back(*spellFactory.CreateSpell<DamageSpell>("Rude Sword", hero1));

	entityManager.AddComponent<SpellComponent>(hero1, hero1Spells);

	std::vector<AttackSkill> hero1AttackSkills;
	hero1AttackSkills.push_back(*skillFactory.CreateSkill<AttackSkill>("Crimson aura", hero1));
	hero1AttackSkills.push_back(*skillFactory.CreateSkill<AttackSkill>("Venom aura", hero1));

	std::vector<DefenceSkill> hero1DefenceSkills;
	hero1DefenceSkills.push_back(*skillFactory.CreateSkill<DefenceSkill>("Sigma Barrier", hero1));
	hero1DefenceSkills.push_back(*skillFactory.CreateSkill<DefenceSkill>("Poison touch", hero1));

	std::vector<SpellCreationSkill> hero1SpellCreationSkills;

	entityManager.AddComponent<SkillsComponent>(hero1, hero1AttackSkills, hero1DefenceSkills, hero1SpellCreationSkills);

	auto* hero1AnimComp = entityManager.GetComponent<AnimationComponent>(hero1);
	hero1AnimComp->AddAnimation("idle", 6, 1563, 6, sf::Vector2i(35, 37), 0.15f);
	hero1AnimComp->AddAnimation("walk_up", 101, 139, 4, sf::Vector2i(18, 37), 0.25f);
	hero1AnimComp->AddAnimation("walk_down", 101, 7, 4, sf::Vector2i(18, 37), 0.25f);
	hero1AnimComp->AddAnimation("walk_left", 101, 51, 4, sf::Vector2i(18, 37), 0.25f);
	hero1AnimComp->AddAnimation("walk_right", 101, 95, 4, sf::Vector2i(18, 37), 0.25f);
	hero1AnimComp->SetAnimation("idle");

	EntityId hero2 = entityManager.CreateEntity();
	entityManager.AddComponent<SelectionComponent>(hero2);
	entityManager.AddComponent<NameComponent>(hero2, "Ralsei");
	entityManager.AddComponent<ColorThemeComponent>(hero2, sf::Color(25, 253, 16));
	entityManager.AddComponent<PositionComponent>(hero2, 1200.0f, 800.0f);
	entityManager.AddComponent<VelocityComponent>(hero2, 0, 0);
	entityManager.AddComponent<CollisionComponent>(hero2, CollisionType::Character);
	entityManager.AddComponent<MassComponent>(hero2, 50);
	entityManager.AddComponent<RotationComponent>(hero2, 0);
	entityManager.AddComponent<HealthComponent>(hero2, 30, 100);
	entityManager.AddComponent<ManaComponent>(hero2, 90, 100);
	entityManager.AddComponent<AvatarComponent>(hero2, "hero2_avatar.png");
	entityManager.AddComponent<ExperienceComponent>(hero2, 80, 100, 1, 2);
	entityManager.AddComponent<AnimationComponent>(hero2, hero2Sprites);
	entityManager.AddComponent<FightTurnComponent>(hero2, false, true);
	entityManager.AddComponent<AttackComponent>(hero2, 8);
	entityManager.AddComponent<DefenceComponent>(hero2, 2);
	entityManager.AddComponent<MagicComponent>(hero2, 7);
	entityManager.AddComponent<DescriptionComponent>(hero2, "Deals damage with scarf");

	std::vector<InventoryItem> hero2InventoryItems;
	hero2InventoryItems.push_back(*inventoryItemFactory.CreateInventoryItem<ManaPotionItem>("Small mana potion", hero2));
	hero2InventoryItems.push_back(*inventoryItemFactory.CreateInventoryItem<DamagePotionItem>("Damage potion", hero2));

	std::vector<WeaponItem> hero2Weapons;
	auto hero2EmptyFistWeapon = *inventoryItemFactory.CreateInventoryItem<WeaponItem>("Empty Fist", hero2);
	hero2Weapons.push_back(hero2EmptyFistWeapon);

	std::vector<ShieldItem> hero2Shields;
	auto hero2PoorManShield = *inventoryItemFactory.CreateInventoryItem<ShieldItem>("Poor man shield", hero2);
	hero2Shields.push_back(hero1PoorManShield);

	entityManager.AddComponent<InventoryComponent>(hero2, hero2EmptyFistWeapon, hero2PoorManShield, hero2Weapons, 6, hero2Shields, 6, hero2InventoryItems, 6);

	std::vector<Spell> hero2Spells;
	hero2Spells.push_back(*spellFactory.CreateSpell<HealSpell>("Heal Prayer", hero2));

	entityManager.AddComponent<SpellComponent>(hero2, hero2Spells);

	auto* hero2AnimComp = entityManager.GetComponent<AnimationComponent>(hero2);
	hero2AnimComp->AddAnimation("idle", 5, 505, 5, sf::Vector2i(49, 40), 0.15f);
	hero2AnimComp->AddAnimation("walk_up", 5, 251, 4, sf::Vector2i(22, 42), 0.2f);
	hero2AnimComp->AddAnimation("walk_down", 5, 104, 4, sf::Vector2i(22, 42), 0.2f);
	hero2AnimComp->AddAnimation("walk_left", 5, 153, 4, sf::Vector2i(22, 42), 0.2f);
	hero2AnimComp->AddAnimation("walk_right", 5, 202, 4, sf::Vector2i(22, 42), 0.2f);
	hero2AnimComp->SetAnimation("idle");

	EntityId box = entityManager.CreateEntity();
	entityManager.AddComponent<PositionComponent>(box, 1300.0f, 900.0f);
	entityManager.AddComponent<VelocityComponent>(box, 0, 0);
	entityManager.AddComponent<CollisionComponent>(box, CollisionType::MovableObject);
	entityManager.AddComponent<MassComponent>(box, 1);
	entityManager.AddComponent<RotationComponent>(box, 0);
	entityManager.AddComponent<ImageComponent>(box, mapTexture, 547, 15, sf::Vector2i(20, 20));

	EntityId mainMenuSoul = entityManager.CreateEntity();
	entityManager.AddComponent<PositionComponent>(mainMenuSoul, window.getSize().x / 2 + 20, window.getSize().y / 2 + 140);
	entityManager.AddComponent<MenuSoulComponent>(mainMenuSoul);
	entityManager.AddComponent<VelocityComponent>(mainMenuSoul, 0, 0);
	entityManager.AddComponent<ImageComponent>(mainMenuSoul, menuSoul, 0, 0, sf::Vector2i(225, 225));

	EntityId fightSoul = entityManager.CreateEntity();
	entityManager.AddComponent<PositionComponent>(fightSoul, 890, 418);
	entityManager.AddComponent<VelocityComponent>(fightSoul, 0, 0);
	entityManager.AddComponent<ImageComponent>(fightSoul, menuSoul, 0, 0, sf::Vector2i(225, 225));
	entityManager.AddComponent<FightSoulComponent>(fightSoul);

	EntityId vendor = entityManager.CreateEntity();

	std::vector<InventoryItem> vendorItems;
	vendorItems.push_back(*inventoryItemFactory.CreateInventoryItem<WeaponItem>("Mecha Saber", vendor));
	vendorItems.push_back(*inventoryItemFactory.CreateInventoryItem<ShieldItem>("Iron Shackle", vendor));
	vendorItems.push_back(*inventoryItemFactory.CreateInventoryItem<HealPotionItem>("Small heal potion", vendor));

	entityManager.AddComponent<VendorComponent>(vendor, vendorItems, 6);

	EntityId enemy1 = entityManager.CreateEntity();
	entityManager.AddComponent<EnemyComponent>(enemy1);
	entityManager.AddComponent<PositionComponent>(enemy1, 1400, 350);
	entityManager.AddComponent<AnimationComponent>(enemy1, enemySprites);
	entityManager.AddComponent<VelocityComponent>(enemy1, 0, 0);
	auto* enemy1AnimationCompAnimationComp = entityManager.GetComponent<AnimationComponent>(enemy1);
	enemy1AnimationCompAnimationComp->AddAnimation("idle", 1, 15, 4, sf::Vector2i(48, 48), 0.15f);
	enemy1AnimationCompAnimationComp->SetAnimation("idle");

	CGameController::InitSystems(inventoryItemFactory, skillFactory);
	CGameController::InitGameSettings(level);
	CFightSystem::LoadAttackPatterns();
	CGameController::SetFightAttacks(CFightSystem::GetAttacks());

	sf::Clock clock;

	while (window.isOpen())
	{
		if (CGameController::GetCurrentGameState() == CurrentState::MainMenu)
		{
			auto playerSoulId = entityManager.GetEntitiesWithComponents<MenuSoulComponent>().front();
			CGameController::SetSelectedEntityId(playerSoulId);
		}
		if (CGameController::GetCurrentGameState() == CurrentState::Fight)
		{
			auto fightSoulId = entityManager.GetEntitiesWithComponents<FightSoulComponent>().front();
			CGameController::SetSelectedEntityId(fightSoulId);
		}

		HandleEvents(window);
		CGameController::UpdateDeltaTime();
		if (CGameController::GetCurrentGameState() == CurrentState::Player)
		{
			CGameController::IncreaseElapsedTime();
		}

		window.clear();
		CGameController::Draw(window, level);
		CGameController::Update();
		window.display();
	}

	return EXIT_SUCCESS;
}
