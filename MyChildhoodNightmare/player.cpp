#include "player.h"
#include <iostream>

using namespace std;
using namespace sf;

bool Player::InitPlayer()
{
	if (!cTexture.loadFromFile("resources/" + PLAYER_FILE_NAME, sf::IntRect(0, 0, PLAYER_SIZE.x, PLAYER_SIZE.y)))
	{
		return false;
	}

	cTexture.setSmooth(true);
	cSprite.setTexture(cTexture);
	cSprite.setOrigin(PLAYER_SIZE.x / 2.0, PLAYER_SIZE.y);
	cSprite.setPosition(PLAYER_SPAWN_POS);

	cCollisionShape.setSize({ PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y - 10 });
	cCollisionShape.setOrigin(cCollisionShape.getSize().x / 2.0f, cCollisionShape.getSize().y);
	cCollisionShape.setPosition(PLAYER_SPAWN_POS);
	cCollisionShape.setFillColor(sf::Color::Green);

	cMoveStatus = IDLE;
	cExistenceStatus = NOT_SPAWNED;

	return true;
}

void Player::DrawCharacter(RenderWindow& window)
{
	window.draw(cSprite);
	window.draw(cCollisionShape);
}

void Player::Jump(float const& elapsedTime)
{
	cSprite.move(0, -PLAYER_MOVE_SPEED * elapsedTime);
}

void Player::Seat(float const& elapsedTime)
{
	cSprite.move(0, PLAYER_MOVE_SPEED * elapsedTime);
}

void Player::GoLeft(float const& elapsedTime)
{
	cSprite.move(-PLAYER_MOVE_SPEED * elapsedTime, 0);
}

void Player::GoRight(float const& elapsedTime)
{
	cSprite.move(PLAYER_MOVE_SPEED * elapsedTime, 0);
}

sf::Vector2f Player::GetCharacterPos()
{
	return cSprite.getPosition();
}