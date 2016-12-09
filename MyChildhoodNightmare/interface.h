#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <cstdlib>
#include <iostream>

const sf::Vector2f PLAYER_BAR_POS = { -620, -330 };
const sf::Vector2f PLAYER_BAR_SIZE = { 180, 140 };
const int PLAYER_HP_FONT_SIZE = 50;
const sf::Vector2f PLAYER_HP_MARGIN = { 80, 2 };
const sf::Vector2f POINTS_BAR_SIZE = { 80, 2 };
const sf::Vector2f PLAYER_POINTS_MARGIN = { 80, 2 };

struct DmgAnnouncement
{
	sf::RectangleShape shape;
	sf::Text dmg;
};

struct PlayerInterface
{
	sf::RectangleShape playerHealthBar;
	sf::Texture playerHealthBarTexture;
	sf::Text playerHealth;
	sf::Font font;

	bool Init();

	void UpdateBarsPos(sf::Vector2f const& cameraPos);
	void UpdatePlayerHP(float health);

	void Draw(sf::RenderWindow& window);
};

std::string IntToStr(int number);