#pragma once
#include "stdafx.h"

const float BULLET_SPEED = 700;
const sf::Vector2f BULLET_SIZE = { 14, 11 };

const float MAX_WEAPON_COLDOWN = 1;
const float MELEE_COLDOWN = 0.4f;
const float SHOOTGUN_COLDOWN = 0.5f;
const float AK_COLDOWN = 0.1f;

enum struct BulletType
{
	PLAYER_AK,
	PLAYER_SHOOTGUN,
	CLOWN_BULLET,
};

struct Bullet
{
	sf::RectangleShape bodyShape;
	sf::Texture bodyTexture;
	sf::FloatRect collisionRect;
	float currentRange = 0;
	float maxRange;
	int movmentOrientation;
	int demage;
	bool isLive = true;

	Bullet(sf::Vector2f const& startPos, int demage, int orientation, float maxRange, BulletType const& type)
	{
		switch (type)
		{
		case BulletType::PLAYER_AK:
			bodyTexture.loadFromFile("resources/bullets_player_ak.png");
			break;
		case BulletType::PLAYER_SHOOTGUN:
			bodyTexture.loadFromFile("resources/bullets_player_shootgun.png");
			break;
		case BulletType::CLOWN_BULLET:
			bodyTexture.loadFromFile("resources/bullets_clown.png");
			break;
		default:
			break;
		}

		collisionRect.left = startPos.x;
		collisionRect.top = startPos.y - 50;
		collisionRect.width = BULLET_SIZE.x;
		collisionRect.height = BULLET_SIZE.y;

		bodyShape.setTexture(&bodyTexture);
		bodyShape.setSize(BULLET_SIZE);
		bodyShape.setPosition({ collisionRect.left, collisionRect.top });
		movmentOrientation = orientation;

		this->maxRange = maxRange;
		this->demage = demage;
	}

	void Update(float elapsedTime);
};