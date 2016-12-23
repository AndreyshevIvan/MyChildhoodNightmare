#pragma once
#include "stdafx.h"
#include "character.h"
#include "player.h"

const sf::Vector2f HAND_SIZE = { 3, 3 };
const float HAND_MARGIN_X = 10;
const float MAX_IDLE_WALKING_COLDOWN = 1.75;

const float SHADOW_START_HEALTH = 210;
const float SHADOW_MOVE_SPEED = 220;
const float SHADOW_DEMAGE = 25;

const float CLOWN_START_HEALTH = 100;
const float CLOWN_MOVE_SPEED = 0;
const float CLOWN_TOUCH_DEMAGE = 6;
const float CLOWN_MIN_TARGET_RANGE = 600;
const float CLOWN_BULLET_DEMAGE = 2;
const float CLOWN_SHOOT_COLDOWN = 0.2f;
const float CLOWN_SHOOT_SERIES_COLDOWN = 1.2f;

const float BIRD_START_HEALTH = 100;
const float BIRD_MOVE_SPEED = 150;
const float BIRD_DEMAGE = 6;

enum struct EnemyType
{
	NONE,
	SHADOW,
	CLOWN,
	BIRD,
	BOSS,
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
	void CreateBird();

	sf::FloatRect position;
	
	float idleWalkingColdown = 0;
	MovementStatus currentRunStatus;

	sf::RectangleShape handLeftTop;
	sf::RectangleShape handLeftMiddle;
	sf::RectangleShape handLeftBottom;

	sf::RectangleShape handRightTop;
	sf::RectangleShape handRightMiddle;
	sf::RectangleShape handRightBottom;

	EnemyType enemyType = EnemyType::NONE;
	EnemyActivity activityStatus = EnemyActivity::IDLE;

	std::function<void(float elapsedTime, std::vector<Object> const& objects)> Idle;
	std::function<void(Player const& player)> Pursuit;

	void Update(float elapsedTime, Player const& player, std::vector<Object> const& objects);
	void UpdateHands();

	void UpdateActivityStatus(Player const& player);
	void UpdateBirdActivityStatus(Player const& player);
	void UpdateShadowActivityStatus(Player const& player);
	void UpdateClownActivityStatus(Player const& player);
	void UpdateBossActivityStatus(Player const& player);

	void ShadowIdle(float elapsedTime, std::vector<Object> const& objects);

	void ClownShoot(Player const& player);

	void BirdPursuite(float elapsedTime, std::vector<Object> const& mapTiles);

	void Draw(sf::RenderWindow& window) override;
};