#include "menu.h"
#include <iostream>

static const sf::Vector2f RESOLUTION = { 1366, 768 };
const sf::Vector2f GAMENAME_POS = { RESOLUTION.x / 2.0f, 220 };
const float MAINMENU_ITEMS_POS_X = RESOLUTION.x / 2.0f;
const float ICON_MARGIN = 10;
const float MAIN_MENU_TOP_MARGIN = 400;
const float MAIN_MENU_ITEMS_MARGIN = 60;

bool Menu::InitMenuItems()
{
	if (gameNameTexture.loadFromFile("resources/GameName.png") &&
		gameMenuItemsFont.loadFromFile("resources/Nightmare.ttf") &&
		mainMenuWrapperTexture.loadFromFile("resources/wrapper_768p.png") &&
		mainMenuIconTexture.loadFromFile("resources/menuIconFace2.png"))
	{
		gameName.setSize(GAMENAME_SIZE);
		gameName.setOrigin(GAMENAME_SIZE.x / 2.0f, GAMENAME_SIZE.y);
		gameName.setTexture(&gameNameTexture);
		gameName.setPosition(GAMENAME_POS);

		mainMenuWrapper.setSize(RESOLUTION);
		mainMenuWrapper.setTexture(&mainMenuWrapperTexture);

		mainMenuIcon.setSize(ICON_SIZE);
		mainMenuIcon.setTexture(&mainMenuIconTexture);
		mainMenuIcon.setOrigin(ICON_SIZE.x, ICON_SIZE.y - ICON_MARGIN);

		sf::Text button1("Defeat fears", gameMenuItemsFont, FONT_SIZE);
		mainMenuItems[0] = button1;
		sf::Text button2("Choose map", gameMenuItemsFont, FONT_SIZE);
		mainMenuItems[1] = button2;
		sf::Text button3("Difficulty level", gameMenuItemsFont, FONT_SIZE);
		mainMenuItems[2] = button3;
		sf::Text button4("Im scared Leave", gameMenuItemsFont, FONT_SIZE);
		mainMenuItems[3] = button4;

		for (int i = 0; i < sizeof(mainMenuItems) / sizeof(mainMenuItems[0]); i++)
		{
			float textCenterX = mainMenuItems[i].getGlobalBounds().width / 2.0f;
			float textCenterY = mainMenuItems[i].getGlobalBounds().height;
			mainMenuItems[i].setOrigin(textCenterX, textCenterY);
			mainMenuItems[i].setPosition(MAINMENU_ITEMS_POS_X, MAIN_MENU_TOP_MARGIN + i * MAIN_MENU_ITEMS_MARGIN);
			mainMenuItems[i].setOutlineColor(FONT_OUTLINE_COLOR);
		}
		currentButton = MainMenu::START;
		buttonsColdown = 0;

		return true;
	}

	return false;
}

void Menu::UdateMainMenu()
{
	int buttonNumber = (int)currentButton;
	float menuItemWidth = mainMenuItems[buttonNumber].getGlobalBounds().width;
	float margin = menuItemWidth / 2.0f + ICON_MARGIN;
	mainMenuIcon.setPosition({ MAINMENU_ITEMS_POS_X - margin , mainMenuItems[buttonNumber].getPosition().y });
}

void Menu::DrawMainMenu(sf::RenderWindow& window)
{
	window.draw(mainMenuWrapper);
	for (int i = 0; i < sizeof(mainMenuItems) / sizeof(mainMenuItems[0]); i++)
	{
		window.draw(mainMenuItems[i]);
	}
	window.draw(gameName);
	window.draw(mainMenuIcon);
}