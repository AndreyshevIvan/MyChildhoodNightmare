#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "tinyxml/level.h"
#include <vector>
#include <list>
#include <iostream>
#include <cmath>
#include "map.h"
#include "game.h"
#include "player.h"

static const float RESOLUTION_WIDTH = 1280;
static const float RESOLUTION_HEIGHT = 720;

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
	Player player;
	Map map; 
	sf::View camera;
	GameStatus gameStatus;
	Level level;
	float elapsedTime;
	std::vector<Object> mapObj;

	bool InitGame();
	void UpdateCamera(sf::Vector2f const& centerPos);
	float GetElapsedTime();
	void Control();
	void Collision(float const& elapsedTime);
};