#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "tinyxml/level.h"
#include <iostream>
#include <cmath>
#include "game.h"
#include "character.h"

const float RESOLUTION_WIDTH = 1280;
const float RESOLUTION_HEIGHT = 720;

const float G = 500;
const float JUMP_IMPULSE = 1;
const float FLYING_SLOWDOWN = 0.8;

const sf::Vector2f PLAYER_SIZE = { 75, 90 };
const sf::Vector2f PLAYER_SPAWN_POS = { 350 , 700 };
const std::string PLAYER_FILE_NAME = "player.png";
const float PLAYER_MOVE_SPEED = 200;
const float PLAYER_JUMP_HEIGHT = 320;

enum GameStatus
{
	START_MENU,
	CHENGE_LEVEL,
	PLAY,
	PAUSE,
	GAME_OVER
};

struct Game
{
	sf::Clock clock;
	sf::View camera;
	GameStatus gameStatus;
	Character player;
	Level level;
	std::vector<Object> mapTiles;
	float elapsedTime;

	void SetElapsedTime();

	bool InitGame();
	void UpdateCamera(sf::RenderWindow& window);
	void ControlPlayer();
	bool IsCollidesWithLevel(sf::RectangleShape& shape);
	void MoveCharacter(Character& character);
	void ApplyGravity(Character& character);
	void UpdateColdowns();
	void UpdateBullets(Character& character);
	void UpdateHealthBar();

	void DrawCharacter(Character& character, sf::RenderWindow& window);
	void DrawLevel(sf::RenderWindow& window);
	void DrawBullets(sf::RenderWindow& window, Character& character);
	void DrawPlayerBar(sf::RenderWindow& window);
};