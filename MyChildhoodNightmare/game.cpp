#include "game.h"

using namespace std;
using namespace sf;

bool Game::InitGame()
{
	if (level.LoadFromFile("resources/firstTileset.tmx"))
	{
		mapObj = level.GetAllObjects();

		if (player.InitPlayer() && map.InitMap())
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

void Game::Control()
{
	if (gameStatus == PLAY)
	{
		if (Keyboard::isKeyPressed(Keyboard::Space))
			player.Jump(elapsedTime);
		if (Keyboard::isKeyPressed(Keyboard::Down))
			player.Seat(elapsedTime);
		if (Keyboard::isKeyPressed(Keyboard::Left))
			player.GoLeft(elapsedTime);
		if (Keyboard::isKeyPressed(Keyboard::Right))
			player.GoRight(elapsedTime);
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			gameStatus = PAUSE;
	}
}

void Game::Collision()
{
	auto collisionRect = player.playerMoveRect;
	collisionRect.left += player.playerSpeed.x;
	collisionRect.top += player.playerSpeed.y;

	for (unsigned int i = 0; i < mapObj.size(); i++)
	{
		if (collisionRect.intersects(mapObj[i].rect) && mapObj[i].name == "solid")
		{
			if (player.playerRunStatus = RUN_RIGHT) // right
			{
				player.playerMoveRect.left -= player.playerSpeed.x;
				player.playerRunStatus = NOT_RUN;
			}
			if (player.playerRunStatus = RUN_LEFT) // left
			{
				player.playerMoveRect.left -= player.playerSpeed.x;
				player.playerRunStatus = NOT_RUN;
			}
			if (player.playerSpeed.y > 0) // bottom
			{

			}
			if (player.playerSpeed.y > 0) // top
			{

			}
		}
	}
}