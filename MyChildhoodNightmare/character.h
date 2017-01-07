#pragma once
#include "stdafx.h"
#include "bullets.h"
#include "sound.h"

const float FLYING_SLOWDOWN = 0.6f;
const float CHARACTERS_JUMP_SPEED = 400;
const float CHARACTERS_JUMP_HEIGHT = 80;
const float MIN_SHOOT_RANGE = 140;

const float CRITICAL_JUMP_SPEED = 700;
const float SPEED_PER_DEMAGE = 160;

const sf::Vector2f BLOOD_SIZE = { 45, 30 };
const int BLOOD_MAX_ROTATION = 30;
const float BLOOD_MARGIN_X = 16;
const float BLOOD_MARGIN_Y = 6;
const int BLOOD_FRAMES = 10;
const float BLOOD_DURATION = 0.25f;

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

struct Blood
{
	Blood(sf::Vector2f const& characterPos, sf::Vector2f const& bulletPos);

	sf::RectangleShape blood;
	sf::Texture bloodTexture;

	sf::Vector2f positionOffset;
	sf::Vector2f* position;

	float secondsPerFrame;
	float duration;
	int currentFrame;

	void Update(sf::Vector2f const& characterPos, float elapsedTime);
	void Draw(sf::RenderWindow& window);
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

	float moveSpeed;
	float jumpSpeed = 0;
	float weaponDemage;
	int touchDemage;
	int shootDemage;
	int health;
	int maxHealth;
	float shootColdown = 0;
	float currentFrame = 0;
	float shootRange = MIN_SHOOT_RANGE;
	sf::Music* deathSound = nullptr;
	void Spawn(sf::Vector2f const& pos);
	std::vector<Blood*> wounds;

	sf::Vector2f GetCharacterPos() const;
	
	bool IsCollidesWithLevel(sf::FloatRect const& rect, std::vector<TmxObject> const& mapTiles);

	void Jump();

	void UpdatePos(float elapsedTime, std::vector<TmxObject> const& mapTiles);
	void UpdateGravity(float elapsedTime, std::vector<TmxObject> const& mapTiles);
	void UpdateHealthStatus();
	void UpdateOrientation();
	
	void UpdateTexture();

	virtual void DrawCharacter(sf::RenderWindow& window);
};