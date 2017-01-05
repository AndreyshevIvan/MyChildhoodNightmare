#include "player.h"

using namespace sf;
using namespace std;

enum
{
	// AMMO
	INFINITY_AMMO = -1,
	SHOOTGUN = 1,
	AK = 2,
};

bool Player::InitPlayer()
{
	if (!bodyTexture.loadFromFile("resources/player.png") ||
		!InitCharacterSound())
	{
		return false;
	}

	collisionRect.width = PLAYER_SIZE.x / 2.0f;
	collisionRect.height = PLAYER_SIZE.y - 10;

	bodyShape.setTexture(&bodyTexture);
	bodyShape.setSize(PLAYER_SIZE);
	bodyShape.setOrigin(PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y);
	bodyShape.setRotation(0);

	existStatus = ExistenceStatus::LIVE;
	moveSpeed = PLAYER_MOVE_SPEED;
	jumpSpeed = 0;
	maxHealth = PLAYER_START_HEALTH;
	health = PLAYER_START_HEALTH;
	boxes = 0;
	shootDemage = PLAYER_PISTOL_DEMAGE;
	akDemage = PLAYER_AK_DEMAGE;
	shootgunDemage = PLAYER_SHOOTGUN_DEMAGE;
	ammo = { INFINITY_AMMO, PLAYER_START_SHOOTGUN_AMMO, PLAYER_START_AK_AMMO };
	shootRange = PLAYER_START_SHOOT_RANGE;
	deathSound = &playerDeath;

	injuredColdown = INJURED_COLDOWN;

	characterBullets.clear();
	CreateCopy();

	return true;
}

void Player::UpdateStatuses()
{
	if (injuredColdown < INJURED_COLDOWN)
	{
		bodyShape.setFillColor(INJURED_COLOR);
	}
	else
	{
		bodyShape.setFillColor(Color(255, 255, 255, 255));
	}
}

void Player::SwitchWeapon()
{
	switchWeapon.play();
	int currWeapon = static_cast<int>(currentWeapon);

	if (currentWeapon == Weapon::AK)
	{
		currentWeapon = Weapon::PISTOL;
	}
	else
	{
		currentWeapon = Weapon(currWeapon + 1);
	}
}

void Player::Attack()
{
	int orientationId = static_cast<int>(orientationStatus);
	int rounds = ammo[static_cast<int>(currentWeapon)];

	switch (currentWeapon)
	{
	case Weapon::PISTOL:
		if (shootColdown > PISTOL_COLDOWN)
		{
			weaponPistol.play();

			auto bullet = new Bullet(GetCharacterPos(), shootDemage, orientationId, shootRange, BulletType::PLAYER_AK);
			characterBullets.push_back(bullet);
			
			shootColdown = 0;
		}
		break;
	case Weapon::SHOOTGUN:
		if (shootColdown > SHOOTGUN_COLDOWN && rounds > 0)
		{
			weaponShootgun.play();

			Vector2f topBullPos = GetCharacterPos() + Vector2f(0, -25);
			Vector2f bottomBullPos = GetCharacterPos() + Vector2f(0, 25);

			auto topBullet = new Bullet(topBullPos, shootgunDemage, orientationId, shootRange, BulletType::PLAYER_SHOOTGUN);
			auto midBullet = new Bullet(GetCharacterPos(), shootgunDemage, orientationId, shootRange, BulletType::PLAYER_SHOOTGUN);
			auto botBullet = new Bullet(bottomBullPos, shootgunDemage, orientationId, shootRange, BulletType::PLAYER_SHOOTGUN);
			characterBullets.push_back(topBullet);
			characterBullets.push_back(midBullet);
			characterBullets.push_back(botBullet);

			ammo[SHOOTGUN]--;
			shootColdown = 0;
		}
		break;
	case Weapon::AK:
		if (shootColdown > AK_COLDOWN && rounds > 0)
		{
			weaponAK.play();

			auto bullet = new Bullet(GetCharacterPos(), akDemage, orientationId, shootRange, BulletType::PLAYER_AK);
			characterBullets.push_back(bullet);

			ammo[AK]--;
			shootColdown = 0;
		}
		break;
	default:
		break;
	}
}

void Player::RotateDeadBody(float elapsedTime)
{
	bodyShape.rotate(DEAD_ROTATION * elapsedTime / GAME_OVER_COLDOWN);
}

void Player::CreateCopy()
{
	copy_ak_demage = akDemage;
	copy_shootgun_demage = shootDemage;
	copy_ak_ammo = ammo[AK];
	copy_shootgun_ammo = ammo[SHOOTGUN];
	copy_maxHealth = maxHealth;
	copy_health = health;
}

void Player::ReturnCopy()
{
	akDemage = copy_ak_demage;
	shootDemage = copy_shootgun_demage;
	ammo[AK] = copy_ak_ammo;
	ammo[SHOOTGUN] = copy_shootgun_ammo;
	maxHealth = copy_maxHealth;
	health = copy_maxHealth; // all levels start with max health
	bodyShape.setRotation(0);
	existStatus = ExistenceStatus::LIVE;
}

void Player::Clear()
{
	boxes = 0;
}