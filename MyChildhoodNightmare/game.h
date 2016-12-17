#pragma once
#include "stdafx.h"
#include "menu.h"
#include "player.h"
#include "interface.h"
#include "enemies.h"

const sf::Vector2f RESOLUTION = { 1366, 768 };
const float CAMERA_VERTICAL_MARGIN = 80;
const sf::Color BACKGROUND_COLOR = sf::Color(20, 12, 28);

struct GameScene
{
	std::function<void(sf::RenderWindow& window)> toHandle;
	std::function<void()> onUpdate;
	std::function<void(sf::RenderWindow& window)> onDraw;
};

struct Game
{
	sf::View camera;
	sf::Clock clock;
	float elapsedTime;
	float buttonColdown;

	Player player;
	std::vector<Enemy*> enemies;

	std::vector<Object> mapTiles;
	sf::Vector2f mapSize;
	Level* currentLevel;
	Level level_1;
	Level level_2;
	sf::RectangleShape background_level_1;
	sf::Texture backgroundTexture_level_1;

	Menu menu;
	PlayerInterface interface;
	Difficult difficult;

	GameScene menuScene;
	GameScene gameplayScene;
	GameScene pauseScene;
	GameScene *currentScene = nullptr;

	bool InitGame();
	void StartGame();
	void SpawnEntities();

	void SetElapsedTime();
	sf::FloatRect GetCameraArea();

	bool IsCollidesWithLevel(sf::FloatRect const& rect);

	void ControlPlayer();
	void ControlMenu(sf::RenderWindow& window);
	void ControlMenuLogic(sf::RenderWindow& window);

	void UpdateCamera(sf::RenderWindow& window);
	void UpdateColdowns();
	void UpdatePlayer();
	void UpdateBullets();
	void UpdateEnemies();
	void UpdateInterface();
	void UpdateBackground();

	void CheckEntitiesCollides();

	void DrawLevel(sf::RenderWindow& window);
	void DrawBullets(sf::RenderWindow& window);
	void DrawEnemies(sf::RenderWindow& window);
};