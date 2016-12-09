#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "bullets.h"
#include "tinyxml/level.h"

const float G = 750;
const float FLYING_SLOWDOWN = 0.6f;
const float MIN_HEIGHT_FOR_DEMAGE = 16;
const float DEMAGE_PER_HEIGHT = 0.8f;
const float CHARACTERS_JUMP_SPEED = 400;

const std::string ENEMY_SHADOW_SPAWN_NAME = "enemy_shadow_spawn";
const sf::Vector2f ENEMY_SHADOW_SIZE = { 60 , 100 };
const float ENEMY_SHADOW_START_HEALTH = 100;
const float ENEMY_SHADOW_MOVE_SPEED = 100;
const unsigned POINTS_FOR_KILL_SHADOW = 30;

const std::string ENEMY_CLOWN_SPAWN_NAME = "enemy_clown_spawn";
const sf::Vector2f ENEMY_CLOWN_SIZE = { 60 , 100 };
const float ENEMY_CLOWN_START_HEALTH = 100;
const float ENEMY_CLOWN_MOVE_SPEED = 100;
const unsigned POINTS_FOR_KILL_CLOWN = 30;

//TODO: Паттерны фабрика и прототип

enum CharacterType
{
	NONE,
	SHADOW,
	CLOWN,
};

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
	Character::Character(sf::FloatRect const& posRect = sf::FloatRect(0, 0, 0, 0), CharacterType const& type = NONE);

	void CreateShadow();
	void CreateClown();

	sf::Texture bodyTexture;
	sf::RectangleShape bodyShape;
	sf::FloatRect collisionRect;

	CharacterType charecterType = NONE;
	MovementStatus runStatus;
	JumpingStatus jumpStatus;
	ExistenceStatus existStatus;
	OrientationStatus orientationStatus;

	std::vector<Bullet*> bullets;

	float moveSpeed;
	float jumpSpeed;
	float weaponDemage;
	float demage;
	float bulletsAngle;
	float health;
	float shootColdown;
	float currentFrame;

	void Spawn(Object const& spawnObj);

	sf::Vector2f GetCharacterPos();
	
	bool IsCollidesWithLevel(sf::FloatRect const& rect, std::vector<Object> const& mapTiles);

	void Jump();

	void UpdatePos(float elapsedTime, std::vector<Object> const& mapTiles);
	void UpdateGravity(float elapsedTime, std::vector<Object> const& mapTiles);
	void UpdateHealthStatus();

	void Draw(sf::RenderWindow& window);

	void Clear();
};