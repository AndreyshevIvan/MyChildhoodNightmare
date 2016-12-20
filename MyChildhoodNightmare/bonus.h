#pragma once
#include "stdafx.h"

const int SPELL_COUNT = 4;
const int BONUS_COUNT = 4;

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
	sf::FloatRect collisionShape;

	SpellType spellType = SpellType::NONE;
	BonusType bonusType;

	void Draw(sf::RenderWindow& window);
};