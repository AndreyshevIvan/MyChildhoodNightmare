#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include "game.h"

static const std::string GAME_NAME = "My Childhood Nightmare";

void InitScenes(Game& game);
void InitGamePlayScene(Game& game);
void InitMainMenuScene(Game& game);

void EnterGameLoop(sf::RenderWindow& window, Game& game);
void HandleEvents(sf::RenderWindow& window, Game& game);
void Update(Game& game);
void Render(sf::RenderWindow& window, Game& game);

int main()
{
	sf::VideoMode videoMode;
	videoMode.width = static_cast<unsigned>(RESOLUTION.x);
	videoMode.height = static_cast<unsigned>(RESOLUTION.y);
	sf::RenderWindow window(videoMode, GAME_NAME, sf::Style::Titlebar | sf::Style::Close | sf::Style::Fullscreen);

	Game game;

	if (game.InitGame())
	{
		InitScenes(game);
		EnterGameLoop(window, game);
	}
	else
	{
		std::cout << "ERROR: FILES NOT FOUND.";
		return 1;
	}

	return 0;
}

void EnterGameLoop(sf::RenderWindow& window, Game& game)
{
	while (window.isOpen())
	{
		game.SetElapsedTime();

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
	game.currentScene->toHandle(window);
}

void Update(Game& game)
{
	game.currentScene->onUpdate();
}

void Render(sf::RenderWindow& window, Game& game)
{
	window.clear(sf::Color(20, 12, 28));
	game.currentScene->onDraw(window);
	window.display();
}

void InitScenes(Game& game)
{
	InitGamePlayScene(game);
	InitMainMenuScene(game);

	game.currentScene = &game.mainMenuScene;
}

void InitGamePlayScene(Game& game)
{
	game.gameplayScene.toHandle = [&](sf::RenderWindow& window) {
		game.ControlPlayer(window);
	};
	game.gameplayScene.onUpdate = [&]() {
		game.UpdateColdowns();
		game.UpdatePlayer();
	};
	game.gameplayScene.onDraw = [&](sf::RenderWindow& window) {
		game.UpdateCamera(window);
		game.DrawLevel(window);
		game.DrawCharacter(game.player, window);
		game.player.DrawBullets(window);
	};
}

void InitMainMenuScene(Game& game)
{
	game.mainMenuScene.toHandle = [&](sf::RenderWindow& window) {
		game.ControlMainMenu(window);
	};
	game.mainMenuScene.onUpdate = [&]() {
		game.UpdateColdowns();
		game.mainMenu.UdateMainMenu();
	};
	game.mainMenuScene.onDraw = [&](sf::RenderWindow& window) {
		game.mainMenu.DrawMainMenu(window);
	};
}