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

const float G = 700;
const float JUMP_IMPULSE = 2;
const float FLYING_SLOWDOWN = 0.6f;

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
	std::function<void(sf::RenderWindow& window)> toHandle;
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
	std::vector<Object> mapTiles;
	float elapsedTime;
	GameScene m_mainMenuScene;
	GameScene m_gameplayScene;
	GameScene m_gameplayScene2;
	GameScene m_pauseScene;
	GameScene *m_currentScene = nullptr;

	bool InitGame();

	void SetElapsedTime();

	void ControlPlayer(sf::RenderWindow& window);
	void ControlMainMenu(sf::RenderWindow& window);

	void UpdateCamera(sf::RenderWindow& window);
	void UpdateGravity(Character& character);
	void UpdateCharacterPos(Character& character);
	void UpdateColdowns();
	void UpdatePlayer();

	void DrawCharacter(Character& character, sf::RenderWindow& window);
	void DrawLevel(sf::RenderWindow& window);

	bool IsCollidesWithLevel(sf::RectangleShape& shape);
};