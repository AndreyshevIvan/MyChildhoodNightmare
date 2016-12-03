#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include <iostream>
#include "tinyxml/level.h"

const float BULLET_SPEED = 700;
const sf::Vector2f BULLET_SIZE = { 30, 8 };
const float MAX_WEAPON_COLDOWN = 1;
const float PISTOL_COLDOWN = 0.3f;
const float AK_COLDOWN = 0.08f;
const float CRY_COLDOWN = 0.5f;

struct Bullet
{
	sf::RectangleShape bodyShape;
	sf::FloatRect collisionRect;
	sf::Vector2f speed;
	int movmentOrientation;
	float demage;

	Bullet(sf::Vector2f const& startPos, int orientation, float dmg)
	{
		collisionRect.left = startPos.x;
		collisionRect.top = startPos.y - 50;
		collisionRect.width = BULLET_SIZE.x;
		collisionRect.height = BULLET_SIZE.y;

		bodyShape.setSize(BULLET_SIZE);
		bodyShape.setFillColor(sf::Color::Magenta);
		bodyShape.setPosition({ collisionRect.left, collisionRect.top });
		movmentOrientation = orientation;

		demage = dmg;
	}

	void Update(float elapsedTime);
};