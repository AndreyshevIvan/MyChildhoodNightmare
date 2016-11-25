#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "bullets.h"
#include "tinyxml/level.h"

const float MAX_WEAPON_COLDOWN = 1;
const float PISTOL_COLDOWN = 0.3;
const float AK_COLDOWN = 0.08;
const float CRY_COLDOWN = 0.5;
const float PLAYER_START_HEALTH = 300;
const sf::Vector2f PLAYER_BAR_DISPLACEMENT = { -500, -300 };

enum ExistenceStatus
{
	NOT_SPAWNED,
	LIVE,
	DEAD
};

enum MoveingStatus
{
	RUN_LEFT,
	RUN_RIGHT,
	NOT_RUN
};

enum JumpingStatus
{
	FLY,
	ON_GROUND
};

enum SeatStatus
{
	SEAT,
	NOT_SEAT
};

enum Orientation
{
	LEFT = 1,
	RIGHT
};

enum Weapon
{
    CRY,
    PISTOL,
    AK
};

struct Character
{
    sf::Texture bodyTexture;
    sf::RectangleShape bodyShape;
    sf::RectangleShape collisionShape;

    MoveingStatus runStatus;
    JumpingStatus jumpStatus;
    SeatStatus seatStatus;
    ExistenceStatus existStatus;
    Orientation orientation;

    float moveSpeed;
    float jumpSpeed;
    float jumpHeight;
    sf::RectangleShape healthBar;

    Weapon weapon;
	float shootColdown;
	std::list<Bullet*> bullets;
	std::list<Bullet*>::iterator bulletsIter;

	bool InitCharacter(std::string const& textureFileName, sf::Vector2f const& spawnPos, sf::Vector2f const& size, float speed, float jumpH);

	sf::Vector2f GetCharacterPos();

	void Jump();
	void Seat();
	void Shoot(Level const& level);
};