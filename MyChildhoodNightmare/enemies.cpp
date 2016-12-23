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
	case EnemyType::BIRD:
		this->CreateBird();
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

	currentRunStatus = MovementStatus(rand() % 2);

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
	moveSpeed = SHADOW_MOVE_SPEED;

	Pursuit = [&](Player const& player) {
		(void)player;
	};

	Idle = [&](float elapsedTime, std::vector<Object> const& objects) {
		moveSpeed = SHADOW_MOVE_SPEED;
		runStatus = currentRunStatus;
		ShadowIdle(elapsedTime, objects);
	};
}

void Enemy::CreateClown()
{
	enemyType = EnemyType::CLOWN;

	bodyTexture.loadFromFile("resources/enemyClown.png");

	health = CLOWN_START_HEALTH;
	demage = CLOWN_TOUCH_DEMAGE;
	shootRange = CLOWN_TARGET_RANGE;

	Pursuit = [&](Player const& player) {
		ClownShoot(player);
	};

	Idle = [&](float elapsedTime, std::vector<Object> const& objects) {
		(void)objects;
		(void)elapsedTime;
	};
}

void Enemy::CreateBird()
{
	enemyType = EnemyType::BIRD;

	bodyTexture.loadFromFile("resources/enemyBird.png");

	health = BIRD_START_HEALTH;
	demage = BIRD_DEMAGE;

	Pursuit = [&](Player const& player) {
		(void)player;
	};

	Idle = [&](float elapsedTime, std::vector<Object> const& objects) {
		(void)objects;
		(void)elapsedTime;
	};
}

void Enemy::Update(float elapsedTime, Player const& player, std::vector<Object> const& objects)
{
	UpdateHealthStatus();
	UpdateActivityStatus(player);

	if (activityStatus == EnemyActivity::IDLE)
	{
		Idle(elapsedTime, objects);
	}
	else
	{
		Pursuit(player);
	}

	if (enemyType != EnemyType::BIRD)
	{
		UpdatePos(elapsedTime, objects);
	}
	UpdateHands();
}

void Enemy::UpdateActivityStatus(Player const& player)
{
	switch (this->enemyType)
	{
	case EnemyType::SHADOW:
		UpdateShadowActivityStatus(player);
		break;
	case EnemyType::CLOWN:
		UpdateClownActivityStatus(player);
		break;
	case EnemyType::BIRD:
		UpdateBirdActivityStatus(player);
		break;
	case EnemyType::BOSS:
		UpdateBossActivityStatus(player);
		break;
	default:
		break;
	}
}

void Enemy::UpdateShadowActivityStatus(Player const& player)
{
	this->activityStatus = EnemyActivity::IDLE;
	(void)player;
}

void Enemy::UpdateClownActivityStatus(Player const& player)
{
	activityStatus = EnemyActivity::IDLE;
	float halfBody = bodyShape.getSize().y / 2.0f;
	
	targetArea.setSize({ 2.0f * CLOWN_TARGET_RANGE, halfBody });
	targetArea.setPosition(GetCharacterPos().x - CLOWN_TARGET_RANGE, GetCharacterPos().y - halfBody - halfBody / 2.0f);
	targetArea.setFillColor(sf::Color(40, 10, 30, 100));

	if (player.collisionRect.intersects(targetArea.getGlobalBounds()))
	{
		cout << "PURSUIT!";
		activityStatus = EnemyActivity::PURSUIT;
		if (player.GetCharacterPos().x < GetCharacterPos().x)
		{
			orientationStatus = OrientationStatus::LEFT;
		}
		else
		{
			orientationStatus = OrientationStatus::RIGHT;
		}
	}
}

void Enemy::UpdateBirdActivityStatus(Player const& player)
{
	(void)player;
}

void Enemy::UpdateBossActivityStatus(Player const& player)
{
	(void)player;
}

void Enemy::ShadowIdle(float elapsedTime, std::vector<Object> const& objects)
{
	auto handLeftMiddle_copy = handLeftMiddle.getGlobalBounds();
	auto handLeftBottom_copy = handLeftBottom.getGlobalBounds();

	auto handRightMiddle_copy = handRightMiddle.getGlobalBounds();
	auto handRightBottom_copy = handRightBottom.getGlobalBounds();

	auto mevoment = elapsedTime * moveSpeed;

	if (runStatus == MovementStatus::RUN_LEFT)
	{
		handLeftMiddle_copy.left -= mevoment;
		handLeftBottom_copy.left -= mevoment;

		if (IsCollidesWithLevel(handLeftMiddle_copy, objects) ||
			!IsCollidesWithLevel(handLeftBottom_copy, objects))
		{
			currentRunStatus = MovementStatus::RUN_RIGHT;
		}
	}
	else if (runStatus == MovementStatus::RUN_RIGHT)
	{
		handRightMiddle_copy.left += mevoment;
		handRightBottom_copy.left += mevoment;

		if (IsCollidesWithLevel(handRightMiddle_copy, objects) ||
			!IsCollidesWithLevel(handRightBottom_copy, objects))
		{
			currentRunStatus = MovementStatus::RUN_LEFT;
		}
	}
}

void Enemy::ClownShoot(Player const& player)
{
	(void)player;
	int orientationId = static_cast<int>(orientationStatus);

	if (shootColdown >= CLOWN_SHOOT_COLDOWN)
	{
		bullets.push_back(new Bullet(GetCharacterPos(), CLOWN_BULLET_DEMAGE, orientationId, shootRange));
		shootColdown = 0;
	}
}

void Enemy::BirdPursuite(float elapsedTime, std::vector<Object> const& objects)
{
	(void)elapsedTime;
	(void)objects;
}

void Enemy::UpdateHands()
{
	float leftHandX = GetCharacterPos().x - bodyShape.getSize().x / 2.0f - HAND_MARGIN_X;
	float rightHandX = GetCharacterPos().x + bodyShape.getSize().x / 2.0f + HAND_MARGIN_X;
	float topHandY = GetCharacterPos().y - bodyShape.getSize().y;
	float bottomHandY = GetCharacterPos().y + 10;

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

	window.draw(targetArea);
	window.draw(bodyShape);
}