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
		mainWrapperTexture.loadFromFile("resources/wrapper_768p.png") &&
		iconTexture.loadFromFile("resources/menuIconFace2.png") &&
		pauseWrapperTexture.loadFromFile("resources/pauseMenu.png"))
	{
		gameName.setSize(GAMENAME_SIZE);
		gameName.setOrigin(GAMENAME_SIZE.x / 2.0f, GAMENAME_SIZE.y);
		gameName.setTexture(&gameNameTexture);
		gameName.setPosition(GAMENAME_POS);

		menuWrapper.setSize(RESOLUTION);
		menuWrapper.setTexture(&mainWrapperTexture);

		pauseWrapper.setSize(PAUSE_MENU_SIZE);
		pauseWrapper.setOrigin( PAUSE_MENU_SIZE.x / 2.0f, PAUSE_MENU_SIZE.y / 2.0f );
		pauseWrapper.setTexture(&pauseWrapperTexture);

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
	float currIconMargin = menuItemWidth / 2.0f + ICON_HORIZONTAL_MARGIN;

	if (currentMenu == CurrentMenu::PAUSE)
	{
		float margin = MENU_TOP_MARGIN;
		for (auto itemsIt = allItems[(size_t)currentMenu].begin(); itemsIt != allItems[(size_t)currentMenu].end(); itemsIt++)
		{
			itemsIt->setPosition(pausePos.x, pausePos.y - RESOLUTION.y / 2.0f + margin);
			margin += MENU_ITEMS_MARGIN;
		}

		menuIcon.setPosition({ pausePos.x - currIconMargin , menuItemCenterY });
	}
	else
	{
		menuIcon.setPosition({ RESOLUTION.x / 2.0f - currIconMargin , menuItemCenterY });
	}
}

void Menu::Select(size_t selectMenuId, size_t selectButtonId)
{
	for (size_t item = 0; item < allItems[selectMenuId].size() - 1; item++)
	{
		if (item != selectButtonId)
		{
			allItems[selectMenuId][item].setFillColor(UNSELECTED_ITEM_COLOR);
		}
		else
		{
			allItems[selectMenuId][item].setFillColor(ITEM_COLOR);
		}
	}
}

void Menu::Draw(sf::RenderWindow& window)
{
	if (currentMenu == CurrentMenu::PAUSE)
	{
		pauseWrapper.setPosition(pausePos);
		window.draw(pauseWrapper);
	}
	else
	{
		window.draw(menuWrapper);
		window.draw(gameName);
	}

	auto currMenu = allItems[(size_t)currentMenu];
	for (auto itemsIt = currMenu.begin(); itemsIt != currMenu.end(); itemsIt++)
	{
		window.draw(*itemsIt);
	}
	window.draw(menuIcon);
}