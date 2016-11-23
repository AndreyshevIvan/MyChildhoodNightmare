#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

const sf::Vector2f PLAYER_SIZE = { 75, 90 };
const sf::Vector2f PLAYER_COLLISION_SHAPE_SIZE = { PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y - 10 };
const sf::Vector2f PLAYER_SPAWN_POS = { 350 , 700 };
const std::string PLAYER_FILE_NAME = "player.png";

const float PLAYER_MOVE_SPEED = 300;
const float PLAYER_ACCELERATION = 200;
const float PLAYER_JUMP_IMPULSE = 0.1;
const float PLAYER_JUMP_SPEED = 200;

enum ExistenceStatus
{
	NOT_SPAWNED,
	LIVE,
	DEATH
};

enum MoveingStatus
{
	RUN_LEFT,
	RUN_RIGHT,
	NOT_RUN
};

enum JumpingStatus
{
	FLY,
	ON_GROUND
};

enum SeatStatus
{
	SEAT,
	NOT_SEAT
};

struct Player
{
	sf::Texture playerTexture;
	MoveingStatus playerRunStatus;
	JumpingStatus playerJumpStatus;
	SeatStatus playerSeatStatus;
	ExistenceStatus playerExistStatus;
	sf::RectangleShape playerShape;
	sf::Vector2f playerSpeed;
	bool isPlayerOnGround;
	sf::FloatRect playerMoveRect;

	bool InitPlayer();
	sf::Vector2f GetCharacterPos();
	void DrawCharacter(sf::RenderWindow& window);
	void Jump(float const& elapsedTime);
	void Seat(float const& elapsedTime);
	void GoLeft(float const& elapsedTime);
	void GoRight(float const& elapsedTime);
	void MovePlayer(float const& elapsedTime);
};
