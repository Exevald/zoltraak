#pragma once

#include <SFML/Graphics.hpp>
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
	sf::Sprite sprite;
	int currentFrameNumber = 0;
	int totalFrames;
	float frameDuration;
	float elapsedTime = 0.f;
	sf::Vector2i frameSize;
	int initialSpriteX = 0;
	int initialSpriteY = 0;

	AnimationComponent(const sf::Texture& texture, int initialSpriteX, int initialSpriteY, int totalFrames, sf::Vector2i frameSize, float frameDuration)
		: totalFrames(totalFrames)
		, frameSize(frameSize)
		, frameDuration(frameDuration)
		, initialSpriteX(initialSpriteX)
		, initialSpriteY(initialSpriteY)
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

	explicit ExperienceComponent(
		int experience,
		int levelExperienceLimit,
		int currentHeroLevel)
		: experience(experience)
		, levelExperienceLimit(levelExperienceLimit)
		, currentHeroLevel(currentHeroLevel)
	{
	}
};

struct MenuSoulComponent : IComponent
{
};

struct FightTurnComponent : IComponent
{
	bool isEntityTurn;

	explicit FightTurnComponent(bool turn)
		: isEntityTurn(turn)
	{
	}
};