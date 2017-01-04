#pragma once
#include "stdafx.h"

const float MAIN_MENU_VOLUME = 10.0f;
const float GAMEPLAY_VOLUME = 18.0f;
const float MENU_BUTTON_SWITCH_VOLUME = 5.0f;
const float WIN_VOLUME = 25.0f;

const float PLAYER_DEATH_VOLUME = 20.0f;
const float WEAPON_VOLUME = 12.0f;
const float SWITCH_WEAPON_VOLUME = 10.0f;

const float ADD_BONUS_VOLUME = 15.0f;

const float COLLIDES_VOLIME = 16.0f;

enum
{
	SHADOW,
	CLOWN,
	GHOST,
	SPIDER,
	BOSS,
};

struct GameSound
{
	sf::Music* currentMusic = nullptr;

	sf::Music menuSound;
	sf::Music gameplaySound;
	sf::Music finalSound;

	sf::Music menuButtonSwitchSound;

	sf::Music shadowCollideSound;
	sf::Music ghostCollideSound;

	bool InitGameSound();
	void CollideSound(int type);
};

struct CharacterSound
{
	sf::Music playerDeath;

	sf::Music weaponPistol;
	sf::Music weaponAK;
	sf::Music weaponShootgun;
	sf::Music switchWeapon;

	bool InitCharacterSound();
};

struct BonusesSound
{
	sf::Music ammoSound;

	bool InitBonusesSound();
};