#pragma once
#include "character.h"

const sf::Vector2f ENEMY_SHADOW_SIZE = { 50, 100 };
const float ENEMY_SHADOW_MOVE_SPEED = 200;
const float ENEMY_SHADOW_START_HEALTH = 200;

struct EnemyShadow :Character
{
	EnemyShadow(Object const& spawnObj);
};