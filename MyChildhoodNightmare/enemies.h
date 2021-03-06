#pragma once
#include "stdafx.h"
#include "character.h"

const sf::Vector2f HAND_SIZE = { 2, 2 };

const sf::Vector2f SHADOW_SIZE = { 50, 100 };
const int SHADOW_START_HEALTH = 100;
const float SHADOW_MOVE_SPEED = 180;
const float SHADOW_PURSUITE_MOVE_SPEED = 300;
const float SHADOW_MOVE_SPEED_RANDOM = 80;
const int SHADOW_TOUCH_DEMAGE = 25;
const sf::Vector2f SHADOW_TARGET_AREA_SIZE = { 600.0f, 120.0f };
const sf::Vector2f SHADOW_TARGET_AREA_MARGIN = { 0, -SHADOW_SIZE.y };

const sf::Vector2f CLOWN_SIZE = { 55, 100 };
const int CLOWN_START_HEALTH = 70;
const float CLOWN_MOVE_SPEED = 0;
const int CLOWN_SHOOT_DEMAGE = 6;
const int CLOWN_TOUCH_DEMAGE = 10;
const float CLOWN_TARGET_RANGE = 500;
const sf::Vector2f CLOWN_TARGET_AREA_SIZE = { CLOWN_TARGET_RANGE * 2.0f , CLOWN_SIZE.y / 2.0f };
const float CLOWN_SHOOT_RANGE = CLOWN_TARGET_RANGE;
const int CLOWN_BULLET_DEMAGE = 3;
const float CLOWN_SHOOT_COLDOWN = 0.24f;

const sf::Vector2f GHOST_SIZE = { 80, 70 };
const int GHOST_START_HEALTH = 80;
const float GHOST_TARGET_RANGE = 600;
const float GHOST_MIN_TARGET_RANGE = 1;
const float GHOST_MOVE_SPEED = 110;
const int GHOST_TOUCH_DEMAGE = 6;

const sf::Vector2f SPIDER_SIZE = { 50, 50 };
const int SPIDER_START_HEALTH = 50;
const sf::Vector2f SPIDER_TARGET_AREA_SIZE = { 60, 230 };
const float SPIDER_MOVE_SPEED = 100;
const float SIDER_JUMP_SPEED = 200;
const int SPIDER_TOUCH_DEMAGE = 9;

const sf::Vector2f BOSS_SIZE = { 160, 250 };
const int BOSS_START_HEALTH = 1000;
const int BOSS_SHOOT_DEMAGE = 5;
const int BOSS_TOUCH_DEMAGE = 100;
const float BOSS_JUMP_DURATION = 2;
const float BOSS_MAX_ATTACK_DURATION = 5;
const float BOSS_TARGET_RANGE = 650;
const float BOSS_SHOOT_COLDOWN = 0.35f;

const float WALL_ATTACK_BULLET_MARGIN = 40;
const int BULLETS_IN_WALL_ATTACK_COUNT = 5;
const int WALL_ATTACK_WINDOW_BULLETS_COUNT = 3;

const int LAVA_DEMAGE = 20;
const float BOTTOM_HAND_MARGIN = 10;

const int EASY_DEMAGE_INCREASE = -2;
const int NORMAL_DEMAGE_INCREASE = 0;
const int HARD_DEMAGE_INCREASE = 3;

enum struct EnemyType
{
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
	Enemy(sf::Vector2f const& posRect, EnemyType const& type, int demageIncrease);

	void CreateShadow(int demageIncrease);
	void CreateClown(int demageIncrease);
	void CreateGhost(int demageIncrease);
	void CreateSpider(int demageIncrease);
	void CreateBoss(int demageIncrease);

	sf::Vector2f position;
	sf::FloatRect targetArea;
	
	sf::Vector2f ghostMove;

	MovementStatus currentRunStatus;

	sf::FloatRect handLeftMiddle;
	sf::FloatRect handLeftBottom;

	sf::FloatRect handRightMiddle;
	sf::FloatRect handRightBottom;

	EnemyType enemyType;
	EnemyActivity activityStatus = EnemyActivity::IDLE;

	std::function<void(Character const& player)> UpdateActivityStatus;
	std::function<void(float elapsedTime, std::vector<TmxObject> const& blocks)> Idle;
	std::function<void(Character const& player, std::vector<Bullet*>& bullets, std::vector<TmxObject> const& blocks, float elapsedTime)> Pursuit;

	void UpdateAI(float elapsedTime, Character const& player, std::vector<TmxObject> const& blocks, std::vector<Bullet*>& bullets);
	void UpdateHands();
	void UpdateTexture(float elapsedTime)override;

	void UpdateGhostActivityStatus(Character const& player);
	void UpdateShadowActivityStatus(Character const& player);
	void UpdateClownActivityStatus(Character const& player);
	void UpdateSpiderActivityStatus(Character const& player);
	void UpdateBossActivityStatus(Character const& player);

	void ShadowWalk(float elapsedTime, std::vector<TmxObject> const& blocks);

	void ClownShoot(std::vector<Bullet*>& bullets);
	bool IsClownOnGround = false;

	void UpdateSpiderPos(float elapsedTime);
	void SpiderPursuite(Character const& player, std::vector<TmxObject> const& blocks);

	void UpdateGhostPos(float elapsedTime);
	void GhostIdle(float elapsedTime);
	void GhostPursuite(Character const& player);

	void BossPursuite(Character const& player, std::vector<Bullet*>& bullets);
	void BossIdle();
	void BossAttack(std::vector<Bullet*>& bullets, sf::Vector2f const& targetPos);
};