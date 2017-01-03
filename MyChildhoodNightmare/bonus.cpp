#include "bonus.h"

using namespace std;

Bonus::Bonus(sf::Vector2f const& position, ItemType const& type)
{
	if (type == ItemType::BONUS)
	{
		bonusType = BonusType(rand() % BONUS_COUNT);

		switch (bonusType)
		{
		case BonusType::AK_AMMO:
			bonusTexture.loadFromFile("resources/bonus_ammo.png");
			break;
		case BonusType::SHOOTGUN_AMMO:
			bonusTexture.loadFromFile("resources/bonus_ammo.png");
			break;
		case BonusType::HEALTH:
			bonusTexture.loadFromFile("resources/bonus_hp.png");
			break;
		case BonusType::RANDOM:
			bonusTexture.loadFromFile("resources/bonus_random.png");
			break;
		default:
			break;
		}

		bodyShape.setSize(BONUS_BODY_SIZE);
	}
	else if (type == ItemType::BOX)
	{
		bonusTexture.loadFromFile("resources/box.png");
		bodyShape.setSize(ITEM_BOX_SIZE);
	}

	bodyShape.setTexture(&bonusTexture);

	const sf::Vector2f POSITION(position.x, position.y - BONUS_BODY_SIZE.y);
	const sf::Vector2f SIZE = bodyShape.getSize() * 0.5f;
	collisionRect = sf::FloatRect(POSITION, SIZE);
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

void DropBonusFromEnemy(sf::Vector2f const& position, std::vector<Bonus*>& bonuses, int probability)
{
	int digit = rand() % 100;

	if (digit < probability)
	{
		bonuses.push_back(new Bonus(position, ItemType::BONUS));
	}
}

bool Bonus::AddBonusEffect(Player& player)
{
	switch (bonusType)
	{
	case BonusType::AK_AMMO:
		if (!AddPropertyValue(player.ammo[AK], BONUS_AK_AMMO_COUNT, PLAYER_MAX_AMMO))
		{
			return false;
		}
		announcementText = "+ " + to_string(BONUS_AK_AMMO_COUNT) + " AK AMMO";
		break;

	case BonusType::SHOOTGUN_AMMO:
		if (!AddPropertyValue(player.ammo[SHOOTGUN], BONUS_SHOOTGUN_AMMO_COUNT, PLAYER_MAX_AMMO))
		{
			return false;
		}
		announcementText = "+ " + to_string(BONUS_SHOOTGUN_AMMO_COUNT) + " SHOOTGUN AMMO";
		break;

	case BonusType::HEALTH:
		if (!AddPropertyValue(player.health, BONUS_HEALTH_COUNT, PLAYER_START_HEALTH))
		{
			return false;
		}
		announcementText = "+ " + to_string(BONUS_HEALTH_COUNT) + "HEALTH";
		break;

	case BonusType::ITEM_BOX:
		player.boxes++;
		announcementText = "YOU FOUND ONE BOX!";
		break;
	default:
		break;
	}

	return true;
}

void Bonus::DrawBonus(sf::RenderWindow& window)
{
	window.draw(bodyShape);
}

bool AddPropertyValue(int& ammo, int addedAmmo, int maxAmmo)
{
	if (ammo == maxAmmo)
	{
		return false;
	}

	while (addedAmmo > 0 && ammo < maxAmmo)
	{
		ammo++;
		addedAmmo--;
	}

	return true;
}