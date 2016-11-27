#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "tinyxml/level.h"
#include <iostream>
#include <cmath>
#include "game.h"
#include "character.h"
#include "menu.h"
#include <functional>

const float RESOLUTION_WIDTH = 1280;
const float RESOLUTION_HEIGHT = 720;

const float G = 700;
const float JUMP_IMPULSE = 2;
const float FLYING_SLOWDOWN = 0.6;

const sf::Vector2f PLAYER_SIZE = { 42, 56 };
const sf::Vector2f PLAYER_SPAWN_POS = { 350 , 700 };
const std::string PLAYER_FILE_NAME = "player.png";
const float PLAYER_MOVE_SPEED = 350;
const float PLAYER_JUMP_HEIGHT = 280;

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
	Character player;
	Level level;
    Menu mainMenu;
	std::vector<Object> mapTiles;
	float elapsedTime;

    bool InitGame();

	void SetElapsedTime();

	void ControlPlayer();
    void ControlMainMenu(sf::RenderWindow& window);

    void UpdateCamera(sf::RenderWindow& window);
	void UpdateGravity(Character& character);
    void UpdateCharacterPos(Character& character);
	void UpdateColdowns();
	void UpdateBullets(Character& character);
	void UpdateHealthBar();

	void DrawCharacter(Character& character, sf::RenderWindow& window);
	void DrawLevel(sf::RenderWindow& window);
	void DrawBullets(sf::RenderWindow& window, Character& character);
	void DrawPlayerBar(sf::RenderWindow& window);

    bool IsCollidesWithLevel(sf::RectangleShape& shape);

    GameScene m_mainMenuScene;
    GameScene m_gameplayScene;
    GameScene m_gameplayScene2;
    GameScene m_pauseScene;
    GameScene m_creditsScene;
    
    GameScene *m_currentScene = nullptr;
};