#pragma once
#include "stdafx.h"

const float MAIN_MENU_VOLUME = 14.0f;
const float PLAYER_DEATH_VOLUME = 20.0f;

struct GameSound
{
	sf::Music mainMenu;
	sf::Music playerDeath;

	bool Init();
};