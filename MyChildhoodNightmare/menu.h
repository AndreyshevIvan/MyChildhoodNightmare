#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

const unsigned FONT_SIZE = 70; // 35
const sf::Color FONT_OUTLINE_COLOR = sf::Color(145, 131, 92);
const sf::Vector2f GAMENAME_SIZE = { 328, 180 };
const float BUTTONS_COLDOWN = 0.15f;
const sf::Vector2f ICON_SIZE = { 37 , 45 };

enum struct MainMenu
{
	START = 0,
	DIFFICULT,
	EXIT,
};

enum struct PauseMenu
{
	CONTINUE = 0,
	RESTART,
	MAIN_MENU,
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
	sf::Text difficultyLevelButton;
	sf::Text exitButton;
	sf::Text continueButton;
	sf::Text restartButton;
	sf::Text goToMainButton;
	sf::Text mainMenuItems[3] = { startButton, difficultyLevelButton, exitButton };
	sf::Text pauseMenuItems[3] = { continueButton, restartButton, goToMainButton };
	MainMenu currentButton;

	bool InitMenuItems();

	void UdateMainMenu();

	void DrawMainMenu(sf::RenderWindow& window);
};