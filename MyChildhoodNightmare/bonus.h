#pragma once
#include "stdafx.h"

const sf::Vector2f BONUS_BODY_SIZE = { 40, 40 };

const int SPELL_COUNT = 3;
const int BONUS_COUNT = 4;
const int BONUS_WEAPON_COUNT = 2;
const int BONUS_PROBABILITY = 60;
const float BONUS_FALL_SPEED = 200;

const int BONUS_AK_AMMO_COUNT = 24;
const int BONUS_SHOOTGUN_AMMO_COUNT = 14;
const int BONUS_HP_COUNT = 20;

const sf::Vector2f ITEM_BOX_SIZE = { 50, 58 };

enum struct SpellType
{
	BANG,
	GODMODE,
	BONUS,
};

enum struct BonusType
{
	AMMO,
	HEALTH,
	SPELL,
	RANDOM,
	ITEM_BOX,
};

enum struct AmmoType
{
	AK,
	SHOOTGUN,
};

enum struct Items
{
	BOX,
};

struct Bonus
{
	Bonus(sf::Vector2f const& position);
	Bonus(sf::Vector2f const& position, Items item);

	sf::RectangleShape bodyShape;
	sf::Texture bonusTexture;
	sf::FloatRect collisionRect;

	SpellType spellType;
	BonusType bonusType;
	AmmoType ammoType;

	float fallSpeed = 0;
	bool IsBonusOnGround = false;

	void Update(float elapsedTime, std::vector<Object> const& blocks);

	void DrawBonus(sf::RenderWindow& window);
};

void CreateBonus(sf::Vector2f const& position, std::vector<Bonus*>& bonuses);