#include "player.h"

using namespace std;
using namespace sf;

bool Player::InitPlayer()
{
	if (!pTexture.loadFromFile("resources/player.png", sf::IntRect(0, 0, PLAYER_SIZE.x, PLAYER_SIZE.y)))
		return false;

	pTexture.setSmooth(true);
	pSprite.setTexture(pTexture);
	pSprite.setOrigin(PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y);
	pSprite.setPosition(600, 400);

	pCollisionShape.setSize(PLAYER_COLLISION_SHAPE_SIZE);
	pCollisionShape.setOrigin(PLAYER_COLLISION_SHAPE_SIZE.x / 2.0f, PLAYER_COLLISION_SHAPE_SIZE.y);
	pCollisionShape.setPosition(GetPlayerPos());
	pCollisionShape.setFillColor(Color::Green);

	return true;
}

void Player::DrawPlayer(RenderWindow& window)
{
	window.draw(pSprite);
	window.draw(pCollisionShape);
}

void Player::Jump()
{
	pSprite.move(0, -PLAYER_MOVE_SPEED);
}

void Player::Seat()
{
	pSprite.move(0, PLAYER_MOVE_SPEED);
}

void Player::GoLeft()
{
	pSprite.move(-PLAYER_MOVE_SPEED, 0);
}

void Player::GoRight()
{
	pSprite.move(PLAYER_MOVE_SPEED, 0);
}

sf::Vector2f Player::GetPlayerPos()
{
	return pSprite.getPosition();
}