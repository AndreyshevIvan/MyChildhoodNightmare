#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

const float PLAYER_SHOOT_COLDOWN = 0.3;

enum ExistenceStatus
{
	NOT_SPAWNED,
	LIVE,
	DEAD
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

enum Orientation
{
	LEFT = 1,
	RIGHT
};

struct Character
{
	sf::Texture bodyTexture;
	sf::RectangleShape bodyShape;
	sf::RectangleShape collisionShape;

	MoveingStatus runStatus;
	JumpingStatus jumpStatus;
	SeatStatus seatStatus;
	ExistenceStatus existStatus;
	Orientation orientation;

	float moveSpeed;
	float jumpSpeed;
	float jumpHeight;

	float shootColdown;

	bool InitCharacter(std::string const& textureFileName, sf::Vector2f const& spawnPos, sf::Vector2f const& size, float speed, float jumpH);

	sf::Vector2f GetCharacterPos();

	void Jump();
	void Seat();
};