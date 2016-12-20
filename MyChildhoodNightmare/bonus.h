#pragma once
#include "stdafx.h"

const int SPELL_COUNT = 4;
const int BONUS_COUNT = 3;
const int BONUS_PROBABILITY = 70;

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
};

struct Bonus
{
	Bonus(sf::Vector2f const& position);

	sf::RectangleShape bodyShape;
	sf::Texture bonusTexture;
	sf::FloatRect collisionRect;

	SpellType spellType = SpellType::NONE;
	BonusType bonusType;

	void Draw(sf::RenderWindow& window);
};