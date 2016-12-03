#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

const unsigned FONT_SIZE = 70; // 35
const unsigned FONT_SIZE_MEDIUM = 55;
const unsigned FONT_SIZE_MINI = 40;
const sf::Color FONT_OUTLINE_COLOR = sf::Color(145, 131, 92);
const sf::Color UNSELECTED_ITEM_COLOR = sf::Color(30, 30, 30);
const sf::Color ITEM_COLOR = sf::Color::White;
const sf::Vector2f GAMENAME_SIZE = { 328, 180 };
const sf::Vector2f PAUSE_MENU_SIZE = { 1200, 600 };
const sf::Vector2f ICON_SIZE = { 37 , 45 };
const float BUTTONS_COLDOWN = 0.18f;

enum struct CurrentMenu
{
	START = 0,
	DIFFICULT,
	PAUSE,
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
	sf::RectangleShape pauseWrapper;
	sf::RectangleShape menuIcon;
	sf::Text startButton;
	std::vector<sf::Text> mainMenuItems;
	std::vector<sf::Text> pauseMenuItems;
	std::vector<sf::Text> difficultMenuItems;
	std::vector<std::vector<sf::Text>> allItems;
	float buttonsColdown;
	sf::Vector2f pausePos;

	bool InitMenuItems();

	void Update();
	void Select(size_t selectMenuId, size_t selectButton);

	void Draw(sf::RenderWindow& window);
};