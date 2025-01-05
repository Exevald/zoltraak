#pragma once

#include "Utils.h"
#include "skill/Skill.h"
#include "spells/Spells.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <utility>

struct IComponent
{
	virtual ~IComponent() = default;
};

enum class CollisionType
{
	Character,
	MovableObject,
	StaticObject
};

struct SelectionComponent
{
	bool isSelected = false;
};

struct AnimationComponent : IComponent
{
	struct AnimationData
	{
		int totalFrames{};
		float frameDuration{};
		sf::Vector2i frameSize;
		int initialSpriteX{};
		int initialSpriteY{};
	};

	sf::Sprite sprite;
	std::unordered_map<std::string, AnimationData> animations;
	std::string currentAnimation;
	int currentFrameNumber = 0;
	float elapsedTime = 0.f;

	explicit AnimationComponent(const sf::Texture& texture)
	{
		sprite.setTexture(texture);
		sprite.setScale(3.f, 3.f);
	}

	void AddAnimation(const std::string& name, int initialSpriteX, int initialSpriteY, int totalFrames, sf::Vector2i frameSize, float frameDuration)
	{
		animations[name] = { totalFrames, frameDuration, frameSize, initialSpriteX, initialSpriteY };
	}

	void SetAnimation(const std::string& name)
	{
		if (animations.find(name) != animations.end() && currentAnimation != name)
		{
			currentAnimation = name;
			currentFrameNumber = 0;
			elapsedTime = 0.f;
			auto& animData = animations[name];
			sprite.setTextureRect(sf::IntRect(animData.initialSpriteX, animData.initialSpriteY, animData.frameSize.x, animData.frameSize.y));
		}
	}
};

struct ImageComponent : IComponent
{
	sf::Sprite sprite;

	explicit ImageComponent(const sf::Texture& texture, int initialSpriteX, int initialSpriteY, sf::Vector2i frameSize)
	{
		sprite.setTexture(texture);
		sprite.setTextureRect(
			sf::IntRect(
				initialSpriteX,
				initialSpriteY,
				frameSize.x,
				frameSize.y));
		sprite.setScale(3.f, 3.f);
	}
};

struct NameComponent : IComponent
{
	std::string name;

	explicit NameComponent(std::string name)
		: name(std::move(name))
	{
	}
};

struct ColorThemeComponent : IComponent
{
	sf::Color colorTheme;

	explicit ColorThemeComponent(sf::Color color)
		: colorTheme(color)
	{
	}
};

struct AvatarComponent : IComponent
{
	std::string avatarFilePath;

	explicit AvatarComponent(std::string avatarFilePath)
		: avatarFilePath(std::move(avatarFilePath))
	{
	}
};

struct PositionComponent : IComponent
{
	float x;
	float y;

	PositionComponent(float x, float y)
		: x(x)
		, y(y)
	{
	}
};

struct VelocityComponent : IComponent
{
	float vx;
	float vy;

	VelocityComponent(float vx, float vy)
		: vx(vx)
		, vy(vy)
	{
	}
};

struct CameraComponent : IComponent
{
	float posX;
	float posY;

	CameraComponent(float posX, float posY)
		: posX(posX)
		, posY(posY)
	{
	}
};

struct CollisionComponent : IComponent
{
	CollisionType type;

	explicit CollisionComponent(CollisionType type)
		: type(type)
	{
	}
};

struct MassComponent : IComponent
{
	float mass;

	explicit MassComponent(float mass)
		: mass(mass)
	{
	}
};

struct RotationComponent
{
	float angle;

	explicit RotationComponent(float angle)
		: angle(angle)
	{
	}
};

struct InteractionComponent : IComponent
{
	bool canPush;
	bool canPickUp;

	InteractionComponent(bool canPush, bool canPickUp)
		: canPush(canPush)
		, canPickUp(canPickUp)
	{
	}
};

struct HealthComponent : IComponent
{
	float currentHealth;
	float maxHealth;

	explicit HealthComponent(float health, float maxHealth)
		: currentHealth(health)
		, maxHealth(maxHealth)
	{
	}
};

struct ManaComponent : IComponent
{
	float currentMana;
	float maxMana;

	explicit ManaComponent(float mana, float maxMana)
		: currentMana(mana)
		, maxMana(maxMana)
	{
	}
};

struct ExperienceComponent : IComponent
{
	int experience;
	int levelExperienceLimit;
	int currentHeroLevel;
	int availableSkillPoints;

	explicit ExperienceComponent(
		int experience,
		int levelExperienceLimit,
		int currentHeroLevel,
		int skillPoints)
		: experience(experience)
		, levelExperienceLimit(levelExperienceLimit)
		, currentHeroLevel(currentHeroLevel)
		, availableSkillPoints(skillPoints)
	{
	}
};

struct MenuSoulComponent : IComponent
{
};

struct FightSoulComponent : IComponent
{
};

struct FightTurnComponent : IComponent
{
	bool hasEntityActed;
	bool isHeroTurn;

	explicit FightTurnComponent(bool hasActed, bool isHeroTurn)
		: hasEntityActed(hasActed)
		, isHeroTurn(isHeroTurn)
	{
	}
};

struct InventoryComponent : IComponent
{
	WeaponItem activeWeapon;
	ShieldItem activeShield;

	std::vector<WeaponItem> weapons;
	int weaponsInventorySize;

	std::vector<ShieldItem> shields;
	int shieldsInventorySize;

	std::vector<InventoryItem> commonItems;
	int commonItemsInventorySize;

	explicit InventoryComponent(
		WeaponItem weapon,
		ShieldItem shield,
		const std::vector<WeaponItem>& weapons,
		int weaponsInventorySize,
		const std::vector<ShieldItem>& shields,
		int shieldsInventorySize,
		const std::vector<InventoryItem>& commonItems,
		int commonItemsInventorySize)
		: commonItems(commonItems)
		, commonItemsInventorySize(commonItemsInventorySize)
		, weapons(weapons)
		, weaponsInventorySize(weaponsInventorySize)
		, shields(shields)
		, shieldsInventorySize(shieldsInventorySize)
		, activeWeapon(std::move(weapon))
		, activeShield(std::move(shield))
	{
	}
};

struct MoneyComponent : IComponent
{
	int money;

	explicit MoneyComponent(int money)
		: money(money)
	{
	}
};

struct AttackComponent : IComponent
{
	int attackValue;

	explicit AttackComponent(int value)
		: attackValue(value)
	{
	}
};

struct DefenceComponent : IComponent
{
	int defenseValue;

	explicit DefenceComponent(int value)
		: defenseValue(value)
	{
	}
};

struct MagicComponent : IComponent
{
	int magicValue;

	explicit MagicComponent(int value)
		: magicValue(value)
	{
	}
};

struct DescriptionComponent : IComponent
{
	std::string description;

	explicit DescriptionComponent(std::string description)
		: description(std::move(description))
	{
	}
};

struct SpellComponent : IComponent
{
	std::vector<Spell> spells;

	explicit SpellComponent(const std::vector<Spell>& spells)
		: spells(spells)
	{
	}
};

struct VendorComponent : IComponent
{
	std::vector<InventoryItem> items;
	int vendorInventorySize;

	explicit VendorComponent(const std::vector<InventoryItem>& items, int vendorInventorySize)
		: items(items)
		, vendorInventorySize(vendorInventorySize)
	{
	}
};

struct EnemyComponent : IComponent
{
};

struct SkillsComponent : IComponent
{
	std::vector<AttackSkill> attackSkills;
	std::vector<DefenceSkill> defenceSkills;
	std::vector<SpellCreationSkill> spellCreationSkills;

	explicit SkillsComponent(
		const std::vector<AttackSkill>& attackSkills,
		const std::vector<DefenceSkill>& defenceSkills,
		const std::vector<SpellCreationSkill>& spellCreationSkills)
		: attackSkills(attackSkills)
		, defenceSkills(defenceSkills)
		, spellCreationSkills(spellCreationSkills)
	{
	}
};