#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include "tinyxml/level.h"
#include <iostream>

const float BULLET_SPEED = 700;
const sf::Vector2f BULLET_SIZE = { 20, 4 };

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

	void Update(float elapsedTime);
};