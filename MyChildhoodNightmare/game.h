#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include "player.h"
#include "map.h"

static const int RESOLUTION_WIDTH = 1280;
static const int RESOLUTION_HEIGHT = 720;

enum GameStatus
{
	START_MENU,
	CHENGE_LEVEL,
	PLAY,
	PAUSE,
	GAME_OVER
};

class Game
{
private:
	sf::Clock clock;
public:
	Map map;
	Player player;

	float elapsedTime;
	GameStatus gameStatus;

	bool InitGame();
	void UpdateElapsedTime();
	float GetElapsedTime();
	void Control();
};