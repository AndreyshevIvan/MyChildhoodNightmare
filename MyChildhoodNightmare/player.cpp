#include "player.h"

using namespace sf;
using namespace std;

bool Player::InitPlayer()
{
	if (!bodyTexture.loadFromFile("resources/player.png"))
	{
		return false;
	}

	collisionRect.width = PLAYER_SIZE.x / 2.0f;
	collisionRect.height = PLAYER_SIZE.y - 10;

	bodyShape.setTexture(&bodyTexture);
	bodyShape.setSize(PLAYER_SIZE);
	bodyShape.setOrigin(PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y);

	moveSpeed = PLAYER_MOVE_SPEED;
	jumpSpeed = 0;
	health = PLAYER_START_HEALTH;

	injuredColdown = INJURED_COLDOWN;

	boxes = 0;
	existStatus = ExistenceStatus::LIVE;
	shootDemage = PLAYER_SHOOT_DEMAGE;
	ammo = { -1, PLAYER_START_SHOOTGUN_AMMO, PLAYER_START_AK_AMMO };
	shootRange = PLAYER_START_SHOOT_RANGE;

	characterBullets.clear();

	return true;
}

void Player::UpdateStatuses()
{
	if (injuredColdown < INJURED_COLDOWN)
	{
		bodyShape.setFillColor(Color(255, 255, 255, 150));
	}
	else
	{
		bodyShape.setFillColor(Color(255, 255, 255, 255));
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

bool Player::AddBonusEffect(Bonus const& bonus)
{
	switch (bonus.bonusType)
	{
	case BonusType::AMMO:
		if (bonus.ammoType == AmmoType::AK)
		{
			if (ammo[2] == PLAYER_MAX_AMMO)
			{
				return false;
			}
			else if (ammo[2] + BONUS_AK_AMMO_COUNT > PLAYER_MAX_AMMO)
			{
				ammo[2] = PLAYER_MAX_AMMO;
			}
			else
			{
				ammo[2] += BONUS_AK_AMMO_COUNT;
			}
		}
		else if (bonus.ammoType == AmmoType::SHOOTGUN)
		{
			if (ammo[1] == PLAYER_MAX_AMMO)
			{
				return false;
			}
			else if (ammo[1] + BONUS_SHOOTGUN_AMMO_COUNT > PLAYER_MAX_AMMO)
			{
				ammo[1] = PLAYER_MAX_AMMO;
			}
			else
			{
				ammo[1] += BONUS_SHOOTGUN_AMMO_COUNT;
			}
		}
		break;
	case BonusType::HEALTH:
		if (health == PLAYER_START_HEALTH)
		{
			return false;
		}
		else if (health + BONUS_HP_COUNT > PLAYER_START_HEALTH)
		{
			health = PLAYER_START_HEALTH;
		}
		else
		{
			health += BONUS_HP_COUNT;
		}
		break;
	case BonusType::SPELL:
		break;
	case BonusType::ITEM_BOX:
		boxes += 1;
		break;
	default:
		break;
	}

	return true;
}

void Player::Attack()
{
	int orientationId = static_cast<int>(orientationStatus);
	int rounds = ammo[(int)currentWeapon];

	switch (currentWeapon)
	{
	case Weapon::MELEE:
		if (shootColdown > MELEE_COLDOWN)
		{
			characterBullets.push_back(new Bullet(GetCharacterPos(), shootDemage, orientationId, shootRange, BulletType::PLAYER_AK));
			shootColdown = 0;
		}
		break;
	case Weapon::SHOOTGUN:
		if (shootColdown > SHOOTGUN_COLDOWN && rounds > 0)
		{
			Vector2f topBullPos = GetCharacterPos() + Vector2f(0, -25);
			Vector2f bottomBullPos = GetCharacterPos() + Vector2f(0, 25);

			characterBullets.push_back(new Bullet(topBullPos, shootDemage, orientationId, shootRange, BulletType::PLAYER_SHOOTGUN));
			characterBullets.push_back(new Bullet(GetCharacterPos(), shootDemage, orientationId, shootRange, BulletType::PLAYER_SHOOTGUN));
			characterBullets.push_back(new Bullet(bottomBullPos, shootDemage, orientationId, shootRange, BulletType::PLAYER_SHOOTGUN));
			ammo[(int)currentWeapon] = rounds - 1;
			shootColdown = 0;
		}
		break;
	case Weapon::AK:
		if (shootColdown > AK_COLDOWN && rounds > 0)
		{
			characterBullets.push_back(new Bullet(GetCharacterPos(), shootDemage, orientationId, shootRange, BulletType::PLAYER_AK));
			ammo[(int)currentWeapon] = rounds - 1;
			shootColdown = 0;
		}
		break;
	default:
		break;
	}
}