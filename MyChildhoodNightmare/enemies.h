#pragma once
#include "stdafx.h"
#include "character.h"
#include "player.h"

const sf::Vector2f HAND_SIZE = { 2, 2 };
const float HAND_MARGIN_X = 10;
const float PI = 3.14f;

const int SHADOW_START_HEALTH = 210;
const float SHADOW_MOVE_SPEED = 180;
const float SHADOW_MOVE_SPEED_RANDOM = 80;
const int SHADOW_DEMAGE = 25;

const int CLOWN_START_HEALTH = 100;
const float CLOWN_MOVE_SPEED = 0;
const int CLOWN_TOUCH_DEMAGE = 6;
const float CLOWN_TARGET_RANGE = 600;
const int CLOWN_BULLET_DEMAGE = 3;
const float CLOWN_SHOOT_COLDOWN = 0.2f;

const int BIRD_START_HEALTH = 100;
const float BIRD_TARGET_RANGE = 1000;
const float BIRD_MOVE_SPEED = 80;
const int BIRD_DEMAGE = 6;

const sf::Vector2f BOSS_SIZE = { 160, 250 };
const int BOSS_START_HEALTH = 100;
const int BOSS_DEMAGE = 2;
const float BOSS_MOVE_SPEED_IDLE = 100;
const float BOSS_MOVE_SPEED_PURSUITE = 300;

const int LAVA_DEMAGE = 20;

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
	void CreateBoss();

	sf::Vector2f position;
	sf::RectangleShape targetArea;
	
	sf::Vector2f birdMove;

	MovementStatus currentRunStatus;

	sf::FloatRect handLeftTop;
	sf::FloatRect handLeftMiddle;
	sf::FloatRect handLeftBottom;

	sf::FloatRect handRightTop;
	sf::FloatRect handRightMiddle;
	sf::FloatRect handRightBottom;

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
	bool IsClownOnGround = false;

	void UpdateBirdPos(float elapsedTime, std::vector<Object> const& blocks);
	void BirdIdle(float elapsedTime);
	void BirdPursuite(Player const& player);
};