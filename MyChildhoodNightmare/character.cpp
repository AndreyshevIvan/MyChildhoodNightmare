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

void Character::UpdatePos(float elapsedTime, std::vector<Object> const& blocks)
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

		if (IsCollidesWithLevel(collisionRect, blocks))
		{
			collisionRect.left -= movement;
		}
	}

	UpdateGravity(elapsedTime, blocks);

	runStatus = NOT_RUN;
	bodyShape.setPosition(GetCharacterPos());
}

void Character::UpdateGravity(float elapsedTime, std::vector<Object> const& blocks)
{
	float movementY = jumpSpeed;

	jumpSpeed = jumpSpeed + G * elapsedTime;
	movementY = jumpSpeed * elapsedTime;

	collisionRect.top += movementY;

	if (IsCollidesWithLevel(collisionRect, blocks))
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

bool Character::IsCollidesWithLevel(FloatRect const& rect, vector<Object> const& blocks)
{
	for (auto block : blocks)
	{
		if (rect.intersects(block.rect) && block.name == "solid")
		{
			return true;
		}
	}
	return false;
}

void Character::Draw(RenderWindow& window, sf::FloatRect const& area)
{
	if (bodyShape.getGlobalBounds().intersects(area))
	{
		window.draw(bodyShape);
	}
}

void Character::Clear()
{
	for (auto it = characterBullets.begin(); it != characterBullets.end();)
	{
		Bullet* bullet = *it;
		it = characterBullets.erase(it);
		delete(bullet);
	}
}