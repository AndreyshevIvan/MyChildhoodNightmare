#pragma once
#include "character.h"

const sf::Vector2f PLAYER_SIZE = { 70, 100 };
const sf::Vector2f PLAYER_SPAWN_POS = { 350 , 700 };
const float PLAYER_MOVE_SPEED = 250;
const float PLAYER_JUMP_SPEED = 400;
const float PLAYER_START_HEALTH = 100;

enum Weapon
{
	MELEE,
	FIREBALL
};

struct Player :Character
{
	Weapon weapon = FIREBALL;
	std::vector<Bullet*> bullets;

	bool InitPlayer(Object const& spawnObj);
	void Attack();
	void Clear();
};