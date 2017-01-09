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

const float INJURED_DURATION = 1.0;
const sf::Color INJURED_COLOR = sf::Color(255, 255, 255, 140);
const float GAME_OVER_DURATION = 2;
const float DEAD_ROTATION = 90;

enum Weapon
{
	PISTOL,
	SHOOTGUN,
	AK,
};

struct Player : Character
{
	Weapon currentWeapon = Weapon::PISTOL;
	std::map<Weapon, int> ammoMap;
	int akDemage;
	int shootgunDemage;
	int boxes;
	float injuredColdown;
	std::map<Weapon, float> weaponColdownsMap;
	float switchWeaponColdown;

	int copy_ak_demage;
	int copy_shootgun_demage;
	int copy_ak_ammo;
	int copy_shootgun_ammo;
	int copy_maxHealth;
	int copy_health;
	int copy_boxes;

	bool InitPlayer();

	void SwitchWeapon();
	
	void Attack();
	void PistolFire(int orientation);
	void ShootgunFire(int orientation);
	void AkFire(int orientation);

	void RotateDeadBody(float elapsedTime);
	void CreateCopy();
	void ReturnCopy();

	void UpdateStatuses();

	void Clear();
};