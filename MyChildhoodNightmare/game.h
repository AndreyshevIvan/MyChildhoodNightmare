#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "player.h"

class Game
{
private:
public:
	Player player;

	bool InitGame();
};