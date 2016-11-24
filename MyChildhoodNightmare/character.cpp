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
	shootColdown = PLAYER_SHOOT_COLDOWN;

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