#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

const unsigned FONT_SIZE = 70; // 35
const unsigned FONT_SIZE_MINI = 40; // 35
const sf::Color FONT_OUTLINE_COLOR = sf::Color(145, 131, 92);
const sf::Vector2f GAMENAME_SIZE = { 328, 180 };
const float BUTTONS_COLDOWN = 0.18f;
const sf::Vector2f ICON_SIZE = { 37 , 45 };

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
	sf::Texture wrapperTexture;
	sf::Texture iconTexture;
	sf::RectangleShape gameName;
	sf::RectangleShape menuWrapper;
	sf::RectangleShape menuIcon;
	sf::Text startButton;
	std::vector<sf::Text> mainMenuItems;
	std::vector<sf::Text> pauseMenuItems;
	std::vector<sf::Text> difficultMenuItems;
	std::vector<std::vector<sf::Text>> allItems;
	float buttonsColdown;

	bool InitMenuItems();

	void Update();

	void Draw(sf::RenderWindow& window);
};