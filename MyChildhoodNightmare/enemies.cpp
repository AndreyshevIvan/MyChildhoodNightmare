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

	int directionId = rand() % 2;
	idleMovement = MovementStatus(directionId);

	bodyShape.setTexture(&bodyTexture);
	const sf::Vector2f BODY_SIZE = {
		static_cast<float>(bodyTexture.getSize().x),
		static_cast<float>(bodyTexture.getSize().y)
	};

	bodyShape.setSize(BODY_SIZE);
	bodyShape.setOrigin(BODY_SIZE.x / 2.0f, BODY_SIZE.y);

	collisionRect.width = bodyShape.getSize().x / 2.0f;
	collisionRect.height = bodyShape.getSize().y - 10;
	collisionRect.top = posRect.top;
	collisionRect.left = posRect.left;
}

void Enemy::CreateShadow()
{
	enemyType = EnemyType::SHADOW;

	bodyTexture.loadFromFile("resources/enemyShadow.png");

	health = SHADOW_START_HEALTH;
	demage = SHADOW_DEMAGE;

	Pursuit = [&](Player const& player) {
		moveSpeed = SHADOW_MOVE_SPEED;
		ShadowPursuit(player);
	};

	Idle = [&]() {
		runStatus = idleMovement;
		moveSpeed = SHADOW_IDLE_MOVE_SPEED;
		if (idleWalkingColdown >= MAX_IDLE_WALKING_COLDOWN)
		{
			if (idleMovement == MovementStatus::RUN_LEFT)
			{
				idleMovement = MovementStatus::RUN_RIGHT;
			}
			else
			{
				idleMovement = MovementStatus::RUN_LEFT;
			}
			idleWalkingColdown = 0;
		}
	};
}

void Enemy::CreateClown()
{
	enemyType = EnemyType::CLOWN;

	bodyTexture.loadFromFile("resources/enemyClown.png");

	moveSpeed = CLOWN_MOVE_SPEED;
	health = CLOWN_START_HEALTH;
	demage = CLOWN_TOUCH_DEMAGE;
}

void Enemy::Update(float elapsedTime, Player const& player, std::vector<Object> const& objects)
{
	UpdateHealthStatus();
	UpdateActivityStatus(player);

	if (this->activityStatus == EnemyActivity::PURSUIT)
	{
		this->Pursuit(player);
	}
	else
	{
		this->Idle();
	}

	LookGround(objects);
	UpdatePos(elapsedTime, objects);
	UpdateHands();
}

void Enemy::UpdateAI()
{
}

void Enemy::UpdateActivityStatus(Player const& player)
{
	float rangeX = this->GetCharacterPos().x - player.GetCharacterPos().x;
	float rangeY = this->GetCharacterPos().y - player.GetCharacterPos().y;

	if (sqrt(rangeX * rangeX + rangeY * rangeY) <= MAX_TARGET_RANGE)
	{
		this->activityStatus = EnemyActivity::PURSUIT;
	}
}

void Enemy::ShadowPursuit(Player const& player)
{
	float enemyPosX = this->GetCharacterPos().x;
	float playerPosX = player.GetCharacterPos().x;

	if (abs(enemyPosX - playerPosX) >= MIN_TARGET_RANGE)
	{
		if (enemyPosX <= playerPosX)
		{
			this->runStatus = MovementStatus::RUN_RIGHT;
		}
		else
		{
			this->runStatus = MovementStatus::RUN_LEFT;
		}
	}
}

void Enemy::LookGround(std::vector<Object> const& objects)
{
	if (runStatus == MovementStatus::RUN_LEFT)
	{
		if (!IsCollidesWithLevel(handLeftBottom.getGlobalBounds(), objects) ||
			IsCollidesWithLevel(handLeftMiddle.getGlobalBounds(), objects))
		{
			Jump();
		}
	}
	else if (runStatus == MovementStatus::RUN_RIGHT)
	{
		if (!IsCollidesWithLevel(handRightBottom.getGlobalBounds(), objects) ||
			IsCollidesWithLevel(handRightMiddle.getGlobalBounds(), objects))
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