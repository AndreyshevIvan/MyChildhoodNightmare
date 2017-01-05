#pragma once
#include "stdafx.h"
#include "menu.h"
#include "player.h"
#include "interface.h"
#include "enemies.h"
#include "bonus.h"
#include "sound.h"

const sf::Vector2f RESOLUTION = { 1366, 768 };
const float CAMERA_VERTICAL_MARGIN = 80;
const sf::Color BACKGROUND_COLOR = sf::Color(20, 12, 28);

const int LEVEL_0_BOXES_COUNT = 0;
const int LEVEL_1_BOXES_COUNT = 3;
const int LEVEL_2_BOXES_COUNT = 3;

struct GameScene
{
	std::function<void(sf::RenderWindow& window)> toHandle;
	std::function<void()> onUpdate;
	std::function<void(sf::RenderWindow& window)> onDraw;
};

struct Game : GameSound
{
	sf::View camera;
	sf::Clock clock;
	float elapsedTime;
	float buttonColdown;
	float gameOverColdown = 0;

	std::map<GameScene*, sf::Music*> sceneMusicMap;
	std::map<Level*, sf::Music*> levelMusicMap;

	Player player;

	std::vector<Enemy*> enemies;
	std::vector<Bullet*> enemyBullets;
	std::vector<Bonus*> bonuses;

	std::vector<Object> blocks;
	std::vector<Object> lava;

	sf::Vector2f mapSize;
	Level* currentLevel;
	Level level_0;
	Level level_1;
	Level level_2;
	Level level_3;
	std::map<Level*, Level*> changeLevelMap;

	sf::RectangleShape currentBackground;
	sf::RectangleShape background_level_0;
	sf::Texture backgroundTexture_level_0;
	sf::RectangleShape background_level_1;
	sf::Texture backgroundTexture_level_1;

	std::map<Level*, int> boxesCountMap;

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
	void StartGame();
	void Restart();
	void NextLevel(Level& level);
	void GetMapData();

	void DifficultAdjustment();

	void CheckCompletedLevel();

	void SpawnEntities();
	void SpawnItems(std::vector<Object> const& spawns, BonusType const& type);
	void SpawnEnemies(std::vector<Object> const& spawns, EnemyType const& type);

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
	void UpdateOST();

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