#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "character.h"

const sf::Vector2f PLAYER_SIZE = { 42, 64 };
const sf::Vector2f PLAYER_SPAWN_POS = { 350 , 700 };
const float PLAYER_MOVE_SPEED = 250;
const float PLAYER_JUMP_HEIGHT = 280;
const float PLAYER_START_HEALTH = 300;

enum Weapon
{
	BRANCH,
	CRY,
	PISTOL,
	AK
};

struct Player :Character
{
	Weapon weapon = AK;
	std::list<Bullet*> bullets;
	std::list<Bullet*>::iterator bulletsIter;
	float currentFrame;

	bool InitPlayer(Object const& spawnObj);
	void Attack(Level const& level);
	void UpdateBullets(float elapsedTime);
	void DrawBullets(sf::RenderWindow& window);
};