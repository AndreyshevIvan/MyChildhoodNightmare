#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const sf::Color BACKGROUND_COLOR = sf::Color(64, 1, 1);

class Map
{
private:
	sf::Sprite mSprite;
	sf::Texture mTexture;
public:
	bool InitMap();
	void DrawMap(sf::RenderWindow& window);
};