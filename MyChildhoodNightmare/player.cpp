#include "player.h"

using namespace std;
using namespace sf;

bool Player::InitPlayer()
{
	if (!playerTexture.loadFromFile("resources/player.png", sf::IntRect(0, 0, PLAYER_SIZE.x, PLAYER_SIZE.y)))
		return false;

	playerTexture.setSmooth(true);
	playerSprite.setTexture(playerTexture);
	playerSprite.setPosition(PLAYER_START_POS);

	return true;
}

void Player::DrawPlayer(RenderWindow& window)
{
	window.draw(playerSprite);
}

void Player::Jump()
{
	playerSprite.move(0, -PLAYER_MOVE_SPEED);
}

void Player::Seat()
{
	playerSprite.move(0, PLAYER_MOVE_SPEED);
}

void Player::GoLeft()
{
	playerSprite.move(-PLAYER_MOVE_SPEED, 0);
}

void Player::GoRight()
{
	playerSprite.move(PLAYER_MOVE_SPEED, 0);
}