#include "map.h"

using namespace std;
using namespace sf;

bool Map::InitMap()
{
	if (!mTexture.loadFromFile("resources/" + MAP_FILE_NAME, sf::IntRect(0, 0, 200, 40)))
	{
		return false;
	}
	mTexture.setSmooth(true);
	mSprite.setTexture(mTexture);
	mSprite.setOrigin(100, 20);
	mSprite.setPosition(800, 350);

	return true;
}

void Map::DrawMap(sf::RenderWindow& window)
{
	window.draw(mSprite);
}