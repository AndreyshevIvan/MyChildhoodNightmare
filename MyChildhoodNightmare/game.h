#pragma once
#include "stdafx.h"
#include "menu.h"
#include "player.h"
#include "interface.h"
#include "enemies.h"
#include "bonus.h"

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
	float gameOverColdown = 0;

	Player player;

	std::vector<Enemy*> enemies;
	std::vector<Bullet*> enemyBullets;
	std::vector<Bonus*> bonuses;

	std::vector<Object> blocks;
	std::vector<Object> lava;

	sf::Vector2f mapSize;
	Level* currentLevel;
	Level level_1;
	Level level_2;
	Level level_3;
	sf::RectangleShape background_level_1;
	sf::Texture backgroundTexture_level_1;

	std::map<Level*, int> boxesCoundMap = {
			{ &level_1, 3 },
			{ &level_2, 1 }
	};

	Menu menu;
	PlayerInterface interface;
	Difficult difficult;

	GameScene menuScene;
	GameScene gameplayScene;
	GameScene pauseScene;
	GameScene gameOverScene;
	GameScene previewScene;
	GameScene winScene;
	GameScene *currentScene = nullptr;

	int bonusProbability = BONUS_PROBABILITY_EASY;

	bool InitGame();
	void StartGame(Level& level);
	void CheckCompletedLevel();

	void SpawnEntities();
	void SpawnBonuses();

	void SetElapsedTime();
	sf::FloatRect GetCameraArea();

	bool IsCollidesWithLevel(sf::FloatRect const& rect);

	void ControlPlayer();
	void ControlMenu(sf::RenderWindow& window);
	void ControlMenuLogic(sf::RenderWindow& window);
	void ControlGameOver(sf::RenderWindow& window);

	void UpdateCamera(sf::RenderWindow& window);
	void UpdateColdowns();
	void UpdatePlayer();
	void UpdateBullets();
	void UpdateEnemies();
	void UpdateInterface();
	void UpdateBackground();
	void UpdateBonuses();

	void CheckEntitiesCollides();
	void EnemyPlayerCollides();
	void PlayerBulletsEnemyCollides();
	void EnemyBulletsPlayerCollides();
	void BonusesPlayerCollides();
	void PlayerLavaCollides();

	void DrawLevel(sf::RenderWindow& window);
	void DrawBullets(sf::RenderWindow& window);
	void DrawEnemies(sf::RenderWindow& window);
	void DrawBonuses(sf::RenderWindow& window);
	void DrawInterface(sf::RenderWindow& window);

	void ClearScene();
};