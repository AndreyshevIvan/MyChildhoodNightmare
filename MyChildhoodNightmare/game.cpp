#include "game.h"

using namespace std;
using namespace sf;

bool Game::InitGame()
{
	if (!player.InitPlayer())
	{
		cout << "ERROR : Player textures not found" "\n";
		return false;
	}
	if (!map.InitMap())
	{
		cout << "ERROR : Map not found" "\n";
		return false;
	}
	gameStatus = PLAY;

	return true;
}

void Game::UpdateElapsedTime()
{
	elapsedTime = clock.getElapsedTime().asSeconds();
	clock.restart();
}

float Game::GetElapsedTime()
{
	return elapsedTime;
}

void Game::Control()
{
	if (gameStatus == PLAY)
	{
		if (Keyboard::isKeyPressed(Keyboard::Up))
			player.Jump();
		if (Keyboard::isKeyPressed(Keyboard::Down))
			player.Seat();
		if (Keyboard::isKeyPressed(Keyboard::Left))
			player.GoLeft();
		if (Keyboard::isKeyPressed(Keyboard::Right))
			player.GoRight();
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			gameStatus = PAUSE;
		player.pCollisionShape.setPosition(player.GetPlayerPos());
	}
}