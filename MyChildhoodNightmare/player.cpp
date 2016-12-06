#include <SFML/Graphics.hpp>
#include "player.h"

using namespace sf;

bool Player::InitPlayer()
{
	if (!bodyTexture.loadFromFile("resources/player.png"))
	{
		return false;
	}

	collisionRect.width = PLAYER_SIZE.x / 2.0f;
	collisionRect.height = PLAYER_SIZE.y - 10;

	bodyShape.setFillColor(Color::Green);
	bodyShape.setSize(PLAYER_SIZE);
	bodyShape.setOrigin(PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y);

	moveSpeed = PLAYER_MOVE_SPEED;
	jumpSpeed = 0;
	health = PLAYER_START_HEALTH;

	injuredColdown = INJURED_COLDOWN;
	shootColdown = 0;
	currentFrame = 0;
	orientationStatus = RIGHT;

	demage = 30;
	bulletsAngle = PLAYER_BULLETS_ANGLE;
	runStatus = NOT_RUN;
	existStatus = LIVE;

	bullets.clear();

	return true;
}

void Player::UpdateStatuses()
{
	if (existStatus == LIVE)
	{
		if (injuredColdown < INJURED_COLDOWN)
		{
			bodyShape.setFillColor(Color::Red);
		}
		else
		{
			bodyShape.setFillColor(Color::Green);
		}
	}
	else
	{
		bodyShape.setFillColor(Color::Blue);
	}
}

void Player::Attack()
{
	if (shootColdown > CRY_COLDOWN)
	{
		int orientationId = static_cast<int>(orientationStatus);
		Vector2f topBullPos = GetCharacterPos() + Vector2f(0, -25);
		Vector2f bottomBullPos = GetCharacterPos() + Vector2f(0, 25);

		switch (weapon)
		{
		case FIREBALL:
			bullets.push_back(new Bullet(GetCharacterPos(), demage, 0, orientationId));
			break;
		case MELEE:
			bullets.push_back(new Bullet(topBullPos, demage, bulletsAngle, orientationId));
			bullets.push_back(new Bullet(GetCharacterPos(), demage, 0, orientationId));
			bullets.push_back(new Bullet(bottomBullPos, demage, -bulletsAngle, orientationId));
			break;
		default:
			break;
		}

		shootColdown = 0;
	}
}