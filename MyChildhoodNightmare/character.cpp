#include "character.h"
#include <iostream>

using namespace std;
using namespace sf;

void Character::Jump()
{
	if (jumpStatus == ON_GROUND)
	{
		jumpSpeed = -400;
	}
}

sf::Vector2f Character::GetCharacterPos()
{
	return sf::Vector2f( collisionRect.left + collisionRect.width / 4.0f , collisionRect.top + collisionRect.height );
}

void Character::UpdatePos(float elapsedTime, std::vector<Object> const& mapTiles)
{
	if (runStatus != NOT_RUN)
	{
		float movement = moveSpeed * elapsedTime;

		if (jumpStatus == FLY)
		{
			movement *= FLYING_SLOWDOWN;
		}
		if (runStatus == RUN_LEFT)
		{
			movement = -movement;
		}

		collisionRect.left += movement;

		if (IsCollidesWithLevel(collisionRect, mapTiles))
		{
			collisionRect.left -= movement;
		}
	}

	UpdateGravity(elapsedTime, mapTiles);

	runStatus = NOT_RUN;
	bodyShape.setPosition(GetCharacterPos());
}

void Character::UpdateGravity(float elapsedTime, std::vector<Object> const& mapTiles)
{
	float movementY = jumpSpeed;

	jumpSpeed = jumpSpeed + G * elapsedTime;
	movementY = jumpSpeed * elapsedTime;

	collisionRect.top += movementY;

	if (IsCollidesWithLevel(collisionRect, mapTiles))
	{
		collisionRect.top -= movementY;
		if (movementY > 0)
		{
			if (movementY > MIN_HEIGHT_FOR_DEMAGE)
			{
				health -= movementY * DEMAGE_PER_HEIGHT;
			}
			jumpStatus = ON_GROUND;
		}
		jumpSpeed = 0;
	}
	else
	{
		jumpStatus = FLY;
	}
}

bool Character::IsCollidesWithLevel(sf::FloatRect const& rect, std::vector<Object> const& mapTiles)
{
	for (unsigned int i = 0; i < mapTiles.size(); i++)
	{
		if (rect.intersects(mapTiles[i].rect) && mapTiles[i].name == "solid")
		{
			return true;
		}
	}
	return false;
}

void Character::Draw(sf::RenderWindow& window)
{
	window.draw(bodyShape);
}
