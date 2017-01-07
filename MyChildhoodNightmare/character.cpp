#include "character.h"

using namespace std;
using namespace sf;

void Character::Spawn(sf::Vector2f const& pos)
{
	collisionRect.left = pos.x;
	collisionRect.top = pos.y;
}

void Character::Jump()
{
	if (jumpStatus == ON_GROUND)
	{
		jumpSpeed = -CHARACTERS_JUMP_SPEED;
	}
}

sf::Vector2f Character::GetCharacterPos() const
{
	sf::Vector2f position(
		collisionRect.left + collisionRect.width / 4.0f,
		collisionRect.top + collisionRect.height
	);

	return position;
}

void Character::UpdatePos(float elapsedTime, std::vector<TmxObject> const& blocks)
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

		if (IsCollidesWithLevel(collisionRect, blocks))
		{
			collisionRect.left -= movement;
		}
	}

	UpdateGravity(elapsedTime, blocks);

	runStatus = NOT_RUN;
	bodyShape.setPosition(GetCharacterPos());
}

void Character::UpdateGravity(float elapsedTime, std::vector<TmxObject> const& blocks)
{
	float movementY = jumpSpeed;

	jumpSpeed = jumpSpeed + G * elapsedTime;
	movementY = jumpSpeed * elapsedTime;

	collisionRect.top += movementY;

	if (IsCollidesWithLevel(collisionRect, blocks))
	{
		collisionRect.top -= movementY;

		if (jumpSpeed > CRITICAL_JUMP_SPEED)
		{
			health -= static_cast<int>(jumpSpeed / SPEED_PER_DEMAGE);
			cout << "DEAMGE: " << static_cast<int>(jumpSpeed / SPEED_PER_DEMAGE) << std::endl;
		}

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
	if (health <= 0)
	{
		existStatus = ExistenceStatus::DEAD;

		if (deathSound != nullptr && deathSound->getStatus() != sf::Music::Playing)
		{
			deathSound->play();
		}
	}
}

bool Character::IsCollidesWithLevel(FloatRect const& rect, vector<TmxObject> const& blocks)
{
	return std::any_of(blocks.begin(), blocks.end(), [&](const TmxObject &block) {
		return (rect.intersects(block.rect) && block.name == "solid");
	});
}

void Character::DrawCharacter(RenderWindow& window)
{
	window.draw(bodyShape);
	for (auto blood : wounds)
	{
		blood->Draw(window);
	}
}

void Character::UpdateOrientation()
{
	if (runStatus == MovementStatus::RUN_LEFT)
	{
		orientationStatus = OrientationStatus::LEFT;
	}
	else if (runStatus == MovementStatus::RUN_RIGHT)
	{
		orientationStatus = OrientationStatus::RIGHT;
	}
}

void Character::UpdateTexture()
{
	auto bodyWidth = static_cast<int>(bodyShape.getSize().x);
	auto bodyHeight = static_cast<int>(bodyShape.getSize().y);

	if (orientationStatus == OrientationStatus::LEFT)
	{
		bodyShape.setTextureRect(sf::IntRect(0, 0, bodyWidth, bodyHeight));
	}
	else
	{
		bodyShape.setTextureRect(sf::IntRect(bodyWidth, 0, bodyWidth, bodyHeight));
	}
}

Blood::Blood(sf::Vector2f const& characterPos, sf::Vector2f const& bulletPos)
{
	bloodTexture.loadFromFile("resources/blood.png");

	positionOffset = characterPos - bulletPos;

	blood.setSize(BLOOD_SIZE);
	blood.setOrigin(0, BLOOD_SIZE.y / 2.0f);
	blood.setTextureRect(sf::IntRect({ 0, 0 }, static_cast<sf::Vector2i>(BLOOD_SIZE)));
	blood.setTexture(&bloodTexture);

	float rotation = static_cast<float>(-BLOOD_MAX_ROTATION + (rand() % (2 * BLOOD_MAX_ROTATION)));

	if (characterPos.x > bulletPos.x)
	{ // character to the left
		rotation += 180;
		positionOffset.x -= BLOOD_MARGIN_X;
	}
	else
	{ // character to the right
		positionOffset.x += BLOOD_MARGIN_X;
	}
	positionOffset.y -= BLOOD_MARGIN_Y;

	blood.setRotation(rotation);

	duration = 0;
	currentFrame = 0;
	secondsPerFrame = BLOOD_DURATION / BLOOD_FRAMES;
}

void Blood::Update(sf::Vector2f const& characterPos, float elapsedTime)
{
	duration += elapsedTime;
	currentFrame = static_cast<int>(duration / secondsPerFrame);

	const int FRAME_POS_X = static_cast<int>(BLOOD_SIZE.x * currentFrame);
	blood.setTextureRect(sf::IntRect({ FRAME_POS_X, 0 }, static_cast<sf::Vector2i>(BLOOD_SIZE)));
	blood.setPosition(characterPos - positionOffset);
}

void Blood::Draw(sf::RenderWindow& window)
{
	window.draw(blood);
}