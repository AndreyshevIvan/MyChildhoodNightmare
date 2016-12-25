#pragma once
#include "character.h"

const sf::Vector2f PLAYER_SIZE = { 53, 75 };
const sf::Vector2f PLAYER_SPAWN_POS = { 350 , 700 };
const float PLAYER_MOVE_SPEED = 250;

const int PLAYER_START_HEALTH = 100;
const int PLAYER_START_SHOOTGUN_AMMO = 7;
const int PLAYER_START_AK_AMMO = 30;
const int PLAYER_MAX_AMMO = 99;
const float PLAYER_START_SHOOT_RANGE = 500;

const float INJURED_COLDOWN = 1.5;

enum struct Weapon
{
	MELEE,
	SHOOTGUN,
	AK,
};

enum struct Spell
{
	BOOM,
	FREEZE,
	SCARE,
};

struct Player : Character
{
	Weapon currentWeapon = Weapon::MELEE;
	float injuredColdown;
	std::vector<int> ammo;

	int boxes = 0;

	bool InitPlayer();

	void SwitchWeapon();
	
	void Attack();
	bool AddBonusEffect(Bonus const& bonus);

	void UpdateStatuses();
	void UpdateTexture();
};