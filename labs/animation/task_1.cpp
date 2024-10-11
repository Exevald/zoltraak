#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>

struct WindowSettings
{
	std::string windowTitle = "Circle Wave Move";
	unsigned int windowWidth = 800;
	unsigned int windowHeight = 600;
	float ballRadius = 40;
	float ballSpeedX = 100;
	float amplitudeY = 80;
	float periodY = 2;
};

int main()
{
	WindowSettings windowSettings;
	sf::RenderWindow window(
			sf::VideoMode({ windowSettings.windowWidth, windowSettings.windowHeight }),
			windowSettings.windowTitle
	);
	sf::Clock clock1;
	sf::Clock clock2;

	sf::Vector2f ballPosition = { 0, 350 };

	sf::CircleShape ball(windowSettings.ballRadius);
	ball.setFillColor(sf::Color(sf::Color::Blue));

	while (window.isOpen())
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}
		}

		const float time = clock1.getElapsedTime().asSeconds();
		const float dt = clock2.restart().asSeconds();

		ballPosition.x = ballPosition.x + windowSettings.ballSpeedX * dt;
		if ((ballPosition.x + 2 * windowSettings.ballRadius >= float(windowSettings.windowWidth)) &&
			(windowSettings.ballSpeedX > 0))
		{
			windowSettings.ballSpeedX = -windowSettings.ballSpeedX;
		}
		if ((ballPosition.x < 0) && (windowSettings.ballSpeedX < 0))
		{
			windowSettings.ballSpeedX = -windowSettings.ballSpeedX;
		}

		const float wavePhase = time * float(2 * M_PI);
		const float y = windowSettings.amplitudeY * std::sin(wavePhase / windowSettings.periodY);
		const sf::Vector2f offset = { 0, y };

		ball.setPosition(ballPosition + offset);

		window.clear();
		window.draw(ball);
		window.display();
	}

	return EXIT_SUCCESS;
}