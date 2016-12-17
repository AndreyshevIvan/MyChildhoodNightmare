#pragma once
#include "character.h"

const sf::Vector2f PLAYER_SIZE = { 70, 100 };
const sf::Vector2f PLAYER_SPAWN_POS = { 350 , 700 };
const float PLAYER_MOVE_SPEED = 250;

const float PLAYER_START_HEALTH = 100;
const int PLAYER_START_SHOOTGUN_AMMO = 7;
const int PLAYER_START_AK_AMMO = 30;

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
	unsigned points;
	std::vector<int> ammo;

	bool InitPlayer();

	void SwitchWeapon();
	
	void Attack();
	void UpdateStatuses();
};