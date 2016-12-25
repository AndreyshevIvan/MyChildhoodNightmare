#include "bonus.h"

using namespace std;

Bonus::Bonus(sf::Vector2f const& position)
{
	int spellId = 0;
	int bonusId = rand() % BONUS_COUNT;

	if ((bonusType = BonusType(bonusId)) == BonusType::SPELL)
	{
		spellId = rand() % SPELL_COUNT;
	}

	switch (bonusType)
	{
	case BonusType::AMMO:
		bonusTexture.loadFromFile("resources/bonus_ammo.png");
		ammoType = AmmoType(rand() % BONUS_WEAPON_COUNT);
		break;
	case BonusType::HEALTH:
		bonusTexture.loadFromFile("resources/bonus_hp.png");
		break;
	case BonusType::SPELL:
		bonusTexture.loadFromFile("resources/bonus_spell.png");
		spellType = SpellType(spellId);
		break;
	case BonusType::RANDOM:
		bonusTexture.loadFromFile("resources/bonus_random.png");
		break;
	default:
		break;
	}

	bodyShape.setSize(BONUS_BODY_SIZE);
	bodyShape.setTexture(&bonusTexture);
	collisionRect.width = BONUS_BODY_SIZE.x / 2.0f;
	collisionRect.height = BONUS_BODY_SIZE.y / 2.0f;

	collisionRect.left = position.x;
	collisionRect.top = position.y - BONUS_BODY_SIZE.y;
}

Bonus::Bonus(sf::Vector2f const& position, Items item)
{
	if (item == Items::BOX)
	{
		bonusType = BonusType::ITEM_BOX;
		bonusTexture.loadFromFile("resources/box.png");
		bodyShape.setTexture(&bonusTexture);
		bodyShape.setSize(ITEM_BOX_SIZE);
	}

	collisionRect.width = ITEM_BOX_SIZE.x / 2.0f;
	collisionRect.height = ITEM_BOX_SIZE.y / 2.0f;

	collisionRect.left = position.x - ITEM_BOX_SIZE.x;
	collisionRect.top = position.y - ITEM_BOX_SIZE.y;
}


void Bonus::Update(float elapsedTime, std::vector<Object> const& blocks)
{
	if (!IsBonusOnGround)
	{
		float rectCenterX = collisionRect.left - collisionRect.width / 2.0f;
		float rectCenterY = collisionRect.top - collisionRect.height / 2.0f;

		bodyShape.setPosition({ rectCenterX , rectCenterY });

		for (auto block : blocks)
		{
			if (block.rect.intersects(collisionRect))
			{
				IsBonusOnGround = true;
			}
		}

		if (!IsBonusOnGround)
		{
			float movement = fallSpeed;

			fallSpeed = fallSpeed + G * elapsedTime;
			movement = fallSpeed * elapsedTime;
			collisionRect.top += movement;
		}
	}
}

void Bonus::DrawBonus(sf::RenderWindow& window)
{
	window.draw(bodyShape);
}

void CreateBonus(sf::Vector2f const& position, std::vector<Bonus*>& bonuses)
{
	int probability = rand() % 100;

	if (probability < BONUS_PROBABILITY)
	{
		bonuses.push_back(new Bonus(position));
	}
	cout << probability << "\n";
}