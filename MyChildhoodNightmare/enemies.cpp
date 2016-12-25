#include "enemies.h"

using namespace std;

Enemy::Enemy(sf::Vector2f const& position, EnemyType const& type)
	:position(position)
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
	collisionRect.top = position.y;
	collisionRect.left = position.x;
}

void Enemy::CreateShadow()
{
	enemyType = EnemyType::SHADOW;

	bodyTexture.loadFromFile("resources/enemyShadow.png");

	health = SHADOW_START_HEALTH;
	demage = SHADOW_DEMAGE;
	
	float randomSpeed = SHADOW_MOVE_SPEED_RANDOM * (rand() % 101) / 100;
	moveSpeed = SHADOW_MOVE_SPEED + randomSpeed;

	Pursuit = [&](Player const& player, std::vector<Bullet*>& bullets) {
		(void)bullets;
		(void)player;
	};

	Idle = [&](float elapsedTime, std::vector<Object> const& blocks) {
		moveSpeed = SHADOW_MOVE_SPEED;
		runStatus = currentRunStatus;
		ShadowIdle(elapsedTime, blocks);
	};
}

void Enemy::CreateClown()
{
	enemyType = EnemyType::CLOWN;

	bodyTexture.loadFromFile("resources/enemyClown.png");

	health = CLOWN_START_HEALTH;
	demage = CLOWN_TOUCH_DEMAGE;
	shootRange = CLOWN_TARGET_RANGE;

	Pursuit = [&](Player const& player, std::vector<Bullet*>& bullets) {
		ClownShoot(player, bullets);
	};

	Idle = [&](float elapsedTime, std::vector<Object> const& blocks) {
		(void)blocks;
		(void)elapsedTime;
	};
}

void Enemy::CreateBird()
{
	enemyType = EnemyType::BIRD;

	bodyTexture.loadFromFile("resources/enemyBird.png");

	health = BIRD_START_HEALTH;
	demage = BIRD_DEMAGE;

	Pursuit = [&](Player const& player, std::vector<Bullet*>& bullets) {
		(void)bullets;
		BirdPursuite(player);
	};

	Idle = [&](float elapsedTime, std::vector<Object> const& blocks) {
		(void)blocks;
		BirdIdle(elapsedTime);
	};
}

void Enemy::UpdateAI(float elapsedTime, Player const& player, std::vector<Object> const& blocks, std::vector<Bullet*>& bullets)
{
	UpdateHealthStatus();
	UpdateActivityStatus(player);

	if (activityStatus == EnemyActivity::IDLE)
	{
		Idle(elapsedTime, blocks);
	}
	else
	{
		Pursuit(player, bullets);
	}

	if (enemyType == EnemyType::BIRD)
	{
		UpdateBirdPos(elapsedTime, blocks);
	}
	else
	{
		UpdatePos(elapsedTime, blocks);
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

	if (player.collisionRect.intersects((sf::FloatRect)targetArea.getGlobalBounds()))
	{
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
	activityStatus = EnemyActivity::IDLE;

	sf::Vector2f playerPos = player.GetCharacterPos();

	float rangeX = playerPos.x - collisionRect.left;
	float rangeY = playerPos.y - collisionRect.top;

	float targetRadius = sqrt(rangeX * rangeX + rangeY * rangeY);

	if (targetRadius <= BIRD_TARGET_RANGE)
	{
		activityStatus = EnemyActivity::PURSUIT;
	}
}

void Enemy::UpdateBossActivityStatus(Player const& player)
{
	(void)player;
}

void Enemy::ShadowIdle(float elapsedTime, std::vector<Object> const& blocks)
{
	sf::FloatRect handLeftMiddle_copy = handLeftMiddle;
	sf::FloatRect handLeftBottom_copy = handLeftBottom;

	sf::FloatRect handRightMiddle_copy = handRightMiddle;
	sf::FloatRect handRightBottom_copy = handRightBottom;

	auto mevoment = elapsedTime * moveSpeed;

	if (runStatus == MovementStatus::RUN_LEFT)
	{
		handLeftMiddle_copy.left -= mevoment;
		handLeftBottom_copy.left -= mevoment;

		if (IsCollidesWithLevel(handLeftMiddle_copy, blocks) ||
			!IsCollidesWithLevel(handLeftBottom_copy, blocks))
		{
			currentRunStatus = MovementStatus::RUN_RIGHT;
		}
	}
	else if (runStatus == MovementStatus::RUN_RIGHT)
	{
		handRightMiddle_copy.left += mevoment;
		handRightBottom_copy.left += mevoment;

		if (IsCollidesWithLevel(handRightMiddle_copy, blocks) ||
			!IsCollidesWithLevel(handRightBottom_copy, blocks))
		{
			currentRunStatus = MovementStatus::RUN_LEFT;
		}
	}
}

void Enemy::ClownShoot(Player const& player, std::vector<Bullet*>& bullets)
{
	(void)player;
	int orientationId = static_cast<int>(orientationStatus);

	if (shootColdown >= CLOWN_SHOOT_COLDOWN)
	{
		bullets.push_back(new Bullet(GetCharacterPos(), CLOWN_BULLET_DEMAGE, orientationId, shootRange));
		shootColdown = 0;
	}
}

void Enemy::UpdateBirdPos(float elapsedTime, std::vector<Object> const& blocks)
{
	sf::FloatRect collisionRect_copy = collisionRect;

	collisionRect_copy.left += elapsedTime * BIRD_MOVE_SPEED * birdMove.x / 2.0f;
	if (!IsCollidesWithLevel(collisionRect_copy, blocks))
	{
		collisionRect.left = collisionRect_copy.left;
	}

	collisionRect_copy.top += elapsedTime * BIRD_MOVE_SPEED * birdMove.y;
	if (!IsCollidesWithLevel(collisionRect_copy, blocks))
	{
		collisionRect.top = collisionRect_copy.top;
	}

	birdMove = { 0, 0 };
	bodyShape.setPosition(GetCharacterPos());
}

void Enemy::BirdIdle(float elapsedTime)
{
	(void)elapsedTime;
}

void Enemy::BirdPursuite(Player const& player)
{
	sf::Vector2f playerPos = player.GetCharacterPos();

	float halfPlayerBody = player.collisionRect.height / 2.0f;
	float moveX = 1;
	float moveY = 1;

	if (playerPos.x < GetCharacterPos().x)
	{
		moveX = -moveX;
	}
	if (playerPos.y - halfPlayerBody < GetCharacterPos().y)
	{
		moveY = -moveY;
	}

	birdMove = { moveX , moveY };
}

void Enemy::UpdateHands()
{
	float leftHandX = GetCharacterPos().x - bodyShape.getSize().x / 2.0f - HAND_MARGIN_X;
	float rightHandX = GetCharacterPos().x + bodyShape.getSize().x / 2.0f + HAND_MARGIN_X;
	float topHandY = GetCharacterPos().y - bodyShape.getSize().y;
	float bottomHandY = GetCharacterPos().y + 10;

	handLeftTop = sf::FloatRect({ leftHandX, topHandY }, HAND_SIZE);
	handLeftMiddle = sf::FloatRect({ leftHandX, GetCharacterPos().y - bodyShape.getSize().y / 4.0f }, HAND_SIZE);
	handLeftBottom = sf::FloatRect({ leftHandX, bottomHandY }, HAND_SIZE );

	handRightTop = sf::FloatRect({ rightHandX, topHandY }, HAND_SIZE);
	handRightMiddle = sf::FloatRect({ rightHandX, GetCharacterPos().y - bodyShape.getSize().y / 4.0f }, HAND_SIZE);
	handRightBottom = sf::FloatRect({ rightHandX, bottomHandY }, HAND_SIZE);
}