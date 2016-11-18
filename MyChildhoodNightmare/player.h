#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const sf::Vector2f PLAYER_SIZE = { 50, 100 };
const sf::Vector2f PLAYER_START_POS = { 200 , 300 };

class Player
{
private:
public:
	sf::RectangleShape playerShape;
	sf::Texture playerTexture;
	bool InitPlayer();
	void DrawPlayer(sf::RenderWindow& window);
};