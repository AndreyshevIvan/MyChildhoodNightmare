#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

const float FLYING_SLOWDOWN = 0.55;

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

enum GameStatus
{
	START_MENU,
	CHENGE_LEVEL,
	PLAY,
	PAUSE,
	GAME_OVER
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

	float moveSpeed;
	float moveVelocity;
	float jumpHeight;

	bool InitCharacter(std::string const& textureFileName, sf::Vector2f const& spawnPos, sf::Vector2f const& size, float speed, float jumpH);

	sf::Vector2f GetCharacterPos();

	void Jump();
	void Seat();
};