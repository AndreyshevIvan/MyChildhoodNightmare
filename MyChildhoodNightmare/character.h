#pragma once
#include "stdafx.h"
#include "bullets.h"
#include "sound.h"
#include "temporaryObjects.h"

const float FLYING_SLOWDOWN = 0.6f;
const float CHARACTERS_JUMP_SPEED = 400;
const float CHARACTERS_JUMP_HEIGHT = 80;
const float MIN_SHOOT_RANGE = 140;

const float CRITICAL_JUMP_SPEED = 700;
const float SPEED_PER_DEMAGE = 160;

const float TIME_TO_FRAME = 0.08f;
const int FRAMES_COUNT = 6;

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

struct Character : CharacterSound
{
	sf::Texture bodyTexture;
	sf::RectangleShape bodyShape;
	sf::FloatRect collisionRect;

	MovementStatus runStatus = MovementStatus::NOT_RUN;
	JumpingStatus jumpStatus;
	ExistenceStatus existStatus = ExistenceStatus::LIVE;
	OrientationStatus orientationStatus = OrientationStatus::RIGHT;

	std::vector<Bullet*> characterBullets;
	std::vector<Blood*> wounds;
	std::vector<GunFire*> gunFire;

	float moveSpeed;
	float jumpSpeed = 0;
	float weaponDemage;
	int touchDemage;
	int shootDemage;
	int health;
	int maxHealth;
	float shootColdown = 0;
	float shootRange = MIN_SHOOT_RANGE;
	sf::Music* deathSound = nullptr;
	void Spawn(sf::Vector2f const& pos);

	float animateTime = 0;

	sf::Vector2f GetCharacterPos() const;
	
	bool IsCollidesWithLevel(sf::FloatRect const& rect, std::vector<TmxObject> const& mapTiles);

	void Jump();

	void UpdatePos(float elapsedTime, std::vector<TmxObject> const& mapTiles);
	void UpdateGravity(float elapsedTime, std::vector<TmxObject> const& mapTiles);
	void UpdateHealthStatus();
	void UpdateOrientation();
	
	virtual void UpdateTexture(float elapsedTime);

	virtual void DrawCharacter(sf::RenderWindow& window);
};