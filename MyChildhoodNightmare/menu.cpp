#include "menu.h"
#include <iostream>

Menu::Menu(float width, float height)
	:resolution(width, height)
{
	gameNameTexture.loadFromFile("resources/GameName.png");
	itemsFont.loadFromFile("resources/nightmarealley.ttf");
	mainWrapperTexture.loadFromFile("resources/wrapper_768p.png");
	iconTexture.loadFromFile("resources/menuIconFace2.png");
	pauseWrapperTexture.loadFromFile("resources/pauseMenu.png");

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
}

void Menu::SetMenu(CurrentMenu const& menu, sf::Vector2f const& center)
{
	currentMenu = menu;
	currentButton = 0;
	buttonsColdown = 0;
	windowCenter = center;

	if (menu != CurrentMenu::PAUSE)
	{
		gameName.setPosition(
			windowCenter.x,
			windowCenter.y - resolution.y / 2.0f + GAMENAME_VERTICAL_MARGIN
		);
	}

	float margin = MENU_FIRST_BUTTON_MARGIN;
	auto currMenu = static_cast<size_t>(currentMenu);

	for (auto it = allMenues[currMenu].begin(); it != allMenues[currMenu].end(); it++)
	{
		it->setPosition(windowCenter.x, windowCenter.y + margin);
		margin += MENU_BUTTONS_MARGIN;
	}

	if (currentMenu == CurrentMenu::PAUSE)
	{
		menuWrapper.setSize(static_cast<sf::Vector2f>(PAUSE_MENU_SIZE));
		menuWrapper.setTexture(&pauseWrapperTexture);
		menuWrapper.setTextureRect(sf::IntRect({0, 0}, PAUSE_MENU_SIZE));
	}
	else if (currentMenu == CurrentMenu::START || currentMenu == CurrentMenu::DIFFICULT)
	{
		menuWrapper.setSize(resolution);
		menuWrapper.setTexture(&mainWrapperTexture);
		menuWrapper.setTextureRect(sf::IntRect({ 0, 0 }, MAIN_MENU_WRAPPER_SIZE));
	}

	menuWrapper.setOrigin(
		menuWrapper.getGlobalBounds().width / 2.0f,
		menuWrapper.getGlobalBounds().height / 2.0f
	);
	menuWrapper.setPosition(windowCenter);
}

void Menu::Update()
{
	auto currMenu = static_cast<size_t>(currentMenu);
	auto currItem = allMenues[currMenu][currentButton];

	auto menuItemWidth = currItem.getGlobalBounds().width;
	auto menuItemCenterY = currItem.getGlobalBounds().height / 2.0f + currItem.getPosition().y;
	float currIconMargin = menuItemWidth / 2.0f + ICON_HORIZONTAL_MARGIN;

	menuIcon.setPosition({ windowCenter.x - currIconMargin , menuItemCenterY });
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
			allMenues[selectMenuId][it].setFillColor(BUTTON_COLOR);
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