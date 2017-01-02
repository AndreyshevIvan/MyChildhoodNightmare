#pragma once
#include "stdafx.h"
#include "character.h"
#include "player.h"

const sf::Vector2f HAND_SIZE = { 2, 2 };
const float HAND_MARGIN_X = 10;
const float PI = 3.14f;

const sf::Vector2f SHADOW_SIZE = { 50, 100 };
const int SHADOW_START_HEALTH = 210;
const float SHADOW_MOVE_SPEED = 180;
const float SHADOW_MOVE_SPEED_RANDOM = 80;
const int SHADOW_DEMAGE = 25;

const sf::Vector2f CLOWN_SIZE = { 55, 100 };
const int CLOWN_START_HEALTH = 200;
const float CLOWN_MOVE_SPEED = 0;
const int CLOWN_TOUCH_DEMAGE = 6;
const float CLOWN_TARGET_RANGE = 500;
const float CLOWN_SHOOT_RANGE = CLOWN_TARGET_RANGE;
const int CLOWN_BULLET_DEMAGE = 3;
const float CLOWN_SHOOT_COLDOWN = 0.2f;

const sf::Vector2f GHOST_SIZE = { 80, 70 };
const int GHOST_START_HEALTH = 100;
const float GHOST_TARGET_RANGE = 700;
const float GHOST_MOVE_SPEED = 110;
const int GHOST_DEMAGE = 6;

const sf::Vector2f SPIDER_SIZE = { 50, 50 };
const int SPIDER_START_HEALTH = 70;
const float SPIDER_TARGET_RANGE = 200;
const float SPIDER_MOVE_SPEED = 130;
const int SPIDER_DEMAGE = 6;

const sf::Vector2f BOSS_SIZE = { 160, 250 };
const int BOSS_START_HEALTH = 2000;
const int BOSS_DEMAGE = 2;
const float BOSS_MOVE_SPEED_IDLE = 100;
const float BOSS_MOVE_SPEED_PURSUITE = 300;

const int LAVA_DEMAGE = 20;

enum struct EnemyType
{
	NONE,
	SHADOW,
	CLOWN,
	GHOST,
	SPIDER,
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
	void CreateGhost();
	void CreateSpider();
	void CreateBoss();

	sf::Vector2f position;
	sf::RectangleShape targetArea;
	
	sf::Vector2f ghostMove;

	MovementStatus currentRunStatus;

	sf::FloatRect handLeftMiddle;
	sf::FloatRect handLeftBottom;

	sf::FloatRect handRightMiddle;
	sf::FloatRect handRightBottom;

	EnemyType enemyType = EnemyType::NONE;
	EnemyActivity activityStatus = EnemyActivity::IDLE;

	std::function<void(float elapsedTime, std::vector<Object> const& blocks)> Idle;
	std::function<void(Character const& player, std::vector<Bullet*>& bullets)> Pursuit;

	void UpdateAI(float elapsedTime, Character const& player, std::vector<Object> const& blocks, std::vector<Bullet*>& bullets);
	void UpdateHands();

	void UpdateActivityStatus(Character const& player);
	void UpdateGhostActivityStatus(Character const& player);
	void UpdateShadowActivityStatus(Character const& player);
	void UpdateClownActivityStatus(Character const& player);
	void UpdateSpiderActivityStatus(Character const& player);
	void UpdateBossActivityStatus(Character const& player);

	void ShadowIdle(float elapsedTime, std::vector<Object> const& blocks);

	void ClownShoot(Character const& player, std::vector<Bullet*>& bullets);
	bool IsClownOnGround = false;

	void UpdateSpiderPos(float elapsedTime);
	void SpiderPursuite(Character const& player);

	void UpdateGhostPos(float elapsedTime);
	void GhostIdle(float elapsedTime);
	void GhostPursuite(Character const& player);
};