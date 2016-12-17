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
	runStatus = NOT_RUN;
	existStatus = LIVE;
	points = 0;
	ammo = { -1, PLAYER_START_SHOOTGUN_AMMO, PLAYER_START_AK_AMMO };

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

void Player::SwitchWeapon()
{
	int currWeapon = static_cast<int>(currentWeapon);

	if (currentWeapon == Weapon(2))
	{
		currentWeapon = Weapon(0);
	}
	else
	{
		currentWeapon = Weapon(currWeapon + 1);
	}
}

void Player::Attack()
{
	int orientationId = static_cast<int>(orientationStatus);
	switch (currentWeapon)
	{
	case Weapon::MELEE:
		if (shootColdown > MELEE_COLDOWN)
		{
			bullets.push_back(new Bullet(GetCharacterPos(), demage, orientationId));
			shootColdown = 0;
		}
		break;
	case Weapon::SHOOTGUN:
		if (shootColdown > SHOOTGUN_COLDOWN && ammo[(int)currentWeapon] > 0)
		{
			Vector2f topBullPos = GetCharacterPos() + Vector2f(0, -25);
			Vector2f bottomBullPos = GetCharacterPos() + Vector2f(0, 25);

			bullets.push_back(new Bullet(topBullPos, demage, orientationId));
			bullets.push_back(new Bullet(GetCharacterPos(), demage, orientationId));
			bullets.push_back(new Bullet(bottomBullPos, demage, orientationId));
			ammo[(int)currentWeapon]--;
			shootColdown = 0;
		}
		break;
	case Weapon::AK:
		if (shootColdown > AK_COLDOWN && ammo[(int)currentWeapon] > 0)
		{
			bullets.push_back(new Bullet(GetCharacterPos(), demage, orientationId));
			ammo[(int)currentWeapon]--;
			shootColdown = 0;
		}
		break;
	default:
		break;
	}

}