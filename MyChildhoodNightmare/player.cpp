#include "player.h"

using namespace std;
using namespace sf;

bool Player::InitPlayer()
{
	playerShape.setSize(PLAYER_SIZE);
	playerShape.setPosition(PLAYER_START_POS);
	playerShape.setFillColor(sf::Color::Blue);

	return true;
}

void Player::DrawPlayer(sf::RenderWindow& window)
{
	window.draw(playerShape);
}