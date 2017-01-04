#include "sound.h"

bool GameSound::InitGameSound()
{
	if (!menuSound.openFromFile("resources/sound/main_menu_sound.ogg") ||
		!gameplaySound.openFromFile("resources/sound/gameplay_1_sound.ogg") ||
		!shadowCollideSound.openFromFile("resources/sound/shadow_collide_sound.ogg") ||
		!ghostCollideSound.openFromFile("resources/sound/ghost_collide_sound.wav") ||
		!finalSound.openFromFile("resources/sound/final_sound.ogg") ||
		!menuButtonSwitchSound.openFromFile("resources/sound/menu_button_switch_sound.ogg") ||
		!ammoBonusSound.openFromFile("resources/sound/bonus_weapon_sound.ogg") ||
		!healthBonusSound.openFromFile("resources/sound/bonus_health_sound.ogg") ||
		!boxBonusSound.openFromFile("resources/sound/bonus_box_sound.wav") ||
		!randomBonusSound.openFromFile("resources/sound/bonus_random_sound.ogg"))
	{
		return false;
	}

	menuSound.setVolume(MAIN_MENU_VOLUME); 
	gameplaySound.setVolume(GAMEPLAY_VOLUME);
	menuButtonSwitchSound.setVolume(MENU_BUTTON_SWITCH_VOLUME);
	finalSound.setVolume(WIN_VOLUME);

	shadowCollideSound.setVolume(COLLIDES_VOLIME);
	ghostCollideSound.setVolume(COLLIDES_VOLIME);

	ammoBonusSound.setVolume(ADD_AMMO_BONUS_VOLUME);
	healthBonusSound.setVolume(ADD_HEALTH_BONUS_VOLUME);
	boxBonusSound.setVolume(ADD_HEALTH_BONUS_VOLUME);
	randomBonusSound.setVolume(ADD_AMMO_BONUS_VOLUME);

	return true;
}

void GameSound::CollideWithEnemySound(int type)
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

void GameSound::CollideWithBonusSound(int type)
{
	switch (type)
	{
	case AK_AMMO:
		ammoBonusSound.play();
		break;
	case SHOOTGUN_AMMO:
		ammoBonusSound.play();
		break;
	case HEALTH:
		healthBonusSound.play();
		break;
	case RANDOM:
		randomBonusSound.play();
		break;
	case BOX:
		boxBonusSound.play();
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
		!weaponShootgun.openFromFile("resources/sound/weapon_shootgun_sound.ogg") ||
		!switchWeapon.openFromFile("resources/sound/switch_weapon_sound.ogg"))
	{
		return false;
	}

	playerDeath.setVolume(PLAYER_DEATH_VOLUME);

	weaponPistol.setVolume(WEAPON_VOLUME);
	weaponAK.setVolume(WEAPON_VOLUME);
	weaponShootgun.setVolume(WEAPON_VOLUME);
	switchWeapon.setVolume(SWITCH_WEAPON_VOLUME);

	return true;
}

bool BonusesSound::InitBonusesSound()
{
	return true;
}