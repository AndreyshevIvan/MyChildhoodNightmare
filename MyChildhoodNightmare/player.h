#pragma once
#include "character.h"

const sf::Vector2f PLAYER_SIZE = { 70, 100 };
const sf::Vector2f PLAYER_SPAWN_POS = { 350 , 700 };
const float PLAYER_MOVE_SPEED = 250;
const float PLAYER_START_HEALTH = 100;
const float PLAYER_BULLETS_ANGLE = 0.2f;
const float INJURED_COLDOWN = 1.5;

enum Weapon
{
	MELEE,
	FIREBALL
};

struct Player :Character
{
	Weapon weapon = FIREBALL;
	float injuredColdown;
	unsigned points;

	bool InitPlayer();
	
	void Attack();
	void UpdateStatuses();
};