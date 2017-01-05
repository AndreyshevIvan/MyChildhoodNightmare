#pragma once
#include "stdafx.h"

const float MAIN_MENU_VOLUME = 10.0f;
const float GAMEPLAY_VOLUME = 18.0f;
const float MENU_BUTTON_SWITCH_VOLUME = 5.0f;
const float WIN_VOLUME = 25.0f;

const float PLAYER_DEATH_VOLUME = 20.0f;
const float WEAPON_VOLUME = 10.0f;
const float SWITCH_WEAPON_VOLUME = 10.0f;

const float ADD_AMMO_BONUS_VOLUME = 15.0f;
const float ADD_HEALTH_BONUS_VOLUME = 36.0f;

const float COLLIDES_VOLIME = 16.0f;

enum
{
	// Enemy types
	SHADOW = 0,
	CLOWN,
	GHOST,
	SPIDER,
	BOSS,
	// Bonus types
	AK_AMMO = 0,
	SHOOTGUN_AMMO,
	HEALTH,
	RANDOM,
	BOX,
};

struct GameSound
{
	sf::Music* currentMusic = nullptr;

	sf::Music menuSound;
	sf::Music level_0_ost;
	sf::Music level_1_ost;
	sf::Music level_2_ost;
	sf::Music gameOverSound;
	sf::Music winSound;

	sf::Music menuButtonSwitchSound;

	sf::Music playerHurtGrunt;
	sf::Music shadowCollideSound;
	sf::Music ghostCollideSound;

	sf::Music ammoBonusSound;
	sf::Music healthBonusSound;
	sf::Music boxBonusSound;
	sf::Music randomBonusSound;

	bool InitGameSound();
	void CollideWithEnemySound(int type);
	void CollideWithBonusSound(int type);
};

struct CharacterSound
{
	sf::Music playerDeath;

	sf::Music weaponPistol;
	sf::Music weaponAK;
	sf::Music weaponShootgun;
	sf::Music switchWeapon;
	sf::Music emptyWeapon;

	bool InitCharacterSound();
};

struct BonusesSound
{
	bool InitBonusesSound();
};