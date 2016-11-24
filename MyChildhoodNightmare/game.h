#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "tinyxml/level.h"
#include <vector>
#include <list>
#include <iostream>
#include <cmath>
#include "game.h"
#include "character.h"

const float RESOLUTION_WIDTH = 1280;
const float RESOLUTION_HEIGHT = 720; 
static const float G = 500;

const sf::Vector2f PLAYER_SIZE = { 75, 90 };
const sf::Vector2f PLAYER_SPAWN_POS = { 350 , 700 };
const std::string PLAYER_FILE_NAME = "player.png";
const float PLAYER_MOVE_SPEED = 300;
const float PLAYER_JUMP_HEIGHT = 330;

struct Game
{
	sf::Clock clock;
	sf::View camera;
	GameStatus gameStatus;
	Character character;
	Level level;
	float elapsedTime;
	std::vector<Object> mapTiles;

	float GetElapsedTime();

	bool InitGame();
	void UpdateCamera(sf::Vector2f const& centerPos);

	void ControlPlayer();
	bool IsCollidesWithLevel(sf::RectangleShape& shape);
	void MoveCharacter(Character& character, float elapsedTime);
	void ApplyGravity(Character& character, float elapsedTime);

	void DrawCharacter(sf::RenderWindow& window);
	void DrawLevel(sf::RenderWindow& window);
};