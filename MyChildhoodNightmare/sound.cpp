#include "sound.h"

bool GameSound::Init()
{
	if (!mainMenu.openFromFile("resources/sound/main_menu_sound.ogg") ||
		!playerDeath.openFromFile("resources/sound/player_death_sound.ogg"))
	{
		return false;
	}

	mainMenu.setVolume(MAIN_MENU_VOLUME);
	playerDeath.setVolume(PLAYER_DEATH_VOLUME);

	return true;
}