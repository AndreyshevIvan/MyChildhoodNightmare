#include "map.h"

using namespace std;
using namespace sf;

bool Map::InitMap()
{
	if (!mTexture.loadFromFile("resources/map.png", sf::IntRect(0, 0, 200, 40)))
		return false;

	mTexture.setSmooth(true);
	mSprite.setTexture(mTexture);
	mSprite.setPosition(600, 400);

	return true;
}

void Map::DrawMap(sf::RenderWindow& window)
{
	window.draw(mSprite);
}