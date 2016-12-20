#include "bonus.h"

using namespace std;

Bonus::Bonus(sf::Vector2f const& position)
{
	int bonusId = rand() % BONUS_COUNT;
	bonusType = BonusType(bonusId);
	int spellId = rand() % SPELL_COUNT + 1;

	cout << "bonus: " << bonusId << " spell: " << spellId;

	switch (bonusType)
	{
	case BonusType::AMMO:
		bonusTexture.loadFromFile("resources/bonus_ammo.png");
		cout << "0";
		break;
	case BonusType::HEALTH:
		bonusTexture.loadFromFile("resources/bonus_hp.png");
		cout << "1";
		break;
	case BonusType::SPELL:
		bonusTexture.loadFromFile("resources/bonus_spell.png");
		cout << "2";
		spellType = SpellType(spellId);
		break;
	case BonusType::RANDOM:
		bonusTexture.loadFromFile("resources/bonus_random.png");
		cout << "3";
		break;
	default:
		break;
	}

	bodyShape.setTexture(&bonusTexture);
	const sf::Vector2f BODY_SIZE = {
		static_cast<float>(bonusTexture.getSize().x),
		static_cast<float>(bonusTexture.getSize().y)
	};

	bodyShape.setSize(BODY_SIZE);
	collisionShape.width = BODY_SIZE.x;
	collisionShape.height = BODY_SIZE.y;

	bodyShape.setPosition(position - BODY_SIZE);
	collisionShape.left = position.x - BODY_SIZE.x;
	collisionShape.top = position.y - BODY_SIZE.y;
}

void Bonus::Draw(sf::RenderWindow& window)
{
	window.draw(bodyShape);
}