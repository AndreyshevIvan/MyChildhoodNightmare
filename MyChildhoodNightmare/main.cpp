#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include "game.h"

static const std::string GAME_NAME = "My Childhood Nightmare";

void EnterGameLoop(sf::RenderWindow& window, Game& game);
void HandleEvents(sf::RenderWindow& window, Game& game);
void Update(Game& game);
void Render(sf::RenderWindow& window, Game& game);

int main()
{
	sf::VideoMode videoMode;
	videoMode.width = RESOLUTION_WIDTH;
	videoMode.height = RESOLUTION_HEIGHT;
	sf::RenderWindow window(videoMode, GAME_NAME, sf::Style::Titlebar + sf::Style::Close);

	Game game;
	if (!game.InitGame())
		return false;

	EnterGameLoop(window, game);

	return 0;
}

void EnterGameLoop(sf::RenderWindow& window, Game& game)
{
	while (window.isOpen())
	{
		game.UpdateElapsedTime();

		HandleEvents(window, game);
		Update(game);
		Render(window, game);
	}
}

void HandleEvents(sf::RenderWindow& window, Game& game)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}
	game.Control();
}

void Update(Game& game)
{

}

void Render(sf::RenderWindow& window, Game& game)
{
	window.clear(BACKGROUND_COLOR);
	game.player.DrawPlayer(window);
	game.map.DrawMap(window);
	window.display();
}