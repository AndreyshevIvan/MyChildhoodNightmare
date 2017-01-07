#include "player.h"

using namespace sf;
using namespace std;

enum
{
	// AMMO
	INFINITY_AMMO = INT_MAX,
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
	shootRange = PLAYER_START_SHOOT_RANGE;
	deathSound = &playerDeath;
	ammoMap = { 
		{ Weapon::PISTOL, INFINITY_AMMO },
		{ Weapon::SHOOTGUN, PLAYER_START_SHOOTGUN_AMMO },
		{ Weapon::AK, PLAYER_START_AK_AMMO }
	};

	injuredColdown = INJURED_DURATION;
	weaponColdownsMap = {
		{ Weapon::PISTOL, PISTOL_COLDOWN },
		{ Weapon::SHOOTGUN, SHOOTGUN_COLDOWN },
		{ Weapon::AK, AK_COLDOWN }
	};

	characterBullets.clear();
	CreateCopy();

	return true;
}

void Player::UpdateStatuses()
{
	if (injuredColdown < INJURED_DURATION)
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
	const float WEAPON_COLDOWN = weaponColdownsMap.find(currentWeapon)->second;

	int rounds = ammoMap.find(currentWeapon)->second;

	bool IsEnoughAmmo = rounds > 0;
	bool IsEmptySoundPlaying = emptyWeapon.getStatus() == sf::Music::Status::Playing;
	bool IsColdownEnded = shootColdown > WEAPON_COLDOWN;

	if (IsColdownEnded)
	{
		if (IsEnoughAmmo)
		{
			switch (currentWeapon)
			{
			case Weapon::PISTOL:
				PistolFire(orientationStatus);
				break;

			case Weapon::SHOOTGUN:
				ShootgunFire(orientationStatus);
				break;

			case Weapon::AK:
				AkFire(orientationStatus);
				break;

			default:
				break;
			}

			shootColdown = 0;
		}
		else if(!IsEmptySoundPlaying)
		{
			emptyWeapon.play();
			shootColdown = 0;
		}
	}
}

void Player::PistolFire(int orientation)
{
	weaponPistol.play();

	auto bullet = new Bullet(GetCharacterPos(), shootDemage, orientation, shootRange, BulletType::PLAYER_AK);
	characterBullets.push_back(bullet);
}

void Player::ShootgunFire(int orientation)
{
	weaponShootgun.play();

	Vector2f topBullPos = GetCharacterPos() + Vector2f(0, -25);
	Vector2f bottomBullPos = GetCharacterPos() + Vector2f(0, 25);

	auto topBullet = new Bullet(topBullPos, shootgunDemage, orientation, shootRange, BulletType::PLAYER_SHOOTGUN);
	auto midBullet = new Bullet(GetCharacterPos(), shootgunDemage, orientation, shootRange, BulletType::PLAYER_SHOOTGUN);
	auto botBullet = new Bullet(bottomBullPos, shootgunDemage, orientation, shootRange, BulletType::PLAYER_SHOOTGUN);

	characterBullets.push_back(topBullet);
	characterBullets.push_back(midBullet);
	characterBullets.push_back(botBullet);

	ammoMap.find(currentWeapon)->second--;
}

void Player::AkFire(int orientation)
{
	weaponAK.play();

	auto bullet = new Bullet(GetCharacterPos(), akDemage, orientation, shootRange, BulletType::PLAYER_AK);
	characterBullets.push_back(bullet);
	ammoMap.find(currentWeapon)->second--;
}

void Player::RotateDeadBody(float elapsedTime)
{
	bodyShape.rotate(DEAD_ROTATION * elapsedTime / GAME_OVER_DURATION);
}

void Player::CreateCopy()
{
	std::cout << "create player copy" << std::endl;
	copy_ak_demage = akDemage;
	copy_shootgun_demage = shootgunDemage;
	copy_ak_ammo = ammoMap.find(Weapon::AK)->second;
	copy_shootgun_ammo = ammoMap.find(Weapon::SHOOTGUN)->second;
	copy_maxHealth = maxHealth;
	copy_health = health;
	copy_boxes = boxes;
}

void Player::ReturnCopy()
{
	std::cout << "return player copy" << std::endl;
	akDemage = copy_ak_demage;
	shootgunDemage = copy_shootgun_demage;
	ammoMap.find(Weapon::AK)->second = copy_ak_ammo;
	ammoMap.find(Weapon::SHOOTGUN)->second = copy_shootgun_ammo;
	maxHealth = copy_maxHealth;
	health = maxHealth; // all levels start with max health
	bodyShape.setRotation(0);
	existStatus = ExistenceStatus::LIVE;
	boxes = copy_boxes;
}

void Player::Clear()
{
	boxes = 0;
	health = maxHealth; // all levels start with max health
}