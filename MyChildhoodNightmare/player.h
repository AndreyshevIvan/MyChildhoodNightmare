#pragma once
#include "character.h"

const sf::Vector2f PLAYER_SIZE = { 53, 75 };
const sf::Vector2f PLAYER_SPAWN_POS = { 350 , 700 };
const float PLAYER_MOVE_SPEED = 250;

const int PLAYER_START_HEALTH = 100;
const int PLAYER_START_SHOOTGUN_AMMO = 7;
const int PLAYER_START_AK_AMMO = 30;
const int PLAYER_MAX_AMMO = 99;
const int PLAYER_PISTOL_DEMAGE = 10;
const int PLAYER_AK_DEMAGE = 6;
const int PLAYER_SHOOTGUN_DEMAGE = 9;
const float PLAYER_START_SHOOT_RANGE = 400;

const int PLAYER_MAX_AK_DEMAGE = 40;
const int PLAYER_MAX_SHOOTGUN_DEMAGE = 30;
const int PLAYER_MAX_HEALTH = 250;

const float INJURED_COLDOWN = 1.5;
const float GAME_OVER_COLDOWN = 2;
const float DEAD_ROTATION = 90;

enum struct Weapon
{
	MELEE,
	SHOOTGUN,
	AK,
};

struct Player : Character
{
	Weapon currentWeapon = Weapon::MELEE;
	std::vector<int> ammo;
	int akDemage = PLAYER_AK_DEMAGE;
	int shootgunDemage = PLAYER_SHOOTGUN_DEMAGE;
	float injuredColdown;
	int boxes = 0;

	int copy_ak_demage;
	int copy_shootgun_demage;
	int copy_ak_ammo;
	int copy_shootgun_ammo;
	int copy_maxHealth;
	int copy_health;

	bool InitPlayer();

	void SwitchWeapon();
	
	void Attack();
	void RotateDeadBody(float elapsedTime);
	void CreateCopy();
	void ReturnCopy();

	void UpdateStatuses();

	void Clear();
};