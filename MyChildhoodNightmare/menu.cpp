#include "menu.h"
#include <iostream>

static const float RESOLUTION_WIDTH = 1280;
static const float RESOLUTION_HEIGHT = 720;
const sf::Vector2f GAMENAME_POS = { RESOLUTION_WIDTH / 2.0f, 220 };
const float MAINMENU_ITEMS_POS_X = RESOLUTION_WIDTH / 2.0f;
const float ICON_MARGIN = 10;

bool Menu::InitMenuItems()
{
    if (gameNameTexture.loadFromFile("resources/GameName.png") &&
        gameFont.loadFromFile("resources/Nightmare.ttf") &&
        mainMenuWrapperTexture.loadFromFile("resources/wrapper_720p.png") &&
        mainMenuIconTexture.loadFromFile("resources/menuIconFace2.png"))
    {
        gameName.setSize(GAMENAME_SIZE);
        gameName.setOrigin(GAMENAME_SIZE.x / 2.0f, GAMENAME_SIZE.y);
        gameName.setTexture(&gameNameTexture);
        gameName.setPosition(GAMENAME_POS);

        mainMenuWrapper.setSize({ RESOLUTION_WIDTH, RESOLUTION_HEIGHT });
        mainMenuWrapper.setTexture(&mainMenuWrapperTexture);

        mainMenuIcon.setSize(ICON_SIZE);
        mainMenuIcon.setTexture(&mainMenuIconTexture);
        mainMenuIcon.setOrigin(ICON_SIZE.x, ICON_SIZE.y - ICON_MARGIN);

        sf::Text button1("Defeat fears", gameFont, FONT_SIZE);
        mainMenuItems[0] = button1;
        sf::Text button2("Chenge map", gameFont, FONT_SIZE);
        mainMenuItems[1] = button2;
        sf::Text button3("Difficulty level", gameFont, FONT_SIZE);
        mainMenuItems[2] = button3;
        sf::Text button4("Im scared Leave", gameFont, FONT_SIZE);
        mainMenuItems[3] = button4;

        for (int i = 0; i < sizeof(mainMenuItems) / sizeof(mainMenuItems[0]); i++)
        {
            float textCenterX = mainMenuItems[i].getGlobalBounds().width / 2.0f;
            float textCenterY = mainMenuItems[i].getGlobalBounds().height;
            mainMenuItems[i].setOrigin(textCenterX, textCenterY);
            mainMenuItems[i].setPosition(MAINMENU_ITEMS_POS_X, 400 + i * 60);
            mainMenuItems[i].setOutlineColor(FONT_OUTLINE_COLOR);
        }

        currentButton = START;
        buttonsColdown = 0;

        return true;
    }
    
    return false;
}

void Menu::UdateMainMenu()
{
    int button = (int)currentButton;
    float margin = mainMenuItems[button].getGlobalBounds().width / 2.0f + ICON_MARGIN;
    mainMenuIcon.setPosition({ RESOLUTION_WIDTH / 2.0f - margin , mainMenuItems[button].getPosition().y });
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