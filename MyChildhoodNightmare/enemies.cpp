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
	case EnemyType::GHOST:
		this->CreateGhost();
		break;
	case EnemyType::SPIDER:
		this->CreateSpider();
		break;
	case EnemyType::BOSS:
		this->CreateBoss();
		break;
	default:
		break;
	}

	currentRunStatus = MovementStatus(rand() % 2);

	bodyShape.setTexture(&bodyTexture);

	sf::Vector2f BODY_SIZE = bodyShape.getSize();

	if (enemyType != EnemyType::SPIDER)
	{
		bodyShape.setTextureRect(sf::IntRect(0, 0, static_cast<int>(BODY_SIZE.x), static_cast<int>(BODY_SIZE.y)));
	}

	bodyShape.setSize(BODY_SIZE);
	bodyShape.setOrigin(BODY_SIZE.x / 2.0f, BODY_SIZE.y);

	collisionRect.width = BODY_SIZE.x / 2.0f;
	collisionRect.height = BODY_SIZE.y - 10;
	collisionRect.top = position.y;
	collisionRect.left = position.x;
}

void Enemy::CreateShadow()
{
	enemyType = EnemyType::SHADOW;

	bodyTexture.loadFromFile("resources/enemyShadow.png");
	bodyShape.setSize(SHADOW_SIZE);

	health = SHADOW_START_HEALTH;
	demage = SHADOW_DEMAGE;
	
	float randomSpeed = SHADOW_MOVE_SPEED_RANDOM * (rand() % 101) / 100;
	moveSpeed = SHADOW_MOVE_SPEED + randomSpeed;

	Pursuit = [&](Character const& player, std::vector<Bullet*>& bullets) {
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
	bodyShape.setSize(CLOWN_SIZE);

	health = CLOWN_START_HEALTH;
	demage = CLOWN_TOUCH_DEMAGE;
	shootRange = CLOWN_SHOOT_RANGE;

	Pursuit = [&](Character const& player, std::vector<Bullet*>& bullets) {
		ClownShoot(player, bullets);
	};

	Idle = [&](float elapsedTime, std::vector<Object> const& blocks) {
		(void)blocks;
		(void)elapsedTime;
	};
}

void Enemy::CreateGhost()
{
	enemyType = EnemyType::GHOST;

	bodyTexture.loadFromFile("resources/enemyGhost.png");
	bodyShape.setSize(GHOST_SIZE);

	health = GHOST_START_HEALTH;
	demage = GHOST_DEMAGE;

	Pursuit = [&](Character const& player, std::vector<Bullet*>& bullets) {
		(void)bullets;
		GhostPursuite(player);
	};

	Idle = [&](float elapsedTime, std::vector<Object> const& blocks) {
		(void)blocks;
		GhostIdle(elapsedTime);
	};
}

void Enemy::CreateSpider()
{
	enemyType = EnemyType::SPIDER;

	bodyTexture.loadFromFile("resources/enemySpider.png");
	bodyShape.setSize(SPIDER_SIZE);

	health = SPIDER_START_HEALTH;
	demage = SPIDER_DEMAGE;

	Pursuit = [&](Character const& player, std::vector<Bullet*>& bullets) {
		(void)bullets;
		SpiderPursuite(player);
	};

	Idle = [&](float elapsedTime, std::vector<Object> const& blocks) {
		(void)blocks;
		(void)elapsedTime;
	};

	const sf::Vector2i BODY_SIZE = {
		static_cast<int>(bodyShape.getSize().x),
		static_cast<int>(bodyShape.getSize().y)
	};
	bodyShape.setTextureRect(sf::IntRect(0, BODY_SIZE.y, BODY_SIZE.x, BODY_SIZE.y));
}

void Enemy::CreateBoss()
{
	enemyType = EnemyType::BOSS;

	bodyTexture.loadFromFile("resources/enemyBoss.png");

	health = BOSS_START_HEALTH;
	demage = BOSS_DEMAGE;

	Pursuit = [&](Character const& player, std::vector<Bullet*>& bullets) {
		(void)bullets;
		(void)player;
	};

	Idle = [&](float elapsedTime, std::vector<Object> const& blocks) {
		(void)blocks;
		(void)elapsedTime;
	};
}

void Enemy::UpdateAI(float elapsedTime, Character const& player, std::vector<Object> const& blocks, std::vector<Bullet*>& bullets)
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

	UpdateOrientation();

	if (enemyType == EnemyType::GHOST)
	{
		UpdateGhostPos(elapsedTime);
	}
	else if (enemyType == EnemyType::SPIDER && activityStatus == EnemyActivity::IDLE)
	{
		UpdateSpiderPos(elapsedTime);
	}
	else
	{
		UpdatePos(elapsedTime, blocks);
	}

	UpdateHands();
	if (!(enemyType == EnemyType::SPIDER && activityStatus == EnemyActivity::IDLE))
	{
		UpdateTexture();
	}
}

void Enemy::UpdateActivityStatus(Character const& player)
{
	switch (this->enemyType)
	{
	case EnemyType::SHADOW:
		UpdateShadowActivityStatus(player);
		break;
	case EnemyType::CLOWN:
		UpdateClownActivityStatus(player);
		break;
	case EnemyType::GHOST:
		UpdateGhostActivityStatus(player);
		break;
	case EnemyType::SPIDER:
		UpdateSpiderActivityStatus(player);
		break;
	case EnemyType::BOSS:
		UpdateBossActivityStatus(player);
		break;
	default:
		break;
	}
}

void Enemy::UpdateShadowActivityStatus(Character const& player)
{
	this->activityStatus = EnemyActivity::IDLE;
	(void)player;
}

void Enemy::UpdateClownActivityStatus(Character const& player)
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

void Enemy::UpdateGhostActivityStatus(Character const& player)
{
	activityStatus = EnemyActivity::IDLE;

	sf::Vector2f playerPos = player.GetCharacterPos();

	float rangeX = playerPos.x - collisionRect.left;
	float rangeY = playerPos.y - collisionRect.top;

	float targetRadius = sqrt(rangeX * rangeX + rangeY * rangeY);

	if (targetRadius <= GHOST_TARGET_RANGE)
	{
		activityStatus = EnemyActivity::PURSUIT;
	}
}

void Enemy::UpdateBossActivityStatus(Character const& player)
{
	(void)player;
}

void Enemy::UpdateSpiderActivityStatus(Character const& player)
{
	auto rangeX = abs(player.GetCharacterPos().x - GetCharacterPos().x);
	auto rangeY = abs(player.GetCharacterPos().y - GetCharacterPos().y);
	auto range = sqrt(rangeX * rangeX + rangeY * rangeY);

	cout << "playerRangeX : " << rangeX << "\n";
	cout << "playerRangeY : " << rangeY << "\n";
	cout << "range : " << range << "\n";

	if (range <= SPIDER_TARGET_RANGE)
	{
		activityStatus = EnemyActivity::PURSUIT;
	}
}

void Enemy::ShadowIdle(float elapsedTime, std::vector<Object> const& blocks)
{
	if (jumpStatus != JumpingStatus::FLY)
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
}

void Enemy::UpdateSpiderPos(float elapsedTime)
{
	(void)elapsedTime;
	bodyShape.setPosition(GetCharacterPos());
}

void Enemy::SpiderPursuite(Character const& player)
{
	(void)player;
}

void Enemy::ClownShoot(Character const& player, std::vector<Bullet*>& bullets)
{
	(void)player;
	int orientationId = static_cast<int>(orientationStatus);

	if (shootColdown >= CLOWN_SHOOT_COLDOWN)
	{
		bullets.push_back(new Bullet(GetCharacterPos(), CLOWN_BULLET_DEMAGE, orientationId, shootRange, BulletType::CLOWN_BULLET));
		shootColdown = 0;
	}
}

void Enemy::UpdateGhostPos(float elapsedTime)
{
	if (ghostMove.x < 0)
	{
		runStatus = MovementStatus::RUN_LEFT;
	}
	else if (ghostMove.x > 0)
	{
		runStatus = MovementStatus::RUN_RIGHT;
	}

	collisionRect.left += elapsedTime * GHOST_MOVE_SPEED * ghostMove.x;;
	collisionRect.top += elapsedTime * GHOST_MOVE_SPEED * ghostMove.y;

	ghostMove = { 0, 0 };
	bodyShape.setPosition(GetCharacterPos());
}

void Enemy::GhostIdle(float elapsedTime)
{
	(void)elapsedTime;
}

void Enemy::GhostPursuite(Character const& player)
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

	ghostMove = { moveX , moveY };
}

void Enemy::UpdateHands()
{
	float leftHandX = GetCharacterPos().x - bodyShape.getSize().x / 2.0f - HAND_MARGIN_X;
	float rightHandX = GetCharacterPos().x + bodyShape.getSize().x / 2.0f + HAND_MARGIN_X;
	float bottomHandY = GetCharacterPos().y + 10;

	handLeftMiddle = sf::FloatRect({ leftHandX, GetCharacterPos().y - bodyShape.getSize().y / 4.0f }, HAND_SIZE);
	handLeftBottom = sf::FloatRect({ leftHandX, bottomHandY }, HAND_SIZE );

	handRightMiddle = sf::FloatRect({ rightHandX, GetCharacterPos().y - bodyShape.getSize().y / 4.0f }, HAND_SIZE);
	handRightBottom = sf::FloatRect({ rightHandX, bottomHandY }, HAND_SIZE);
}