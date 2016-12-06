#include "bullets.h"

void Bullet::Update(float elapsedTime)
{
	if (movmentOrientation == 1)
	{
		collisionRect.left -= BULLET_SPEED * elapsedTime;
		collisionRect.top += BULLET_SPEED * elapsedTime * angle;
	}
	else if (movmentOrientation == 2)
	{
		collisionRect.left += BULLET_SPEED * elapsedTime;
		collisionRect.top += BULLET_SPEED * elapsedTime * angle;
	}

	bodyShape.setPosition(sf::Vector2f{ collisionRect.left, collisionRect.top });
}