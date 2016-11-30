#include "player.h"

bool Player::InitPlayer(Object const& spawnObj)
{
	if (!bodyTexture.loadFromFile("resources/p.png"))
	{
		return false;
	}

	sf::Vector2f pos = { spawnObj.rect.left, spawnObj.rect.top };

	collisionRect.width = PLAYER_SIZE.x / 2.0f;
	collisionRect.height = PLAYER_SIZE.y - 10;
	collisionRect.left = spawnObj.rect.left;
	collisionRect.top = spawnObj.rect.top;

	bodyShape.setFillColor(sf::Color::Green);
	bodyShape.setSize(PLAYER_SIZE);
	bodyShape.setOrigin(PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y);

	moveSpeed = PLAYER_MOVE_SPEED;
	jumpSpeed = 0;
	jumpHeight = PLAYER_JUMP_HEIGHT;

	shootColdown = 0;
	currentFrame = 0;
	orientationStatus = RIGHT;

	return true;
}

void Player::Attack()
{
	switch (weapon)
	{
	case FIREBALL:
		if (shootColdown > CRY_COLDOWN)
		{
			bullets.push_back(new Bullet(GetCharacterPos(), static_cast<int>(orientationStatus)));
			shootColdown = 0;
		}
		break;
	case MELEE:
		if (shootColdown > CRY_COLDOWN)
		{
			bullets.push_back(new Bullet(GetCharacterPos() - sf::Vector2f{ 0, 25 }, static_cast<int>(orientationStatus)));
			bullets.push_back(new Bullet(GetCharacterPos(), static_cast<int>(orientationStatus)));
			bullets.push_back(new Bullet(GetCharacterPos() - sf::Vector2f{ 0, -25 }, static_cast<int>(orientationStatus)));
			shootColdown = 0;
		}
	default:
		break;
	}
}