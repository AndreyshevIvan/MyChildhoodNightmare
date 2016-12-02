#pragma once
#include "character.h"

const sf::Vector2f ENEMY_SHADOW_SIZE = { 50, 100 };
const float ENEMY_SHADOW_MOVE_SPEED = 200;
const float ENEMY_SHADOW_START_HEALTH = 200;

struct EnemyShadow :Character
{
	EnemyShadow(Object const& spawnObj)
	{
		bodyTexture.loadFromFile("resources/enemyShadow.png");

		collisionRect.width = ENEMY_SHADOW_SIZE.x / 2.0f;
		collisionRect.height = ENEMY_SHADOW_SIZE.y - 10;
		collisionRect.left = spawnObj.rect.left;
		collisionRect.top = spawnObj.rect.top;

		bodyShape.setSize(ENEMY_SHADOW_SIZE);
		bodyShape.setTexture(&bodyTexture);
		bodyShape.setSize(ENEMY_SHADOW_SIZE);
		bodyShape.setOrigin(ENEMY_SHADOW_SIZE.x / 2.0f, ENEMY_SHADOW_SIZE.y);

		moveSpeed = ENEMY_SHADOW_MOVE_SPEED;
		jumpSpeed = 0;
		health = ENEMY_SHADOW_START_HEALTH;

		shootColdown = 0;
		currentFrame = 0;
		orientationStatus = RIGHT;
	}
};