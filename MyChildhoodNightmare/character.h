#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "bullets.h"
#include "tinyxml/level.h"

const float G = 750;
const float FLYING_SLOWDOWN = 0.6f;

enum CharacterType
{
	PLAYER,
	FRIEND_BEAR,
	ENEMY_SHADOW,
	ENEMY_SPIDER,
	ENEMY_GOBLIN,
	ENEMY_CLOWN,
	BOSS_SISTER,
};

enum ExistenceStatus
{
	NOT_SPAWNED,
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

	MovementStatus runStatus;
	JumpingStatus jumpStatus;
	ExistenceStatus existStatus;
	OrientationStatus orientationStatus;

	float moveSpeed;
	float jumpSpeed;
	float weaponDemage;
	float demage;
	float health;
	float adoptedDemage;
	float shootColdown;

	sf::Vector2f GetCharacterPos(); 
	
	bool IsCollidesWithLevel(sf::FloatRect const& rect, std::vector<Object> const& mapTiles);

	void Jump();
	void UpdatePos(float elapsedTime, std::vector<Object> const& mapTiles);
	void UpdateGravity(float elapsedTime, std::vector<Object> const& mapTiles);
};