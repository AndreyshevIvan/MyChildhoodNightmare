#include "game.h"

using namespace std;
using namespace sf;

bool Game::InitGame()
{
	if (level.LoadFromFile("resources/firstTileset.tmx"))
	{
		string str;
		mapTiles = level.GetAllObjects();
		for (unsigned int i = 0; i < mapTiles.size(); i++)
		{
			mapTiles[i].GetPropertyFloat(str);
		}

		if (character.InitCharacter(PLAYER_FILE_NAME, PLAYER_SPAWN_POS, PLAYER_SIZE, PLAYER_MOVE_SPEED, PLAYER_JUMP_HEIGHT))
		{
			camera.reset(sf::FloatRect(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT));
			gameStatus = PLAY;

			return true;
		}
	}
	 
	return false;
}

void Game::UpdateCamera(sf::Vector2f const& playerPos)
{
	camera.setCenter(sf::Vector2f(playerPos.x, playerPos.y - PLAYER_SIZE.y / 2.0f));
}

float Game::GetElapsedTime()
{
	elapsedTime = clock.getElapsedTime().asSeconds();
	clock.restart();
	return elapsedTime;
}

void Game::DrawLevel(sf::RenderWindow& window)
{
	level.Draw(window);
}

void Game::DrawCharacter(sf::RenderWindow& window)
{
	window.draw(character.bodyShape);
	window.draw(character.collisionShape);
}

void Game::ControlPlayer()
{
	if (gameStatus == PLAY)
	{
		if (Keyboard::isKeyPressed(Keyboard::Space))
			character.Jump();
		if (Keyboard::isKeyPressed(Keyboard::Down))
			character.Seat();
		if (Keyboard::isKeyPressed(Keyboard::Left))
			character.runStatus = RUN_LEFT;
		if (Keyboard::isKeyPressed(Keyboard::Right))
			character.runStatus = RUN_RIGHT;
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			gameStatus = PAUSE;
	}
}

void Game::MoveCharacter(Character& character, float elapsedTime)
{
	if (character.runStatus != NOT_RUN)
	{
		float movement = character.moveSpeed * elapsedTime;

		if (character.jumpStatus == FLY)
		{
			movement *= FLYING_SLOWDOWN;
		}
		if (character.runStatus == RUN_LEFT)
		{
			movement = -movement;
		}

		character.collisionShape.move(movement, 0);

		if (IsCollidesWithLevel(character.collisionShape))
		{
			character.collisionShape.move(-movement, 0);
		}
	}

	ApplyGravity(character, elapsedTime);

	character.runStatus = NOT_RUN;
	character.bodyShape.setPosition(character.GetCharacterPos());
}

void Game::ApplyGravity(Character& character, float elapsedTime)
{
	character.moveVelocity = character.moveVelocity + G * elapsedTime;
	float movementY = character.moveVelocity * elapsedTime;

	character.collisionShape.move(0, movementY);

	if (IsCollidesWithLevel(character.collisionShape))
	{
		character.collisionShape.move(0, -movementY);
		character.jumpStatus = ON_GROUND;
		character.moveVelocity = 0;
	}
	else
	{
		character.jumpStatus = FLY;
	}
}

bool Game::IsCollidesWithLevel(sf::RectangleShape& shape)
{
	auto collisionRect = shape.getGlobalBounds();

	for (unsigned int i = 0; i < mapTiles.size(); i++)
	{
		if (collisionRect.intersects(mapTiles[i].rect) && mapTiles[i].name == "solid")
		{
			return true;
		}
	}
	return false;
}