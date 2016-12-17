#pragma once
#include "stdafx.h"
#include "character.h"
#include "player.h"

const sf::Vector2f HAND_SIZE = { 2, 2 };
const float HAND_MARGIN_X = 10;

const std::string SHADOW_SPAWN_NAME = "enemy_shadow_spawn";
const sf::Vector2f SHADOW_SIZE = { 60 , 100 };
const float SHADOW_START_HEALTH = 100;
const float SHADOW_MOVE_SPEED = 100;
const unsigned POINTS_FOR_KILL_SHADOW = 30;

const std::string CLOWN_SPAWN_NAME = "enemy_clown_spawn";
const sf::Vector2f CLOWN_SIZE = { 60 , 100 };
const float CLOWN_START_HEALTH = 100;
const float CLOWN_MOVE_SPEED = 200;
const unsigned POINTS_FOR_KILL_CLOWN = 30;
const float CLOWN_MIN_TARGET_RANGE_X = 30;
const float CLOWN_MAX_TARGET_RANGE_X = 400;

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

	sf::RectangleShape handLeftTop;
	sf::RectangleShape handLeftMiddle;
	sf::RectangleShape handLeftBottom;

	sf::RectangleShape handRightTop;
	sf::RectangleShape handRightMiddle;
	sf::RectangleShape handRightBottom;

	EnemyType enemyType = EnemyType::NONE;
	EnemyActivity enemyActivity = EnemyActivity::IDLE;

	void UpdateAI(Player& player, Level& level);
	void UpdateClownAim(Player& player);
	void UpdateHands();
	void UpdateStatus(Level& level);

	void Draw(sf::RenderWindow& window) override;
};