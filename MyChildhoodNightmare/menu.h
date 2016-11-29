#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

const unsigned FONT_SIZE = 70; // 35
const sf::Color FONT_OUTLINE_COLOR = sf::Color(145, 131, 92);
const sf::Vector2f GAMENAME_SIZE = { 328, 180 };
const float BUTTONS_COLDOWN = 0.15f;
const sf::Vector2f ICON_SIZE = { 37 , 45 };

enum MainMenu
{
	START = 0,
	CHENGE_MAP,
	DIFFICULT,
	EXIT
};

struct Menu
{
	sf::Font gameMenuItemsFont;
	sf::Texture gameNameTexture;
	sf::Texture mainMenuWrapperTexture;
	sf::Texture mainMenuIconTexture;
	sf::RectangleShape gameName;
	sf::RectangleShape mainMenuWrapper;
	sf::RectangleShape mainMenuIcon;
	sf::Text startButton;
	sf::Text chengeLevel;
	sf::Text difficultyLevelButton;
	sf::Text exitButton;
	sf::Text mainMenuItems[4] = { startButton, chengeLevel, difficultyLevelButton, exitButton };
	MainMenu currentButton;
	float buttonsColdown;

	bool InitMenuItems();

	void UdateMainMenu();

	void DrawMainMenu(sf::RenderWindow& window);
};