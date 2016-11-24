#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include "game.h"

static const std::string GAME_NAME = "My Childhood Nightmare";

void EnterGameLoop(sf::RenderWindow& window, Game& game);
void HandleEvents(sf::RenderWindow& window, Game& game);
void Update(Game& game, float const& elapsedTime);
void Render(sf::RenderWindow& window, Game& game);

int main()
{
	sf::VideoMode videoMode;
	videoMode.width = (unsigned int)RESOLUTION_WIDTH;
	videoMode.height = (unsigned int)RESOLUTION_HEIGHT;
	sf::RenderWindow window(videoMode, GAME_NAME, sf::Style::Titlebar + sf::Style::Close);

	Game game;

	if (!game.InitGame())
	{
		return 1;
	}

	EnterGameLoop(window, game);

	return 0;
}

void EnterGameLoop(sf::RenderWindow& window, Game& game)
{
	while (window.isOpen())
	{
		HandleEvents(window, game);
		Update(game, game.GetElapsedTime());
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
	game.ControlPlayer();
}

void Update(Game& game, float const& elapsedTime)
{
	game.MoveCharacter(game.character, elapsedTime);
}

void Render(sf::RenderWindow& window, Game& game)
{
	window.clear(sf::Color::White);

	game.UpdateCamera(game.character.GetCharacterPos());
	window.setView(game.camera);
	game.DrawLevel(window);
	game.DrawCharacter(window);

	window.display();
}