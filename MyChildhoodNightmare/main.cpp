#include <iostream>
#include <string>
#include "game.h"

static const std::string GAME_NAME = "My Childhood Nightmare";

void InitScenes(Game& game);
void InitGamePlayScene(Game& game);
void InitMenuScene(Game& game);
void InitGameOverScene(Game& game);

void EnterGameLoop(sf::RenderWindow& window, Game& game);

void HandleEvents(sf::RenderWindow& window, Game& game);
void Update(Game& game);
void Render(sf::RenderWindow& window, Game& game);

int main()
{
	sf::VideoMode videoMode;
	videoMode.width = static_cast<unsigned>(RESOLUTION.x);
	videoMode.height = static_cast<unsigned>(RESOLUTION.y);
	sf::RenderWindow window(videoMode, GAME_NAME, sf::Style::Titlebar | sf::Style::Close);
	srand(static_cast<unsigned>(time(NULL)));

	Game game;

	if (game.InitGame())
	{
		InitScenes(game);
		EnterGameLoop(window, game);
	}
	else
	{
		std::cout << "ERROR: ANY FILES NOT FOUND.";
		system("pause");
		return 1;
	}

	return 0;
}

void EnterGameLoop(sf::RenderWindow& window, Game& game)
{
	while (window.isOpen())
	{
		game.SetElapsedTime();

		HandleEvents(window, game);
		Update(game);
		Render(window, game);
		
		//std::cout << "FPS : " << static_cast<int>(1 / game.elapsedTime) << "\n";
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
	game.currentScene->onUpdate();
}

void Render(sf::RenderWindow& window, Game& game)
{
	window.clear(BACKGROUND_COLOR);
	window.draw(game.background_level_1);
	game.currentScene->onDraw(window);
	window.display();
}

void InitScenes(Game& game)
{
	InitGamePlayScene(game);
	InitMenuScene(game);
	InitGameOverScene(game);

	game.currentScene = &game.menuScene;
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
		game.UpdateBackground();
		game.CheckEntitiesCollides();
	};
	game.gameplayScene.onDraw = [&](sf::RenderWindow& window) {
		game.UpdateCamera(window);
		game.UpdateInterface();
		game.DrawLevel(window);
		game.interface.Draw(window);
		game.player.Draw(window);
		game.DrawBullets(window);
		game.DrawEnemies(window);
		game.DrawBonuses(window);
	};
}

void InitMenuScene(Game& game)
{
	game.menuScene.toHandle = [&](sf::RenderWindow& window) {
		game.ControlMenu(window);
	};
	game.menuScene.onUpdate = [&]() {
		game.UpdateColdowns();
		game.menu.Update();
	};
	game.menuScene.onDraw = [&](sf::RenderWindow& window) {
		if (game.menu.currentMenu == CurrentMenu::PAUSE)
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
		sf::RectangleShape* gameOverBack = &game.interface.gameOver;
		auto pos = game.camera.getCenter();

		gameOverBack->setPosition(pos);
	};
	game.gameOverScene.onDraw = [&](sf::RenderWindow& window) {
		window.draw(game.interface.gameOver);
	};
}