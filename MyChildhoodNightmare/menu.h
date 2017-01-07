#pragma once
#include "stdafx.h"

const unsigned FONT_SIZE = 70; // 35
const unsigned FONT_SIZE_MEDIUM = 55;
const unsigned FONT_SIZE_MINI = 40;
const sf::Color UNSELECTED_ITEM_COLOR = sf::Color(30, 30, 30);
const sf::Color ITEM_COLOR = sf::Color::White;
const sf::Vector2f GAMENAME_SIZE = { 328, 180 };
const float GAMENAME_POS_Y = 240;
const sf::Vector2f PAUSE_MENU_SIZE = { 900, 450 };
const sf::Vector2f ICON_SIZE = { 37 , 45 };
const float BUTTONS_COLDOWN = 0.18f;

enum struct CurrentMenu
{
	START = 0,
	DIFFICULT,
	PAUSE,
};

enum struct Difficult
{
	EASY = 0,
	NORMAL,
	HARD,
};

struct Menu
{
	CurrentMenu currentMenu;
	size_t currentButton;
	sf::Font itemsFont;
	sf::Texture gameNameTexture;
	sf::Texture mainWrapperTexture;
	sf::Texture pauseWrapperTexture;
	sf::Texture iconTexture;
	sf::RectangleShape gameName;
	sf::RectangleShape menuWrapper;
	sf::RectangleShape menuIcon;
	sf::Text startButton;
	std::vector<sf::Text> mainMenuItems;
	std::vector<sf::Text> pauseMenuItems;
	std::vector<sf::Text> difficultMenuItems;
	std::vector<std::vector<sf::Text>> allMenues;
	float buttonsColdown;
	sf::Vector2f menuPos;

	bool InitMenuItems();

	void SetMenu(CurrentMenu const& menu, sf::Vector2f const& point);

	void Update();
	void Select(CurrentMenu const& selectMenu, Difficult const& selectButton);
	void SwitchButtonUp();
	void SwitchButtonDown();

	void Draw(sf::RenderWindow& window);
};