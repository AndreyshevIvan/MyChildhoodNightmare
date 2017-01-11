#include <iostream>
#include <string>
#include "game.h"

using namespace std;
using namespace sf;


static const string GAME_NAME = "My Childhood Nightmare";
static const string SETTINGS_NAME = "Settings";

static const vector<sf::Vector2u> RESOLUTIONS = {
	sf::Vector2u(1024, 576),
	sf::Vector2u(1280, 720),
	sf::Vector2u(1366, 768),
	sf::Vector2u(1920, 1080)
};
static const sf::Vector2u START_RESOLUTION = RESOLUTIONS[0];



void InitScenes(Game& game);
void InitGamePlayScene(Game& game);
void InitMenuScene(Game& game);
void InitGameOverScene(Game& game);
void InitPreviewScene(Game& game);
void InitWinScene(Game& game);

bool EnterSettingsLoop(sf::RenderWindow& window, sf::Vector2u& newResolution);
void EnterGameLoop(sf::RenderWindow& window, Game& game);

void HandleEvents(sf::RenderWindow& window, Game& game);
void Update(Game& game);
void Render(sf::RenderWindow& window, Game& game);

int main()
{
	sf::VideoMode settingsVideoMode;
	settingsVideoMode.width = START_RESOLUTION.x;
	settingsVideoMode.height = START_RESOLUTION.y;
	sf::RenderWindow settingsWindow(settingsVideoMode, SETTINGS_NAME, sf::Style::Titlebar | sf::Style::Close);

	sf::Vector2u gameResolution;
	if (EnterSettingsLoop(settingsWindow, gameResolution))
	{
		sf::VideoMode gameVideoMode;
		gameVideoMode.width = static_cast<unsigned>(CAMERA_AREA_SIZE.x);
		gameVideoMode.height = static_cast<unsigned>(CAMERA_AREA_SIZE.y);
		sf::RenderWindow gameWindow(gameVideoMode, GAME_NAME, sf::Style::Titlebar | sf::Style::Close);
		gameWindow.setSize(gameResolution);

		Game game(static_cast<float>(gameResolution.x), static_cast<float>(gameResolution.y));

		InitScenes(game);
		EnterGameLoop(gameWindow, game);
	}

	return 0;
}

bool EnterSettingsLoop(sf::RenderWindow& window, sf::Vector2u& newResolution)
{
	sf::Clock clock;
	float buttonColdown = 0;

	sf::Vector2f windowSize = static_cast<sf::Vector2f>(window.getSize());
	sf::Vector2f windowCenter = windowSize * 0.5f;
	
	Menu settingsMenu(windowSize.x, windowSize.y);
	settingsMenu.SetMenu(MenuType::RESIZE_SETTINGS, windowCenter);
	size_t button = 0;

	while (window.isOpen())
	{
		bool IsButtonColdownEnd = buttonColdown > BUTTONS_COLDOWN;
		const float elapsedTime = clock.getElapsedTime().asSeconds();
		clock.restart();

		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return false;
			}

		if (!IsButtonColdownEnd)
			buttonColdown += elapsedTime;
		else
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				newResolution = RESOLUTIONS[button];
				break;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				button = (button == 0) ? (RESOLUTIONS.size() - 1) : (button - 1);
				settingsMenu.SwitchButtonUp();
				buttonColdown = 0;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				button = (button == RESOLUTIONS.size() - 1) ? 0 : (button + 1);
				settingsMenu.SwitchButtonDown();
				buttonColdown = 0;
			}
		}

		settingsMenu.Update(windowCenter);

		window.clear();
		settingsMenu.Draw(window);
		window.display();
	}

	window.close();
	return true;
}

void EnterGameLoop(sf::RenderWindow& window, Game& game)
{

	while (window.isOpen())
	{
		game.SetElapsedTime();

		if (game.elapsedTime >= 0.1)
		{
			game.elapsedTime = 0.016f;
		}

		HandleEvents(window, game);
		Update(game);
		Render(window, game);
		
		//cout << "FPS : " << static_cast<int>(1 / game.elapsedTime) << "\n";
	}
}

void HandleEvents(sf::RenderWindow& window, Game& game)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
	}
	game.currentScene->toHandle(window);
}

void Update(Game& game)
{
	game.UpdateOST();
	game.currentScene->onUpdate();
}

void Render(sf::RenderWindow& window, Game& game)
{
	window.clear(BACKGROUND_COLOR);
	window.draw(game.visualEffects.currentBackground);
	game.currentScene->onDraw(window);
	window.display();
}

void InitScenes(Game& game)
{
	InitGamePlayScene(game);
	InitMenuScene(game);
	InitGameOverScene(game);
	InitPreviewScene(game);
	InitWinScene(game);

	game.currentScene = &game.previewScene;
}

void InitGamePlayScene(Game& game)
{
	game.gameplayScene.toHandle = [&](sf::RenderWindow& window) {
		window;
		game.ControlPlayer();
	};
	game.gameplayScene.onUpdate = [&]() {
		game.UpdateColdowns();
		game.UpdatePlayer();
		game.UpdateEnemies();
		game.UpdateBullets();
		game.UpdateBonuses();
		game.CheckEntitiesCollides();
	};
	game.gameplayScene.onDraw = [&](sf::RenderWindow& window) {
		game.UpdateCamera(window);
		game.UpdateInterface();
		game.DrawLevel(window);
		game.player.DrawCharacter(window);
		game.DrawBullets(window);
		game.DrawEnemies(window);
		game.DrawBonuses(window);
		game.DrawInterface(window);
	};
}

void InitMenuScene(Game& game)
{
	game.menuScene.toHandle = [&](sf::RenderWindow& window) {
		game.ControlMenu(window);
	};
	game.menuScene.onUpdate = [&]() {
		game.UpdateColdowns();
		game.menu.Update(game.camera.getCenter());
	};
	game.menuScene.onDraw = [&](sf::RenderWindow& window) {
		if (game.menu.currentMenu == MenuType::PAUSE)
		{
			game.gameplayScene.onDraw(window);
		}
		window.setView(game.camera);
		game.menu.Draw(window);
	};
}

void InitGameOverScene(Game& game)
{
	game.gameOverScene.toHandle = [&](sf::RenderWindow& window) {
		game.ControlGameOver(window);
	};
	game.gameOverScene.onUpdate = [&]() {
		sf::RectangleShape* gameOverBack = &game.visualEffects.gameOver;
		auto pos = game.camera.getCenter();

		gameOverBack->setPosition(pos);
	};
	game.gameOverScene.onDraw = [&](sf::RenderWindow& window) {
		window.draw(game.visualEffects.gameOver);
	};
}

void InitPreviewScene(Game& game)
{
	game.previewScene.toHandle = [&](sf::RenderWindow& window) {
		(void)window;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			game.currentScene = &game.menuScene;
		}
	};
	game.previewScene.onUpdate = [&]() {
		auto cameraCenter = game.camera.getCenter();
		if (game.visualEffects.UpdatePreview(cameraCenter, game.elapsedTime))
		{
			game.currentScene = &game.menuScene;
		}
	};
	game.previewScene.onDraw = [&](sf::RenderWindow& window) {
		game.visualEffects.DrawPart(window);
	};
}

void InitWinScene(Game& game)
{
	game.winScene.toHandle = [&](sf::RenderWindow& window) {
		(void)window;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			game.menu.SetMenu(MenuType::START, game.camera.getCenter());
			game.currentScene = &game.menuScene;
		}
	};
	game.winScene.onUpdate = [&]() {
		auto cameraCenter = game.camera.getCenter();
		game.visualEffects.UpdateWin(cameraCenter);
	};
	game.winScene.onDraw = [&](sf::RenderWindow& window) {
		game.visualEffects.DrawWin(window);
	};
}