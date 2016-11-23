#include "player.h"
#include <iostream>

using namespace std;
using namespace sf;

bool Player::InitPlayer()
{
	if (!playerTexture.loadFromFile("resources/" + PLAYER_FILE_NAME))
	{
		return false;
	}
	playerMoveRect.left = PLAYER_SPAWN_POS.x;
	playerMoveRect.top = PLAYER_SPAWN_POS.y;
	playerMoveRect.width = PLAYER_SIZE.x; 
	playerMoveRect.height = PLAYER_SIZE.y;

	playerTexture.setSmooth(true);
	playerShape.setSize(PLAYER_SIZE);
	playerShape.setTexture(&playerTexture);
	playerShape.setPosition(PLAYER_SPAWN_POS);

	playerRunStatus = NOT_RUN;
	playerJumpStatus = FLY;
	playerSeatStatus = NOT_SEAT;
	playerExistStatus = NOT_SPAWNED;

	playerSpeed = { 0, 0 };

	return true;
}

void Player::DrawCharacter(RenderWindow& window)
{
	window.draw(playerShape);
}

void Player::Jump(float const& elapsedTime)
{
	if (isPlayerOnGround)
	{
		playerSpeed.y = -PLAYER_MOVE_SPEED * elapsedTime;
	}
}

void Player::Seat(float const& elapsedTime)
{

}

void Player::GoLeft(float const& elapsedTime)
{
	playerSpeed.x = -PLAYER_MOVE_SPEED * elapsedTime;
	playerRunStatus = RUN_LEFT;
}

void Player::GoRight(float const& elapsedTime)
{
	playerSpeed.x = PLAYER_MOVE_SPEED * elapsedTime;
	playerRunStatus = RUN_RIGHT;
}

sf::Vector2f Player::GetCharacterPos()
{
	return { playerMoveRect.left, playerMoveRect.top };
}

void Player::MovePlayer(float const& elapsedTime)
{
	if (playerRunStatus != NOT_RUN)
	{
		playerMoveRect.left += playerSpeed.x;
		playerRunStatus = NOT_RUN;
	}
	cout << playerSpeed.x << "\n";

	//playerMoveRect.top += playerSpeed.y;

	playerShape.setPosition(GetCharacterPos());
}