#include "game.h"

using namespace std;
using namespace sf;

bool Game::InitGame()
{
	if (!player.InitPlayer())
	{
		cout << "Player textures not found" "\n";
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			player.Jump();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			player.Seat();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			player.GoLeft();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			player.GoRight();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			gameStatus = PAUSE;
	}
}