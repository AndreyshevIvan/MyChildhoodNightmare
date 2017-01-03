#include "sound.h"

bool GameSound::InitGameSound()
{
	if (!menuSound.openFromFile("resources/sound/main_menu_sound.ogg") ||
		!gameplaySound.openFromFile("resources/sound/gameplay_1_sound.ogg") ||
		!shadowCollideSound.openFromFile("resources/sound/shadow_collide_sound.ogg") ||
		!ghostCollideSound.openFromFile("resources/sound/ghost_collide_sound.WAV") ||
		!finalSound.openFromFile("resources/sound/final_sound.ogg"))
	{
		return false;
	}

	menuSound.setVolume(MAIN_MENU_VOLUME); 
	gameplaySound.setVolume(GAMEPLAY_VOLUME);
	finalSound.setVolume(WIN_VOLUME);

	shadowCollideSound.setVolume(COLLIDES_VOLIME);
	ghostCollideSound.setVolume(COLLIDES_VOLIME);

	return true;
}

void GameSound::CollideSound(int type)
{
	switch (type)
	{
	case SHADOW:
		shadowCollideSound.play();
		break;
	case CLOWN:
		break;
	case GHOST:
		ghostCollideSound.play();
		break;
	case SPIDER:
		break;
	case BOSS:
		break;
	default:
		break;
	}
}

bool CharacterSound::InitCharacterSound()
{
	if (!playerDeath.openFromFile("resources/sound/player_death_sound.ogg") ||
		!weaponPistol.openFromFile("resources/sound/weapon_pistol_sound.ogg") ||
		!weaponAK.openFromFile("resources/sound/weapon_ak_sound.ogg") ||
		!weaponShootgun.openFromFile("resources/sound/weapon_shootgun_sound.ogg"))
	{
		return false;
	}

	playerDeath.setVolume(PLAYER_DEATH_VOLUME);

	weaponPistol.setVolume(WEAPON_VOLUME);
	weaponAK.setVolume(WEAPON_VOLUME);
	weaponShootgun.setVolume(WEAPON_VOLUME);

	return true;
}