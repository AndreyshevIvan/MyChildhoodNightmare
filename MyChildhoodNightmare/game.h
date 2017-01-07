#pragma once
#include "stdafx.h"
#include "menu.h"
#include "player.h"
#include "interface.h"
#include "enemies.h"
#include "bonus.h"
#include "sound.h"

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
	Game(float width, float height);

	sf::Vector2f resolution;

	sf::View camera;
	sf::Clock clock;
	float elapsedTime;
	float buttonColdown;
	float gameOverColdown = 0;

	std::map<GameScene*, sf::Music*> sceneMusicMap;
	std::map<TmxLevel*, sf::Music*> levelMusicMap;

	Player player;

	std::vector<Enemy*> enemies;
	std::vector<Bullet*> enemyBullets;
	std::vector<Bonus*> bonuses;

	std::vector<TmxObject> blocks;
	std::vector<TmxObject> lava;

	sf::Vector2f mapSize;
	TmxLevel* currentLevel;
	TmxLevel level_0;
	TmxLevel level_1;
	TmxLevel level_2;
	TmxLevel level_3;
	std::map<TmxLevel*, TmxLevel*> changeLevelMap;

	sf::RectangleShape currentBackground;
	sf::RectangleShape background_level_0;
	sf::Texture backgroundTexture_level_0;
	sf::RectangleShape background_level_1;
	sf::Texture backgroundTexture_level_1;

	std::map<TmxLevel*, int> boxesCountMap;

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

	int bonusProbability;
	int demageIncrease;

	void StartGame();
	void Restart();
	void NextLevel(TmxLevel& level);
	void GetMapData();

	void DifficultAdjustment();

	void CheckCompletedLevel();

	void SpawnEntities();
	void SpawnItems(std::vector<TmxObject> const& spawns, BonusType const& type);
	void SpawnEnemies(std::vector<TmxObject> const& spawns, EnemyType const& type);

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