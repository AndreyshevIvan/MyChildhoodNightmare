#include "menu.h"
#include <iostream>

static const sf::Vector2f RESOLUTION = { 1366, 768 };
const float ICON_HORIZONTAL_MARGIN = 35;
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

		menuIcon.setSize(ICON_SIZE);
		menuIcon.setTexture(&iconTexture);
		menuIcon.setOrigin(ICON_SIZE.x / 2.0f, ICON_SIZE.y / 2.0f);

		mainMenuItems = {
			sf::Text("Start", itemsFont, FONT_SIZE),
			sf::Text("Difficulty", itemsFont, FONT_SIZE),
			sf::Text("Leave", itemsFont, FONT_SIZE)
		};

		difficultMenuItems = {
			sf::Text("Easy", itemsFont, FONT_SIZE),
			sf::Text("Normal", itemsFont, FONT_SIZE),
			sf::Text("Hard", itemsFont, FONT_SIZE),
			sf::Text("back", itemsFont, FONT_SIZE_MINI)
		};

		pauseMenuItems = {
			sf::Text("Continue", itemsFont, FONT_SIZE_MEDIUM),
			sf::Text("Restart", itemsFont, FONT_SIZE_MEDIUM),
			sf::Text("Main menu", itemsFont, FONT_SIZE_MEDIUM)
		};

		allMenues = {
			mainMenuItems,
			difficultMenuItems,
			pauseMenuItems
		};

		for (auto menu = allMenues.begin(); menu != allMenues.end(); menu++)
		{
			for (auto button = menu->begin(); button != menu->end(); button++)
			{
				button->setOrigin(
					button->getGlobalBounds().width / 2.0f,
					button->getGlobalBounds().height / 2.0f
				);
			}
		}

		return true;
	}

	return false;
}

void Menu::SetMenu(CurrentMenu const& menu, sf::Vector2f const& point)
{
	currentMenu = menu;
	currentButton = 0;
	buttonsColdown = 0;
	menuPos = point;

	if (menu != CurrentMenu::PAUSE)
	{
		gameName.setPosition(
			menuPos.x,
			menuPos.y - RESOLUTION.y / 2.0f + GAMENAME_POS_Y
		);
	}

	float margin = MENU_TOP_MARGIN;
	auto currMenu = static_cast<size_t>(currentMenu);

	for (auto it = allMenues[currMenu].begin(); it != allMenues[currMenu].end(); it++)
	{
		it->setPosition(menuPos.x, menuPos.y - RESOLUTION.y / 2.0f + margin);
		margin += MENU_ITEMS_MARGIN;
	}
}

void Menu::Update()
{
	auto currMenu = static_cast<size_t>(currentMenu);
	auto currItem = allMenues[currMenu][currentButton];

	auto menuItemWidth = currItem.getGlobalBounds().width;
	auto menuItemCenterY = currItem.getGlobalBounds().height / 2.0f + currItem.getPosition().y;
	float currIconMargin = menuItemWidth / 2.0f + ICON_HORIZONTAL_MARGIN;

	menuIcon.setPosition({ menuPos.x - currIconMargin , menuItemCenterY });

	if (currentMenu == CurrentMenu::PAUSE)
	{
		menuWrapper.setSize(PAUSE_MENU_SIZE);
		menuWrapper.setTextureRect(sf::IntRect(0, 0, (int)PAUSE_MENU_SIZE.x, (int)PAUSE_MENU_SIZE.y));
		menuWrapper.setTexture(&pauseWrapperTexture);
	}
	else
	{
		menuWrapper.setSize(RESOLUTION);
		menuWrapper.setTextureRect(sf::IntRect(0, 0, (int)RESOLUTION.x, (int)RESOLUTION.y));
		menuWrapper.setTexture(&mainWrapperTexture);
	}

	menuWrapper.setOrigin(
		menuWrapper.getGlobalBounds().width / 2.0f,
		menuWrapper.getGlobalBounds().height / 2.0f
	);
	menuWrapper.setPosition(menuPos);
}

void Menu::Select(CurrentMenu const& selectMenu, Difficult const& selectButton)
{
	size_t selectMenuId = static_cast<size_t>(selectMenu);
	size_t selectButtonId = static_cast<size_t>(selectButton);

	for (size_t it = 0; it < allMenues[selectMenuId].size() - 1; it++)
	{
		if (it != selectButtonId)
		{
			allMenues[selectMenuId][it].setFillColor(UNSELECTED_ITEM_COLOR);
		}
		else
		{
			allMenues[selectMenuId][it].setFillColor(ITEM_COLOR);
		}
	}
}

void Menu::SwitchButtonUp()
{
	auto currMenu = allMenues[(size_t)currentMenu];
	auto maxItem = currMenu.size() - 1;

	buttonsColdown = 0;
	if (currentButton == 0)
	{
		currentButton = maxItem;
	}
	else
	{
		currentButton = currentButton - 1;
	}
}

void Menu::SwitchButtonDown()
{
	auto currMenu = allMenues[(size_t)currentMenu];
	auto maxItem = currMenu.size() - 1;

	buttonsColdown = 0;
	if (currentButton == maxItem)
	{
		currentButton = 0;
	}
	else
	{
		currentButton = currentButton + 1;
	}
}


void Menu::Draw(sf::RenderWindow& window)
{
	window.draw(menuWrapper);

	if (currentMenu != CurrentMenu::PAUSE)
	{
		window.draw(gameName);
	}

	auto currMenu = allMenues[(size_t)currentMenu];
	for (auto menuElement : currMenu)
	{
		window.draw(menuElement);
	}
	window.draw(menuIcon);
}