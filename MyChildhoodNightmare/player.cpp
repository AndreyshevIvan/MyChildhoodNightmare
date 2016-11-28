#include "player.h"

bool Player::InitPlayer(Object const& spawnObj)
{
	if (!bodyTexture.loadFromFile("resources/p.png"))
	{
		return false;
	}

	sf::Vector2f spawnPos = { spawnObj.rect.left, spawnObj.rect.top };

	bodyShape.setTextureRect(sf::IntRect(30, 35, 80, 140));
	bodyShape.setSize(PLAYER_SIZE);
	bodyShape.setTexture(&bodyTexture);
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
	orientation = RIGHT;

	return true;
}

void Player::Attack(Level const& level)
{
	switch (weapon)
	{
	case CRY:
		if (shootColdown > CRY_COLDOWN)
		{
			bullets.push_back(new Bullet(GetCharacterPos(), level, orientation));
			shootColdown = 0;
		}
		break;
	case AK:
		if (shootColdown > AK_COLDOWN)
		{
			bullets.push_back(new Bullet(GetCharacterPos(), level, orientation));
			shootColdown = 0;
		}
		break;
	case PISTOL:
		if (shootColdown > PISTOL_COLDOWN)
		{
			bullets.push_back(new Bullet({ GetCharacterPos().x, GetCharacterPos().y + 20 }, level, orientation));
			bullets.push_back(new Bullet({ GetCharacterPos().x, GetCharacterPos().y }, level, orientation));
			bullets.push_back(new Bullet({ GetCharacterPos().x, GetCharacterPos().y - 20 }, level, orientation));
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