#pragma once
#include "stdafx.h"

const int SPELL_COUNT = 4;
const sf::Vector2f BONUS_BODY_SIZE = { 40, 40 };
const int BONUS_COUNT = 3;
const int BONUS_PROBABILITY = 70;
const float BONUS_FALL_SPEED = 200;

enum struct SpellType
{
	NONE,
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
};

struct Bonus
{
	Bonus(sf::Vector2f const& position);

	sf::RectangleShape bodyShape;
	sf::Texture bonusTexture;
	sf::FloatRect collisionRect;

	SpellType spellType = SpellType::NONE;
	BonusType bonusType;

	float fallSpeed = 0;

	void Update(float elapsedTime, std::vector<Object> const& blocks);

	void Draw(sf::RenderWindow& window);
};