#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <functional>

#include "tinyxml/level.h"
#include "menu.h"
#include "player.h"

const sf::Vector2f RESOLUTION = { 1366, 768 };
const float CAMERA_VERTICAL_MARGIN = 80;
const sf::Color BACKGROUND_COLOR = sf::Color(20, 12, 28);

enum GameStatus
{
	MAIN_MENU,
	CHENGE_LEVEL,
	PLAY,
	PAUSE,
	GAME_OVER
};

struct GameScene
{
	std::function<void(sf::RenderWindow& window, sf::Event& event)> toHandle;
	std::function<void()> onUpdate;
	std::function<void(sf::RenderWindow& window)> onDraw;
};

struct Game
{
	sf::Clock clock;
	sf::View camera;
	GameStatus gameStatus;
	Player player;
	Level level;
	Menu mainMenu;
	Menu pauseMenu;
	std::vector<Object> mapTiles;
	float elapsedTime;
	sf::Vector2f mapSize;
	GameScene mainMenuScene;
	GameScene gameplayScene;
	GameScene pauseScene;
	GameScene *currentScene = nullptr;

	bool InitGame();

	void SetElapsedTime();

	bool IsCollidesWithLevel(sf::FloatRect const& rect);

	void ControlPlayer(sf::RenderWindow& window, sf::Event& event);
	void ControlMainMenu(sf::RenderWindow& window, sf::Event& event);

	void UpdateCamera(sf::RenderWindow& window);
	void UpdateColdowns();
	void UpdatePlayer();
	void UpdateBullets();

	void DrawCharacter(Character& character, sf::RenderWindow& window);
	void DrawLevel(sf::RenderWindow& window);
	void DrawPlayerBullets(sf::RenderWindow& window);
};