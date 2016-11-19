#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const sf::Vector2f PLAYER_SIZE = { 75, 90 };
const sf::Vector2f PLAYER_COLLISION_SHAPE_SIZE = { PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y - 10 };
const sf::Vector2f PLAYER_START_POS = { 200 , 300 };
const float PLAYER_MOVE_SPEED = 1;

class Player
{
private:
	sf::Sprite pSprite;
	sf::Texture pTexture;
public:
	sf::RectangleShape pCollisionShape;

	bool InitPlayer();
	void DrawPlayer(sf::RenderWindow& window);
	void Jump();
	void Seat();
	void GoLeft();
	void GoRight();
	sf::Vector2f GetPlayerPos();
};