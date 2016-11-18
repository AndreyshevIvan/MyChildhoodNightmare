#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include "game.h"

static const int RESOLUTION_WIDTH = 1280;
static const int RESOLUTION_HEIGHT = 720;
static const std::string GAME_NAME = "My Childhood Nightmare";

void EnterGameLoop(sf::RenderWindow& window, sf::Clock clock, Game& game);
void HandleEvents(sf::RenderWindow& window);
void Update();
void Render(sf::RenderWindow& window, Game& game);

int main()
{
	sf::Clock clock;
	sf::VideoMode videoMode;
	videoMode.width = RESOLUTION_WIDTH;
	videoMode.height = RESOLUTION_HEIGHT;
	sf::RenderWindow window(videoMode, GAME_NAME, sf::Style::Titlebar + sf::Style::Close);

	Game game;
	game.InitGame();

	EnterGameLoop(window, clock, game);

	return 0;
}

void EnterGameLoop(sf::RenderWindow& window, sf::Clock clock, Game& game)
{
	while (window.isOpen())
	{
		const float elapsedTime = clock.getElapsedTime().asSeconds();
		clock.restart();

		HandleEvents(window);
		Update();
		Render(window, game);
	}
}

void HandleEvents(sf::RenderWindow& window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Up);
			if (event.key.code == sf::Keyboard::Down);
			if (event.key.code == sf::Keyboard::Left);
			if (event.key.code == sf::Keyboard::Right);
			if (event.key.code == sf::Keyboard::Escape);
		}
	}
}

void Update()
{

}

void Render(sf::RenderWindow& window, Game& game)
{
	window.clear();
	game.player.DrawPlayer(window);
	window.display();
}