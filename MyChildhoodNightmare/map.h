#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const std::string MAP_FILE_NAME = "map.png";
const sf::Color BACKGROUND_COLOR = sf::Color(64, 1, 1);

struct Map
{
	sf::Texture mTexture;
	sf::Sprite mSprite;

	bool InitMap();
	void DrawMap(sf::RenderWindow& window);
};