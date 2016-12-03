#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <functional>

#include "tinyxml/level.h"
#include "menu.h"
#include "player.h"
#include "enemy_shadow.h"

const sf::Vector2f RESOLUTION = { 1366, 768 };
const float CAMERA_VERTICAL_MARGIN = 80;
const sf::Color BACKGROUND_COLOR = sf::Color(20, 12, 28);

struct GameScene
{
	std::function<void(sf::RenderWindow& window, sf::Event& event)> toHandle;
	std::function<void()> onUpdate;
	std::function<void(sf::RenderWindow& window)> onDraw;
};

struct Game
{
	sf::Clock clock;
	sf::View camera;
	Player player;
	std::vector<EnemyShadow*> enemyShadows;
	Level level;
	Menu menu;
	std::vector<Object> mapTiles;
	float elapsedTime;
	sf::Vector2f mapSize;
	GameScene menuScene;
	GameScene gameplayScene;
	GameScene pauseScene;
	GameScene *currentScene = nullptr;
	Difficult difficult;

	bool InitGame();
	void StartGame();

	void SetElapsedTime();

	bool IsCollidesWithLevel(sf::FloatRect const& rect);

	void ControlPlayer(sf::Event& event);
	void ControlMenu(sf::RenderWindow& window, sf::Event& event);
	void ControlMenuLogic(sf::RenderWindow& window, sf::Event& event);

	void UpdateCamera(sf::RenderWindow& window);
	void UpdateColdowns();
	void UpdatePlayer();
	void UpdateBullets();
	void UpdateEnemies();

	void DrawLevel(sf::RenderWindow& window);
	void DrawPlayerBullets(sf::RenderWindow& window);
	void DrawEnemies(sf::RenderWindow& window);
};