#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "bullets.h"
#include "tinyxml/level.h"

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
	ExistenceStatus existStatus;
	Orientation orientation;

	float moveSpeed;
	float jumpSpeed;
	float jumpHeight;
	float weaponDemage;
	float adoptedDemage;
	float shootColdown;

	sf::Vector2f GetCharacterPos();

	void Jump();
};