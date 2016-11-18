#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "player.h"

enum GameStatus
{
	START_MENU,
	PLAY,
	PAUSE,
	GAME_OVER
};

class Game
{
private:
	sf::Clock clock;
public:
	Player player;
	float elapsedTime;
	GameStatus gameStatus;

	bool InitGame();
	void UpdateElapsedTime();
	float GetElapsedTime();
	void Control();
};