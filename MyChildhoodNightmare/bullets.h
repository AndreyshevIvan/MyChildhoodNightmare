#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include "tinyxml/level.h"
#include <iostream>

const float BULLET_SPEED = 700;
const sf::Vector2f BULLET_SIZE = { 30, 10 };

struct Bullet
{
	sf::RectangleShape bodyShape;
	sf::Vector2f speed;
	std::vector<Object> mapTiles;
	int movmentOrientation;
	bool IsLife;

	Bullet(sf::Vector2f const& startPos, Level level, int const& orientation)
	{
		bodyShape.setSize(BULLET_SIZE);
		bodyShape.setFillColor(sf::Color::Magenta);
		bodyShape.setPosition({ startPos.x, startPos.y - 50});
		IsLife = true;
		mapTiles = level.GetObjects("solid");
		movmentOrientation = orientation;
	}

	void Update(float elapsedTime)
	{
		switch (movmentOrientation)
		{
		case 1:
			bodyShape.move({ -BULLET_SPEED * elapsedTime, 0 });
			break;
		case 2:
			bodyShape.move({ BULLET_SPEED * elapsedTime, 0 });
			break;
		default:
			break;
		}

		auto collisionRect = bodyShape.getGlobalBounds();
		for (unsigned int i = 0; i < mapTiles.size(); i++)
		{
			if (collisionRect.intersects(mapTiles[i].rect) && mapTiles[i].name == "solid")
			{
				IsLife = false;
			}
		}
	}
};