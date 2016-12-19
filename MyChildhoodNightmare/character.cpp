#include "character.h"
#include <iostream>

using namespace std;
using namespace sf;

void Character::Spawn(Object const& spawnObj)
{
	collisionRect.left = spawnObj.rect.left;
	collisionRect.top = spawnObj.rect.top;
}

void Character::Jump()
{
	if (jumpStatus == ON_GROUND)
	{
		jumpSpeed = -CHARACTERS_JUMP_SPEED;
	}
}

sf::Vector2f Character::GetCharacterPos() const
{
	sf::Vector2f position(
		collisionRect.left + collisionRect.width / 4.0f,
		collisionRect.top + collisionRect.height
	);

	return position;
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
			jumpStatus = ON_GROUND;
		}
		jumpSpeed = 0;
	}
	else
	{
		jumpStatus = FLY;
	}
}

void Character::UpdateHealthStatus()
{
	if (health <= 0)
	{
		existStatus = ExistenceStatus::DEAD;
	}
}

bool Character::IsCollidesWithLevel(FloatRect const& rect, vector<Object> const& mapTiles)
{
	for (unsigned i = 0; i < mapTiles.size(); i++)
	{
		if (rect.intersects(mapTiles[i].rect) && mapTiles[i].name == "solid")
		{
			return true;
		}
	}
	return false;
}

void Character::Draw(RenderWindow& window)
{
	window.draw(bodyShape);
}

void Character::Clear()
{
	for (auto it = bullets.begin(); it != bullets.end();)
	{
		Bullet* bullet = *it;
		it = bullets.erase(it);
		delete(bullet);
	}
}