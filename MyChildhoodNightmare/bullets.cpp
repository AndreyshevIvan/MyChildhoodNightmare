#include "bullets.h"

void Bullet::Update(float elapsedTime)
{
	if (movmentOrientation == 1)
	{
		collisionRect.left -= BULLET_SPEED * elapsedTime, 0;
	}
	else
	{
		collisionRect.left += BULLET_SPEED * elapsedTime, 0;
	}

	bodyShape.setPosition(sf::Vector2f{ collisionRect.left, collisionRect.top });
}