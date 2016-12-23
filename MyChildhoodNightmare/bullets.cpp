#include "bullets.h"

void Bullet::Update(float elapsedTime)
{
	float movement = BULLET_SPEED * elapsedTime;

	if (movmentOrientation == 1)
	{
		collisionRect.left -= movement;
	}
	else
	{
		collisionRect.left += movement;
	}

	bodyShape.setPosition(sf::Vector2f{ collisionRect.left, collisionRect.top });
	currentRange += movement;
	if (currentRange >= maxRange)
	{
		isLive = false;
	}
}