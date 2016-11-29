#include "player.h"

bool Player::InitPlayer(Object const& spawnObj)
{
	if (!bodyTexture.loadFromFile("resources/p.png"))
	{
		return false;
	}

	sf::Vector2f spawnPos = { spawnObj.rect.left, spawnObj.rect.top };

	bodyShape.setTextureRect(sf::IntRect(30, 35, 80, 140));
	bodyShape.setTexture(&bodyTexture);
	bodyShape.setSize(PLAYER_SIZE);
	bodyShape.setOrigin(PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y);

	collisionShape.setSize({ PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y - 10 });
	//collisionShape.setFillColor(sf::Color::Red);
	collisionShape.setOrigin(PLAYER_SIZE.x / 4.0f, PLAYER_SIZE.y - 10);
	collisionShape.setPosition(spawnPos);

	moveSpeed = PLAYER_MOVE_SPEED;
	jumpSpeed = 0;
	jumpHeight = PLAYER_JUMP_HEIGHT;

	shootColdown = 0;
	currentFrame = 0;
	orientationStatus = RIGHT;

	return true;
}

void Player::Attack(Level const& level)
{
	switch (weapon)
	{
	case FIREBALL:
		if (shootColdown > CRY_COLDOWN)
		{
			bullets.push_back(new Bullet(GetCharacterPos(), level, orientationStatus));
			shootColdown = 0;
		}
		break;
	default:
		break;
	}
}

void Player::UpdateBullets(float elapsedTime)
{
	for (bulletsIter = bullets.begin(); bulletsIter != bullets.end();)
	{
		Bullet* bullet = *bulletsIter;
		bullet->Update(elapsedTime);
		if (bullet->IsLife == false)
		{
			bulletsIter = bullets.erase(bulletsIter);
			delete(bullet);
		}
		else
		{
			bulletsIter++;
		}
	}
}

void Player::DrawBullets(sf::RenderWindow& window)
{
	for (bulletsIter = bullets.begin(); bulletsIter != bullets.end();)
	{
		Bullet* bullet = *bulletsIter;
		window.draw(bullet->bodyShape);
		bulletsIter++;
	}
}