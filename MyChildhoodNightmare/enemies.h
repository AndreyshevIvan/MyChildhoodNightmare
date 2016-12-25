#pragma once
#include "stdafx.h"
#include "character.h"
#include "player.h"

const sf::Vector2f HAND_SIZE = { 2, 2 };
const float HAND_MARGIN_X = 10;
const float PI = 3.14f;

const float SHADOW_START_HEALTH = 210;
const float SHADOW_MOVE_SPEED = 180;
const float SHADOW_MOVE_SPEED_RANDOM = 80;
const float SHADOW_DEMAGE = 25;

const float CLOWN_START_HEALTH = 100;
const float CLOWN_MOVE_SPEED = 0;
const float CLOWN_TOUCH_DEMAGE = 6;
const float CLOWN_TARGET_RANGE = 700;
const float CLOWN_BULLET_DEMAGE = 3;
const float CLOWN_SHOOT_COLDOWN = 0.2f;

const float BIRD_START_HEALTH = 100;
const float BIRD_TARGET_RANGE = 1000;
const float BIRD_MOVE_SPEED = 80;
const float BIRD_DEMAGE = 6;

const float LAVA_DEMAGE = 20;

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
	Enemy(sf::Vector2f const& posRect = sf::Vector2f(0, 0), EnemyType const& type = EnemyType::NONE);

	void CreateShadow();
	void CreateClown();
	void CreateBird();

	sf::Vector2f position;
	sf::RectangleShape targetArea;
	
	sf::Vector2f birdMove;

	MovementStatus currentRunStatus;

	sf::RectangleShape handLeftTop;
	sf::RectangleShape handLeftMiddle;
	sf::RectangleShape handLeftBottom;

	sf::RectangleShape handRightTop;
	sf::RectangleShape handRightMiddle;
	sf::RectangleShape handRightBottom;

	EnemyType enemyType = EnemyType::NONE;
	EnemyActivity activityStatus = EnemyActivity::IDLE;

	std::function<void(float elapsedTime, std::vector<Object> const& blocks)> Idle;
	std::function<void(Player const& player, std::vector<Bullet*>& bullets)> Pursuit;

	void UpdateAI(float elapsedTime, Player const& player, std::vector<Object> const& blocks, std::vector<Bullet*>& bullets);
	void UpdateHands();

	void UpdateActivityStatus(Player const& player);
	void UpdateBirdActivityStatus(Player const& player);
	void UpdateShadowActivityStatus(Player const& player);
	void UpdateClownActivityStatus(Player const& player);
	void UpdateBossActivityStatus(Player const& player);

	void ShadowIdle(float elapsedTime, std::vector<Object> const& blocks);

	void ClownShoot(Player const& player, std::vector<Bullet*>& bullets);

	void UpdateBirdPos(float elapsedTime, std::vector<Object> const& blocks);
	void BirdIdle(float elapsedTime);
	void BirdPursuite(Player const& player);
};