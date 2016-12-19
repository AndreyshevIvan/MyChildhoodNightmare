#pragma once
#include "stdafx.h"
#include "character.h"
#include "player.h"

const sf::Vector2f HAND_SIZE = { 3, 3 };
const float HAND_MARGIN_X = 10;
const float MAX_TARGET_RANGE = 300;
const float MIN_TARGET_RANGE = 30;

const std::string SHADOW_SPAWN_NAME = "enemy_shadow_spawn";
const sf::Vector2f SHADOW_SIZE = { 50 , 100 };
const float SHADOW_START_HEALTH = 100;
const float SHADOW_MOVE_SPEED = 200;

const std::string CLOWN_SPAWN_NAME = "enemy_clown_spawn";
const sf::Vector2f CLOWN_SIZE = { 55 , 100 };
const float CLOWN_START_HEALTH = 100;
const float CLOWN_MOVE_SPEED = 200;

enum struct EnemyType
{
	NONE,
	SHADOW,
	CLOWN,
};

enum struct EnemyActivity 
{
	IDLE,
	PURSUIT,
};

struct Enemy : Character
{
	Enemy(sf::FloatRect const& posRect = sf::FloatRect(0, 0, 0, 0), EnemyType const& type = EnemyType::NONE);

	void CreateShadow();
	void CreateClown();

	sf::FloatRect position;
	float targetRange;

	sf::RectangleShape handLeftTop;
	sf::RectangleShape handLeftMiddle;
	sf::RectangleShape handLeftBottom;

	sf::RectangleShape handRightTop;
	sf::RectangleShape handRightMiddle;
	sf::RectangleShape handRightBottom;

	EnemyType enemyType = EnemyType::NONE;
	EnemyActivity activityStatus = EnemyActivity::IDLE;

	std::function<void()> Idle;
	std::function<void(Player const& player)> Pursuit;

	void Update(float elapsedTime, Player const& player, std::vector<Object> const& objects);
	void ShadowPursuit(Player const& player);
	void UpdateHands();
	void LookGround(std::vector<Object> const& objects);
	void UpdateActivityStatus(Player const& player);
	void UpdateAI();

	void Draw(sf::RenderWindow& window) override;
};