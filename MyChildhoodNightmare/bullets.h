#pragma once
#include "stdafx.h"

const float BULLET_SPEED = 700;
const sf::Vector2f BULLET_SIZE = { 30, 8 };

const float MAX_WEAPON_COLDOWN = 1;
const float MELEE_COLDOWN = 0.4f;
const float SHOOTGUN_COLDOWN = 0.5f;
const float AK_COLDOWN = 0.1f;

struct Bullet
{
	sf::RectangleShape bodyShape;
	sf::FloatRect collisionRect;
	sf::Vector2f speed;
	int movmentOrientation;
	float demage;
	bool isLive = true;

	Bullet(sf::Vector2f const& startPos, float demage, int orientation)
	{
		collisionRect.left = startPos.x;
		collisionRect.top = startPos.y - 50;
		collisionRect.width = BULLET_SIZE.x;
		collisionRect.height = BULLET_SIZE.y;

		bodyShape.setSize(BULLET_SIZE);
		bodyShape.setFillColor(sf::Color::Magenta);
		bodyShape.setPosition({ collisionRect.left, collisionRect.top });
		movmentOrientation = orientation;

		this->demage = demage;
	}

	void Update(float elapsedTime);
};