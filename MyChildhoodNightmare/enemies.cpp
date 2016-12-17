#include "enemies.h"

using namespace std;

Enemy::Enemy(sf::FloatRect const& posRect, EnemyType const& type)
	:position(posRect)
	,enemyType(type)
{
	switch (type)
	{
	case EnemyType::SHADOW:
		this->CreateShadow();
		break;
	case EnemyType::CLOWN:
		this->CreateClown();
		break;
	default:
		break;
	}

	handLeftTop.setSize(HAND_SIZE);
	handLeftMiddle.setSize(HAND_SIZE);
	handLeftBottom.setSize(HAND_SIZE);
	handRightTop.setSize(HAND_SIZE);
	handRightMiddle.setSize(HAND_SIZE);
	handRightBottom.setSize(HAND_SIZE);

	collisionRect.top = posRect.top;
	collisionRect.left = posRect.left;

	shootColdown = 0;
	currentFrame = 0;
	orientationStatus = RIGHT;
	runStatus = NOT_RUN;
	jumpSpeed = 0;
}

void Enemy::CreateShadow()
{
	enemyType = EnemyType::SHADOW;
	enemyActivity = EnemyActivity::IDLE;

	bodyTexture.loadFromFile("resources/enemyShadow.png");
	bodyShape.setTexture(&bodyTexture);

	collisionRect.width = SHADOW_SIZE.x / 2.0f;
	collisionRect.height = SHADOW_SIZE.y - 10;

	bodyShape.setSize(SHADOW_SIZE);
	bodyShape.setTexture(&bodyTexture);
	bodyShape.setOrigin(SHADOW_SIZE.x / 2.0f, SHADOW_SIZE.y);

	moveSpeed = SHADOW_MOVE_SPEED;
	health = SHADOW_START_HEALTH;
	demage = 12;
}

void Enemy::CreateClown()
{
	enemyType = EnemyType::CLOWN;
	enemyActivity = EnemyActivity::IDLE;

	bodyTexture.loadFromFile("resources/enemyClown.png");
	bodyShape.setTexture(&bodyTexture);

	collisionRect.width = CLOWN_SIZE.x / 2.0f;
	collisionRect.height = CLOWN_SIZE.y - 10;

	bodyShape.setSize(CLOWN_SIZE);
	bodyShape.setOrigin(CLOWN_SIZE.x / 2.0f, CLOWN_SIZE.y);

	moveSpeed = CLOWN_MOVE_SPEED;
	jumpSpeed = 0;
	health = CLOWN_START_HEALTH;
	demage = 12;

	Pursuit = [&](Player& player) {
		player;
	};
}

void Enemy::UpdateAI(Player& player, Level& level)
{
	UpdatePositionStatus(level);
	player;
	UpdateHands();
}

void Enemy::UpdateClownAim(Player& player)
{
	if (abs(this->GetCharacterPos().x - player.GetCharacterPos().x) < CLOWN_MAX_TARGET_RANGE_X && 
		abs(this->GetCharacterPos().x - player.GetCharacterPos().x) > CLOWN_MIN_TARGET_RANGE_X)
	{
		
	}
}

void Enemy::UpdatePositionStatus(Level& level)
{
	auto solids = level.GetObjects("solid");

	if (runStatus == MovementStatus::RUN_LEFT)
	{
		if (!IsCollidesWithLevel(handLeftBottom.getGlobalBounds(), solids) ||
			IsCollidesWithLevel(handLeftMiddle.getGlobalBounds(), solids))
		{
			Jump();
		}
	}
	else if (runStatus == MovementStatus::RUN_RIGHT)
	{
		if (!IsCollidesWithLevel(handRightBottom.getGlobalBounds(), solids) ||
			IsCollidesWithLevel(handRightMiddle.getGlobalBounds(), solids))
		{
			Jump();
		}
	}
}

void Enemy::UpdateHands()
{
	float leftHandX = GetCharacterPos().x - bodyShape.getSize().x / 2.0f - HAND_MARGIN_X;
	float rightHandX = GetCharacterPos().x + bodyShape.getSize().x / 2.0f + HAND_MARGIN_X;
	float topHandY = GetCharacterPos().y - bodyShape.getSize().y;
	float bottomHandY = GetCharacterPos().y + CHARACTERS_JUMP_HEIGHT;

	handLeftTop.setPosition(leftHandX, topHandY);
	handLeftMiddle.setPosition(leftHandX, GetCharacterPos().y - bodyShape.getSize().y / 4.0f);
	handLeftBottom.setPosition(leftHandX, bottomHandY);

	handRightTop.setPosition(rightHandX, topHandY);
	handRightMiddle.setPosition(rightHandX, GetCharacterPos().y - bodyShape.getSize().y / 4.0f);
	handRightBottom.setPosition(rightHandX, bottomHandY);
}

void Enemy::Draw(sf::RenderWindow& window)
{
	window.draw(handLeftTop);
	window.draw(handLeftBottom);
	window.draw(handLeftMiddle);
	window.draw(handRightTop);
	window.draw(handRightMiddle);
	window.draw(handRightBottom);
	window.draw(bodyShape);
}