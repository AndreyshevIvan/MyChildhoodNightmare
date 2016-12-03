#include <SFML/Graphics.hpp>
#include "player.h"

using namespace sf;

bool Player::InitPlayer(Object const& spawnObj)
{
	if (!bodyTexture.loadFromFile("resources/player.png"))
	{
		return false;
	}

	collisionRect.width = PLAYER_SIZE.x / 2.0f;
	collisionRect.height = PLAYER_SIZE.y - 10;
	collisionRect.left = spawnObj.rect.left;
	collisionRect.top = spawnObj.rect.top;

	bodyShape.setFillColor(sf::Color::Green);
	bodyShape.setSize(PLAYER_SIZE);
	bodyShape.setOrigin(PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y);

	moveSpeed = PLAYER_MOVE_SPEED;
	jumpSpeed = 0;
	health = PLAYER_START_HEALTH;

	enjuredColdown = 0;
	shootColdown = 0;
	currentFrame = 0;
	orientationStatus = RIGHT;

	demage = 30;
	runStatus = NOT_RUN;
	existStatus = LIVE;

	return true;
}

void Player::UpdateStatuses()
{
	if (existStatus == LIVE)
	{
		if (enjuredColdown < INJURED_COLDOWN)
		{
			bodyShape.setFillColor(sf::Color::Red);
		}
		else
		{
			bodyShape.setFillColor(sf::Color::Green);
		}
	}
	else
	{
		bodyShape.setFillColor(sf::Color::Blue);
	}
}

void Player::Attack()
{
	switch (weapon)
	{
	case FIREBALL:
		if (shootColdown > CRY_COLDOWN)
		{
			bullets.push_back(new Bullet(GetCharacterPos(), static_cast<int>(orientationStatus), demage));
			shootColdown = 0;
		}
		break;
	case MELEE:
		if (shootColdown > CRY_COLDOWN)
		{
			bullets.push_back(new Bullet(GetCharacterPos() - sf::Vector2f{ 0, 25 }, static_cast<int>(orientationStatus), demage));
			bullets.push_back(new Bullet(GetCharacterPos(), static_cast<int>(orientationStatus), demage));
			bullets.push_back(new Bullet(GetCharacterPos() - sf::Vector2f{ 0, -25 }, static_cast<int>(orientationStatus), demage));
			shootColdown = 0;
		}
	default:
		break;
	}
}