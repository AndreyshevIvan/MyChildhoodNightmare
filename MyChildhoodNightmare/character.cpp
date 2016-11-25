#include "character.h"
#include <iostream>

using namespace std;
using namespace sf;

bool Character::InitCharacter(string const& textureFileName, sf::Vector2f const& spawnPos, sf::Vector2f const& size, float speed, float jumpH)
{
	if (!bodyTexture.loadFromFile("resources/" + textureFileName))
	{
		return false;
	}

	bodyShape.setSize(size);
	bodyShape.setTexture(&bodyTexture);
	bodyShape.setOrigin(size.x / 2.0f, size.y);
	bodyShape.setPosition(spawnPos);

	collisionShape.setSize({ size.x / 2.0f, size.y - 10 });
	collisionShape.setFillColor(sf::Color::Red);
	collisionShape.setOrigin(size.x / 4.0f, size.y - 10);
	collisionShape.setPosition(spawnPos);

	moveSpeed = speed;
	jumpSpeed = 0;
	jumpHeight = jumpH;

	healthBar.setSize({ PLAYER_START_HEALTH, 16 });
	healthBar.setPosition(GetCharacterPos() + PLAYER_BAR_DISPLACEMENT);
	healthBar.setFillColor(sf::Color::Red);

	shootColdown = 0;
    weapon = AK;

	orientation = RIGHT;

	return true;
}

void Character::Jump()
{
	if (jumpStatus == ON_GROUND)
	{
		jumpSpeed = -jumpHeight;
	}
}

void Character::Seat()
{

}

sf::Vector2f Character::GetCharacterPos()
{
	return collisionShape.getPosition();
}

void Character::Shoot(Level const& level)
{
    switch (weapon)
    {
    case CRY:
        if (shootColdown > CRY_COLDOWN)
        {
            bullets.push_back(new Bullet(GetCharacterPos(), level, orientation));
            shootColdown = 0;
        }
        break;
    case AK:
        if (shootColdown > AK_COLDOWN)
        {
            bullets.push_back(new Bullet(GetCharacterPos(), level, orientation));
            shootColdown = 0;
        }
        break;
    case PISTOL:
        if (shootColdown > PISTOL_COLDOWN)
        {
            bullets.push_back(new Bullet({ GetCharacterPos().x, GetCharacterPos().y + 20 }, level, orientation));
            bullets.push_back(new Bullet({ GetCharacterPos().x, GetCharacterPos().y }, level, orientation));
            bullets.push_back(new Bullet({ GetCharacterPos().x, GetCharacterPos().y - 20 }, level, orientation));
            shootColdown = 0;
        }
        break;
    default:
        break;
    }
}