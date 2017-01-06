#include "sound.h"

bool GameSound::InitGameSound()
{
	if (!menuSound.openFromFile("resources/sound/main_menu.ogg") ||
		!level_0_ost.openFromFile("resources/sound/level_0_ost.ogg") ||
		!level_1_ost.openFromFile("resources/sound/level_1_ost.ogg") ||
		!level_2_ost.openFromFile("resources/sound/level_2_ost.ogg") ||
		!shadowCollideSound.openFromFile("resources/sound/shadow_collide.ogg") ||
		!ghostCollideSound.openFromFile("resources/sound/ghost_collide.wav") ||
		!winSound.openFromFile("resources/sound/win.ogg") ||
		!menuButtonSwitchSound.openFromFile("resources/sound/menu_button_switch.ogg") ||
		!ammoBonusSound.openFromFile("resources/sound/bonus_weapon.ogg") ||
		!healthBonusSound.openFromFile("resources/sound/bonus_health.ogg") ||
		!boxBonusSound.openFromFile("resources/sound/bonus_box.wav") ||
		!randomBonusSound.openFromFile("resources/sound/bonus_random.ogg") ||
		!playerHurtGrunt.openFromFile("resources/sound/player_hurt_grunt.ogg") ||
		!gameOverSound.openFromFile("resources/sound/game_over.ogg"))
	{
		return false;
	}

	menuSound.setVolume(MAIN_MENU_VOLUME);
	level_0_ost.setVolume(GAMEPLAY_VOLUME);
	level_1_ost.setVolume(GAMEPLAY_VOLUME);
	level_2_ost.setVolume(GAMEPLAY_VOLUME);
	menuButtonSwitchSound.setVolume(MENU_BUTTON_SWITCH_VOLUME);
	gameOverSound.setVolume(MAIN_MENU_VOLUME);
	winSound.setVolume(WIN_VOLUME);

	playerHurtGrunt.setVolume(COLLIDES_VOLIME);
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
	playerHurtGrunt.play();

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
	if (!playerDeath.openFromFile("resources/sound/player_death.ogg") ||
		!weaponPistol.openFromFile("resources/sound/weapon_pistol.ogg") ||
		!weaponAK.openFromFile("resources/sound/weapon_ak.ogg") ||
		!weaponShootgun.openFromFile("resources/sound/weapon_shootgun.ogg") ||
		!switchWeapon.openFromFile("resources/sound/switch_weapon.ogg") ||
		!emptyWeapon.openFromFile("resources/sound/empty_weapon.ogg"))
	{
		return false;
	}

	playerDeath.setVolume(PLAYER_DEATH_VOLUME);

	weaponPistol.setVolume(WEAPON_VOLUME);
	weaponAK.setVolume(WEAPON_VOLUME);
	weaponShootgun.setVolume(WEAPON_VOLUME);
	switchWeapon.setVolume(SWITCH_WEAPON_VOLUME);
	emptyWeapon.setVolume(SWITCH_WEAPON_VOLUME);

	return true;
}

bool BonusesSound::InitBonusesSound()
{
	return true;
}