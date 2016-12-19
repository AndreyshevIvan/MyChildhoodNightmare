#pragma once
#include "stdafx.h"
#include "bullets.h"

const float G = 750;
const float FLYING_SLOWDOWN = 0.6f;
const float MIN_HEIGHT_FOR_DEMAGE = 16;
const float DEMAGE_PER_HEIGHT = 0.8f;
const float CHARACTERS_JUMP_SPEED = 400;
const float CHARACTERS_JUMP_HEIGHT = 80;

enum ExistenceStatus
{
	LIVE,
	DEAD,
};

enum MovementStatus
{
	RUN_LEFT,
	RUN_RIGHT,
	NOT_RUN,
};

enum JumpingStatus
{
	FLY,
	ON_GROUND,
};

enum OrientationStatus
{
	LEFT = 1,
	RIGHT,
};

struct Character
{
	sf::Texture bodyTexture;
	sf::RectangleShape bodyShape;
	sf::FloatRect collisionRect;

	MovementStatus runStatus = MovementStatus::NOT_RUN;
	JumpingStatus jumpStatus;
	ExistenceStatus existStatus = ExistenceStatus::LIVE;
	OrientationStatus orientationStatus = OrientationStatus::RIGHT;

	std::vector<Bullet*> bullets;

	float moveSpeed;
	float jumpSpeed = 0;
	float weaponDemage;
	float demage;
	float bulletsAngle;
	float health;
	float shootColdown = 0;
	float currentFrame = 0;

	void Spawn(Object const& spawnObj);

	sf::Vector2f GetCharacterPos() const;
	
	bool IsCollidesWithLevel(sf::FloatRect const& rect, std::vector<Object> const& mapTiles);

	void Jump();

	void UpdatePos(float elapsedTime, std::vector<Object> const& mapTiles);
	void UpdateGravity(float elapsedTime, std::vector<Object> const& mapTiles);
	void UpdateHealthStatus();

	virtual void Draw(sf::RenderWindow& window);

	void Clear();
};