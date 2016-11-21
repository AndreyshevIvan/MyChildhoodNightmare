#include "game.h"

using namespace std;
using namespace sf;

bool Game::InitGame()
{
	if (player.InitPlayer() && map.InitMap() && level.LoadFromFile("resources/firstTileset.tmx"))
	{
		camera.reset(sf::FloatRect(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT));
		gameStatus = PLAY;
		mapObj = level.GetAllObjects();

		return true;
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
		if (Keyboard::isKeyPressed(Keyboard::Up))
			player.Jump(elapsedTime);
		if (Keyboard::isKeyPressed(Keyboard::Down))
			player.Seat(elapsedTime);
		if (Keyboard::isKeyPressed(Keyboard::Left))
			player.GoLeft(elapsedTime);
		if (Keyboard::isKeyPressed(Keyboard::Right))
			player.GoRight(elapsedTime);
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			gameStatus = PAUSE;
		player.cCollisionShape.setPosition(player.GetCharacterPos());
	}
}

void Game::Collision(float const& elapsedTime)
{
	Vector2f mapPos = map.mSprite.getPosition();
	Vector2f playerPos = player.GetCharacterPos();
	auto playerBounds = player.cCollisionShape.getGlobalBounds();
	auto mapBounds = map.mSprite.getGlobalBounds();
	float step = PLAYER_MOVE_SPEED * elapsedTime;
}