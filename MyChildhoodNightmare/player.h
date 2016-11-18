#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const sf::Vector2f PLAYER_SIZE = { 75, 90 };
const sf::Vector2f PLAYER_START_POS = { 200 , 300 };
const float PLAYER_MOVE_SPEED = 1;

class Player
{
private:
	sf::Sprite playerSprite;
	sf::Texture playerTexture;
public:
	bool InitPlayer();
	void DrawPlayer(sf::RenderWindow& window);
	void Jump();
	void Seat();
	void GoLeft();
	void GoRight();
};