#include "character.h"
#include <iostream>

using namespace std;
using namespace sf;

void Character::Jump()
{
	if (jumpStatus == ON_GROUND)
	{
		jumpSpeed = -jumpHeight;
	}
}

sf::Vector2f Character::GetCharacterPos()
{
	return collisionShape.getPosition();
}