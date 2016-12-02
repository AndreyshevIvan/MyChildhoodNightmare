#include <iostream>
#include <string>
#include "game.h"

static const std::string GAME_NAME = "My Childhood Nightmare";

void InitScenes(Game& game);
void InitGamePlayScene(Game& game);
void InitMenuScene(Game& game);

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

	Game game;

	if (game.InitGame())
	{
		InitScenes(game);
		EnterGameLoop(window, game);
	}
	else
	{
		std::cout << "ERROR: FILES NOT FOUND.";
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
	}
}

void HandleEvents(sf::RenderWindow& window, Game& game)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}
	game.currentScene->toHandle(window, event);
}

void Update(Game& game)
{
	game.currentScene->onUpdate();
}

void Render(sf::RenderWindow& window, Game& game)
{
	window.clear(BACKGROUND_COLOR);
	game.currentScene->onDraw(window);
	window.display();
}

void InitScenes(Game& game)
{
	InitGamePlayScene(game);
	InitMenuScene(game);

	game.currentScene = &game.menuScene;
}

void InitGamePlayScene(Game& game)
{
	game.gameplayScene.toHandle = [&](sf::RenderWindow& window, sf::Event& event) {
		window;
		game.ControlPlayer(event);
	};
	game.gameplayScene.onUpdate = [&]() {
		game.UpdateColdowns();
		game.UpdatePlayer();
		game.UpdateEnemies();
	};
	game.gameplayScene.onDraw = [&](sf::RenderWindow& window) {
		game.UpdateCamera(window);
		game.DrawLevel(window);
		game.player.Draw(window);
		game.DrawPlayerBullets(window);
		game.DrawEnemies(window);
	};
}

void InitMenuScene(Game& game)
{
	game.menuScene.toHandle = [&](sf::RenderWindow& window, sf::Event& event) {
		game.ControlMenu(window, event);
	};
	game.menuScene.onUpdate = [&]() {
		game.camera.reset(sf::FloatRect(0, 0, RESOLUTION.x, RESOLUTION.y));
		game.UpdateColdowns();
		game.menu.Update();
	};
	game.menuScene.onDraw = [&](sf::RenderWindow& window) {
		window.setView(game.camera);
		game.menu.Draw(window);
	};
}