#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <memory>

struct WindowSettings
{
	std::string windowTitle = "Cat moves after mouse";
	unsigned int windowWidth = 800;
	unsigned int windowHeight = 600;
	sf::Color backgroundColor = sf::Color::White;
	float playerSpeed = 20.0f;
	float playerScaleFactor = 15.0f;

};

bool IsCatRotationRight = true;

std::unique_ptr<sf::Texture> LoadTexture(const std::string& filename)
{
	auto texture = std::make_unique<sf::Texture>();
	if (!texture->loadFromFile(filename))
	{
		throw std::invalid_argument("Failed to load texture" + filename);
	}
	return texture;
}

void OnMouseMove(const sf::Event::MouseMoveEvent& event, sf::Vector2f& mousePosition)
{
	mousePosition = { float(event.x), float(event.y) };
}

void PollEvents(sf::RenderWindow& window, sf::Vector2f& mousePosition)
{
	sf::Event event{};
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			OnMouseMove(event.mouseMove, mousePosition);
		}
	}
}

void SetImageScale(sf::Sprite& player, float widthMultiplier, float heightMultiplier, const WindowSettings& settings)
{
	sf::Vector2f targetSize(float(settings.windowWidth) / float(settings.playerScaleFactor),
			float(settings.windowHeight) / float(settings.playerScaleFactor));
	player.setScale(
			targetSize.x / player.getLocalBounds().width * widthMultiplier,
			targetSize.y / player.getLocalBounds().height * heightMultiplier
	);
}

void UpdateCatPosition(const sf::Vector2f& mousePosition, sf::Sprite& player, float dt, const WindowSettings& settings)
{
	const sf::Vector2f delta = mousePosition - player.getPosition();
	const float deltaLength = std::hypot(delta.x, delta.y);

	if (deltaLength < 5.0f)
	{
		return;
	}

	const sf::Vector2f direction = delta / deltaLength;

	player.move(direction * settings.playerSpeed * dt);

	if (direction.x >= 0 && !IsCatRotationRight)
	{
		SetImageScale(player, 1, 1, settings);
		IsCatRotationRight = true;
	}
	else if (direction.x < 0 && IsCatRotationRight)
	{
		SetImageScale(player, -1, 1, settings);
		IsCatRotationRight = false;
	}
}

void DrawFrame(sf::RenderWindow& window, sf::Sprite& player, sf::Sprite& pointer)
{
	window.clear(sf::Color::White);
	window.draw(player);
	window.draw(pointer);
	window.display();
}

void UpdateFrame(const sf::RenderWindow& window, sf::Sprite& player, sf::Clock& clock, sf::Sprite& pointer,
		const WindowSettings& settings)
{
	const float dt = clock.restart().asSeconds();

	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	pointer.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

	UpdateCatPosition(sf::Vector2f(mousePos), player, dt, settings);
}

int main()
{
	WindowSettings windowSettings;

	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;

	sf::RenderWindow window(
			sf::VideoMode({ windowSettings.windowWidth, windowSettings.windowHeight }),
			windowSettings.windowTitle, sf::Style::Default, contextSettings
	);

	sf::Vector2f mousePosition;
	std::unique_ptr<sf::Texture> playerTexture, pointerTexture;

	try
	{
		playerTexture = LoadTexture("cat.png");
		pointerTexture = LoadTexture("red_pointer.png");
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	sf::Sprite pointer(*pointerTexture);
	pointer.setOrigin(pointer.getLocalBounds().width / 2, pointer.getLocalBounds().height / 2);
	SetImageScale(pointer, 1, 1, windowSettings);

	sf::Sprite player(*playerTexture);
	player.setOrigin(player.getLocalBounds().width / 2, player.getLocalBounds().height / 2);
	SetImageScale(player, 1, 1, windowSettings);

	player.setPosition(
			float(windowSettings.windowWidth) / 2,
			float(windowSettings.windowHeight) / 2
	);

	sf::Clock clock;

	while (window.isOpen())
	{
		PollEvents(window, mousePosition);
		UpdateFrame(window, player, clock, pointer, windowSettings);
		DrawFrame(window, player, pointer);
	}

	return EXIT_SUCCESS;
}
