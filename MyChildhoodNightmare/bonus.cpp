#include "bonus.h"

using namespace std;

Bonus::Bonus(sf::Vector2f const& position)
{
	int bonusId = rand() % BONUS_COUNT + 1;
	bonusType = BonusType(bonusId);
	int spellId = rand() % SPELL_COUNT + 1;

	cout << "bonus: " << bonusId << " spell: " << spellId;

	switch (bonusType)
	{
	case BonusType::AMMO:
		bonusTexture.loadFromFile("resources/bonus_ammo.png");
		break;
	case BonusType::HEALTH:
		bonusTexture.loadFromFile("resources/bonus_hp.png");
		break;
	case BonusType::RANDOM:
		bonusTexture.loadFromFile("resources/bonus_random.png");
		break;
	case BonusType::SPELL:
		bonusTexture.loadFromFile("resources/bonus_spell.png");
		spellType = SpellType(spellId);
		break;
	default:
		break;
	}

	bodyShape.setTexture(&bonusTexture);
	bodyShape.setSize(BONUS_BODY_SIZE);
	collisionRect.width = BONUS_BODY_SIZE.x / 2.0f;
	collisionRect.height = BONUS_BODY_SIZE.y / 2.0f;

	collisionRect.left = position.x - BONUS_BODY_SIZE.x;
	collisionRect.top = position.y - BONUS_BODY_SIZE.y;
}

void Bonus::Update(float elapsedTime, std::vector<Object> const& blocks)
{
	bool isCollideWithLevel = false;

	float rectCenterX = collisionRect.left - collisionRect.width / 2.0f;
	float rectCenterY = collisionRect.top - collisionRect.height / 2.0f;

	bodyShape.setPosition({ rectCenterX , rectCenterY });

	for (auto block : blocks)
	{
		if (block.rect.intersects(collisionRect))
		{
			isCollideWithLevel = true;
		}
	}

	if (!isCollideWithLevel)
	{
		float movement = fallSpeed;

		fallSpeed = fallSpeed + G * elapsedTime;
		movement = fallSpeed * elapsedTime;
		collisionRect.top += movement;
	}
}

void Bonus::Draw(sf::RenderWindow& window)
{
	window.draw(bodyShape);
}