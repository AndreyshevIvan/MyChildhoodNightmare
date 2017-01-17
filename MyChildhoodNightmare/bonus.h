#pragma once
#include "stdafx.h"
#include "player.h"

const sf::Vector2f BONUS_BODY_SIZE = { 40, 40 };

const int BONUS_COUNT = 4;
const int BONUS_WEAPON_COUNT = 2;

const int EASY_BONUS_PROBABILITY = 75;
const int NORMAL_BONUS_PROBABILITY = 50;
const int HARD_BONUS_PROBABILITY = 30;

const float BONUS_FALL_SPEED = 200;

const int BONUS_AK_AMMO_COUNT = 14;
const int BONUS_SHOOTGUN_AMMO_COUNT = 8;
const int BONUS_HEALTH_COUNT = 20;

const int BONUS_HEALTH_INCREASE = 10;
const int BONUS_AK_DEMAGE_INCREASE = 2;
const int BONUS_SHOOTGUN_DEMAGE_INCREASE = 2;

const sf::Vector2f ITEM_BOX_SIZE = { 58, 50 };

enum struct BonusType
{
	AK_AMMO,
	SHOOTGUN_AMMO,
	HEALTH,
	GIFT,
	ITEM_BOX,
	RANDOMIZE,
};

struct Bonus : BonusesSound
{
	Bonus(sf::Vector2f const& position, BonusType const& type = BonusType::RANDOMIZE);

	sf::RectangleShape bodyShape;
	sf::Texture bonusTexture;
	sf::FloatRect collisionRect;

	BonusType bonusType;
	float fallSpeed = 0;
	bool IsBonusOnGround = false;
	std::string announcementText = "";

	void Update(float elapsedTime, std::vector<TmxObject> const& blocks);
	bool AddBonusEffect(Player& player);
	void AddRandomBonus(Player& player);

	void DrawBonus(sf::RenderWindow& window);
};

void DropBonusFromEnemy(sf::Vector2f const& position, std::vector<Bonus*>& bonuses, int probability);
bool AddPropertyValue(int& ammo, int addedAmmo, int maxAmmo);