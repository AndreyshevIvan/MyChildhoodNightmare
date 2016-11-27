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
void Update(Game& game, float const& elapsedTime);
void Render(sf::RenderWindow& window, Game& game);

int main()
{
	sf::VideoMode videoMode;
	videoMode.width = (unsigned int)RESOLUTION_WIDTH;
	videoMode.height = (unsigned int)RESOLUTION_HEIGHT;
	sf::RenderWindow window(videoMode, GAME_NAME, sf::Style::Titlebar + sf::Style::Close);
    window.setKeyRepeatEnabled(false);

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
		Update(game, game.elapsedTime);
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
    game.m_currentScene->toHandle(window);
}

void Update(Game& game, float const& elapsedTime)
{
    game.m_currentScene->onUpdate();
}

void Render(sf::RenderWindow& window, Game& game)
{
	window.clear(sf::Color(20, 12, 28));
    game.m_currentScene->onDraw(window);
	window.display();
}

void InitScenes(Game& game)
{
    InitGamePlayScene(game);
    InitMainMenuScene(game);

    game.m_currentScene = &game.m_mainMenuScene;
}

void InitGamePlayScene(Game& game)
{
    game.m_gameplayScene.toHandle = [&](sf::RenderWindow& window) {
        game.ControlPlayer();
    };
    game.m_gameplayScene.onUpdate = [&]() {
        game.UpdateColdowns();
        game.UpdateCharacterPos(game.player);
        game.UpdateBullets(game.player);
        game.UpdateHealthBar();
    };
    game.m_gameplayScene.onDraw = [&](sf::RenderWindow& window) {
        game.UpdateCamera(window);
        game.DrawLevel(window);
        game.DrawCharacter(game.player, window);
        game.DrawBullets(window, game.player);
        game.DrawPlayerBar(window);
    };
}

void InitMainMenuScene(Game& game)
{
    game.m_mainMenuScene.toHandle = [&](sf::RenderWindow& window) {
        game.ControlMainMenu(window);
    };
    game.m_mainMenuScene.onUpdate = [&]() {
        game.UpdateColdowns();
        game.mainMenu.UdateMainMenu();
    };
    game.m_mainMenuScene.onDraw = [&](sf::RenderWindow& window) {
        game.mainMenu.DrawMainMenu(window);
    };
}

