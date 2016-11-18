#include "player.h"

using namespace std;
using namespace sf;

bool Player::InitPlayer()
{
	playerShape.setSize(PLAYER_SIZE);
	playerShape.setPosition(PLAYER_START_POS);
	playerShape.setFillColor(Color::Blue);

	return true;
}

void Player::DrawPlayer(RenderWindow& window)
{
	window.draw(playerShape);
}

void Player::Jump()
{
	playerShape.move(0, PLAYER_MOVE_SPEED);
}

void Player::Seat()
{
	playerShape.move(0, -PLAYER_MOVE_SPEED);
}

void Player::GoLeft()
{
	playerShape.move(-PLAYER_MOVE_SPEED, 0);
}

void Player::GoRight()
{
	playerShape.move(PLAYER_MOVE_SPEED, 0);
}