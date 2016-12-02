#include "menu.h"
#include <iostream>

static const sf::Vector2f RESOLUTION = { 1366, 768 };
const sf::Vector2f GAMENAME_POS = { RESOLUTION.x / 2.0f, 220 };
const float ICON_HORIZONTAL_MARGIN = 35;
const float ICON_VERTICAL_MARGIN = 3;
const float MENU_TOP_MARGIN = 400;
const float MENU_ITEMS_MARGIN = 60;

bool Menu::InitMenuItems()
{
	if (gameNameTexture.loadFromFile("resources/GameName.png") &&
		itemsFont.loadFromFile("resources/nightmarealley.ttf") &&
		wrapperTexture.loadFromFile("resources/wrapper_768p.png") &&
		iconTexture.loadFromFile("resources/menuIconFace2.png"))
	{
		gameName.setSize(GAMENAME_SIZE);
		gameName.setOrigin(GAMENAME_SIZE.x / 2.0f, GAMENAME_SIZE.y);
		gameName.setTexture(&gameNameTexture);
		gameName.setPosition(GAMENAME_POS);

		menuWrapper.setSize(RESOLUTION);
		menuWrapper.setTexture(&wrapperTexture);
		menuWrapper.setPosition(0,0);

		menuIcon.setSize(ICON_SIZE);
		menuIcon.setTexture(&iconTexture);
		menuIcon.setOrigin(ICON_SIZE.x / 2.0f, ICON_SIZE.y / 2.0f);

		mainMenuItems = {
			sf::Text("Start", itemsFont, FONT_SIZE),
			sf::Text("Difficult", itemsFont, FONT_SIZE),
			sf::Text("Leave", itemsFont, FONT_SIZE)
		};

		difficultMenuItems = {
			sf::Text("Easy", itemsFont, FONT_SIZE),
			sf::Text("Normal", itemsFont, FONT_SIZE),
			sf::Text("Hard", itemsFont, FONT_SIZE),
			sf::Text("back", itemsFont, FONT_SIZE_MINI)
		};

		pauseMenuItems = {
			sf::Text("Continue", itemsFont, FONT_SIZE),
			sf::Text("Restart", itemsFont, FONT_SIZE),
			sf::Text("Main menu", itemsFont, FONT_SIZE)
		};

		allItems = {
			mainMenuItems,
			difficultMenuItems,
			pauseMenuItems
		};

		for (auto menuIt = allItems.begin(); menuIt != allItems.end(); menuIt++)
		{
			float margin = MENU_TOP_MARGIN;
			for (auto itemsIt = menuIt->begin(); itemsIt != menuIt->end(); itemsIt++)
			{
				float textCenterX = itemsIt->getGlobalBounds().width / 2.0f;
				float textCenterY = itemsIt->getGlobalBounds().height / 2.0f;
				itemsIt->setOrigin(textCenterX, textCenterY);
				itemsIt->setPosition(RESOLUTION.x / 2.0f, margin);
				itemsIt->setOutlineColor(FONT_OUTLINE_COLOR);
				margin += MENU_ITEMS_MARGIN;
			}
		}

		currentMenu = CurrentMenu::START;
		currentButton = 0;
		buttonsColdown = 0;

		return true;
	}

	return false;
}

void Menu::Update()
{
	auto currItem = allItems[(size_t)currentMenu][currentButton];
	auto menuItemWidth = currItem.getGlobalBounds().width;
	auto menuItemCenterY = currItem.getGlobalBounds().height / 2.0f + currItem.getPosition().y - ICON_VERTICAL_MARGIN;
	float currIonMargin = menuItemWidth / 2.0f + ICON_HORIZONTAL_MARGIN;

	menuIcon.setPosition({ RESOLUTION.x / 2.0f - currIonMargin , menuItemCenterY });
}

void Menu::Draw(sf::RenderWindow& window)
{
	auto currMenu = allItems[(size_t)currentMenu];

	window.draw(menuWrapper);
	for (auto itemIt = currMenu.begin(); itemIt != currMenu.end(); itemIt++)
	{
		window.draw(*itemIt);
	}
	window.draw(gameName);
	window.draw(menuIcon);
}