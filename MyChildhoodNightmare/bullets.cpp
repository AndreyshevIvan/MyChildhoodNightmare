#include "bullets.h"

void Bullet::Update(float elapsedTime)
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
		if (collisionRect.intersects(mapTiles[i].rect))
		{
			IsLife = false;
		}
	}
}