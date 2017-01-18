#pragma once
#include "stdafx.h"

const unsigned FONT_SIZE = 70;
const unsigned FONT_SIZE_MEDIUM = 55;
const unsigned FONT_SIZE_MINI = 40;

const sf::Color RESIZE_SETTINGS_BG_COLOR = sf::Color(14, 4, 21);
const sf::Color UNSELECTED_ITEM_COLOR = sf::Color(30, 30, 30);
const sf::Color SELECTED_BUTTON_COLOR = sf::Color::White;

const sf::Vector2f GAMENAME_SIZE = { 328, 180 };
const sf::Vector2i MAIN_MENU_WRAPPER_SIZE = { 1366, 768 };
const sf::Vector2i PAUSE_MENU_SIZE = { 900, 450 };
const sf::Vector2f ICON_SIZE = { 37 , 45 };

const float ICON_HORIZONTAL_MARGIN = 35;
const float MENU_FIRST_BUTTON_MARGIN = 30;
const float MENU_BUTTONS_MARGIN = 60;
const float GAMENAME_VERTICAL_MARGIN = 240;

const float BUTTONS_COLDOWN = 0.18f;

enum struct MenuType
{
	START = 0,
	DIFFICULT,
	PAUSE,
	RESIZE_SETTINGS,
};

enum struct Difficult
{
	EASY = 0,
	NORMAL,
	HARD,
};

struct Menu
{
	Menu::Menu(float width, float height);

	sf::Vector2f resolution;

	sf::Font buttonFont;

	MenuType currentMenu;
	size_t currentButton;

	sf::Texture gameNameTexture;
	sf::Texture mainWrapperTexture;
	sf::Texture pauseWrapperTexture;
	sf::Texture iconTexture;
	sf::RectangleShape gameName;
	sf::RectangleShape menuWrapper;
	sf::RectangleShape menuIcon;

	std::vector<std::vector<sf::Text>> allMenues;
	std::vector<sf::Text> mainMenuButtons;
	std::vector<sf::Text> pauseMenuButtons;
	std::vector<sf::Text> difficultMenuButtons;
	std::vector<sf::Text> resizeMenuButtons;

	float buttonsColdown;
	sf::Vector2f windowCenter;

	void SetMenu(MenuType const& menu, sf::Vector2f const& center);

	void Update(sf::Vector2f const& center);
	void Select(MenuType const& selectMenu, Difficult const& selectButton);
	void SwitchButtonUp();
	void SwitchButtonDown();

	void Draw(sf::RenderWindow& window);
};