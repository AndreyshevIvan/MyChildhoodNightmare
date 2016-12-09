#include "character.h"
#include <iostream>

using namespace std;
using namespace sf;

Character::Character(sf::FloatRect const& posRect, CharacterType const& type)
{
	switch (type)
	{
	case SHADOW:
		this->CreateShadow();
		break;
	case CLOWN:
		this->CreateClown();
		break;
	default:
		break;
	}

	collisionRect.top = posRect.left;
	collisionRect.left = posRect.top;

	shootColdown = 0;
	currentFrame = 0;
	orientationStatus = RIGHT;
	runStatus = NOT_RUN;
	jumpSpeed = 0;
}

void Character::CreateShadow()
{
	charecterType = SHADOW;
	cout << "create one shadow" "\n";

	bodyTexture.loadFromFile("resources/enemyShadow.png");
	bodyShape.setTexture(&bodyTexture);

	collisionRect.width = ENEMY_SHADOW_SIZE.x / 2.0f;
	collisionRect.height = ENEMY_SHADOW_SIZE.y - 10;

	bodyShape.setSize(ENEMY_SHADOW_SIZE);
	bodyShape.setTexture(&bodyTexture);
	bodyShape.setOrigin(ENEMY_SHADOW_SIZE.x / 2.0f, ENEMY_SHADOW_SIZE.y);

	moveSpeed = ENEMY_SHADOW_MOVE_SPEED;
	health = ENEMY_SHADOW_START_HEALTH;
	demage = 12;
}

void Character::CreateClown()
{
	cout << "create one clown" "\n";
	charecterType = CLOWN;
	bodyTexture.loadFromFile("resources/enemyClown.png");
	bodyShape.setTexture(&bodyTexture);

	collisionRect.width = ENEMY_CLOWN_SIZE.x / 2.0f;
	collisionRect.height = ENEMY_CLOWN_SIZE.y - 10;

	bodyShape.setSize(ENEMY_CLOWN_SIZE);
	bodyShape.setOrigin(ENEMY_CLOWN_SIZE.x / 2.0f, ENEMY_CLOWN_SIZE.y);

	moveSpeed = ENEMY_CLOWN_MOVE_SPEED;
	jumpSpeed = 0;
	health = ENEMY_CLOWN_START_HEALTH;
	demage = 12;
}

void Character::Spawn(Object const& spawnObj)
{
	collisionRect.left = spawnObj.rect.left;
	collisionRect.top = spawnObj.rect.top;
}

void Character::Jump()
{
	if (jumpStatus == ON_GROUND)
	{
		jumpSpeed = -CHARACTERS_JUMP_SPEED;
	}
}

sf::Vector2f Character::GetCharacterPos()
{
	sf::Vector2f position(
		collisionRect.left + collisionRect.width / 4.0f,
		collisionRect.top + collisionRect.height
	);

	return position;
}

void Character::UpdatePos(float elapsedTime, std::vector<Object> const& mapTiles)
{
	if (runStatus != NOT_RUN)
	{
		float movement = moveSpeed * elapsedTime;

		if (jumpStatus == FLY)
		{
			movement *= FLYING_SLOWDOWN;
		}
		if (runStatus == RUN_LEFT)
		{
			movement = -movement;
		}

		collisionRect.left += movement;

		if (IsCollidesWithLevel(collisionRect, mapTiles))
		{
			collisionRect.left -= movement;
		}
	}

	UpdateGravity(elapsedTime, mapTiles);

	runStatus = NOT_RUN;
	bodyShape.setPosition(GetCharacterPos());
}

void Character::UpdateGravity(float elapsedTime, std::vector<Object> const& mapTiles)
{
	float movementY = jumpSpeed;

	jumpSpeed = jumpSpeed + G * elapsedTime;
	movementY = jumpSpeed * elapsedTime;

	collisionRect.top += movementY;

	if (IsCollidesWithLevel(collisionRect, mapTiles))
	{
		collisionRect.top -= movementY;
		if (movementY > 0)
		{
			jumpStatus = ON_GROUND;
		}
		jumpSpeed = 0;
	}
	else
	{
		jumpStatus = FLY;
	}
}

void Character::UpdateHealthStatus()
{
	if (health < 0)
	{
		existStatus = DEAD;
	}
}

bool Character::IsCollidesWithLevel(sf::FloatRect const& rect, std::vector<Object> const& mapTiles)
{
	for (unsigned i = 0; i < mapTiles.size(); i++)
	{
		if (rect.intersects(mapTiles[i].rect) && mapTiles[i].name == "solid")
		{
			return true;
		}
	}
	return false;
}

void Character::Draw(sf::RenderWindow& window)
{
	window.draw(bodyShape);
}

void Character::Clear()
{
	for (auto it = bullets.begin(); it != bullets.end();)
	{
		Bullet* bullet = *it;
		it = bullets.erase(it);
		delete(bullet);
	}
}