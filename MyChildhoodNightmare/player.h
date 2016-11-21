#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

const sf::Vector2f PLAYER_SIZE = { 75, 90 };
const sf::Vector2f PLAYER_COLLISION_SHAPE_SIZE = { PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y - 10 };
const sf::Vector2f PLAYER_SPAWN_POS = { 200 , 300 };
const float PLAYER_MOVE_SPEED = 350;
const std::string PLAYER_FILE_NAME = "player.png";

enum ExistenceStatus
{
	NOT_SPAWNED,
	LIVE,
	DEATH
};

enum MoveingStatus
{
	IDLE,
	RUN,
	FLY,
	SEAT
};

struct Player
{
	sf::Texture cTexture;
	MoveingStatus cMoveStatus;
	ExistenceStatus cExistenceStatus;
	sf::Sprite cSprite;
	sf::RectangleShape cCollisionShape;

	bool InitPlayer();
	sf::Vector2f GetCharacterPos();
	void DrawCharacter(sf::RenderWindow& window);
	void Jump(float const& elapsedTime);
	void Seat(float const& elapsedTime);
	void GoLeft(float const& elapsedTime);
	void GoRight(float const& elapsedTime);
};
