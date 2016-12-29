#pragma once
#include "stdafx.h"

const sf::Vector2f PLAYER_BAR_POS = { -620, -330 };

const sf::Vector2f PLAYER_HEALTH_BAR_SIZE = { 180, 70 };
const sf::Vector2f PLAYER_WEAPON_BAR_SIZE = { 110, 80 };
const sf::Vector2f PREVIEW_IMAGE_SIZE = { 400, 400 };
const sf::Vector2f BOX_SIZE = { 58, 50 };
const sf::Vector2f BOSS_BAR_SIZE = { 700, 82 };
const sf::Vector2f BOSS_HP_LINE_SIZE = { 630, 30 };

const int PLAYER_HP_FONT_SIZE = 50;
const int PLAYER_AMMO_FONT_SIZE = 28;
const int PREVIEW_FONT_SIZE = 35;

const sf::Vector2f PLAYER_HP_MARGIN = { 80, 2 };
const sf::Vector2f PLAYER_WEAPON_MARGIN = { 0, PLAYER_HEALTH_BAR_SIZE.y };
const sf::Vector2f PLAYER_AMMO_MARGIN = { 88, 121 };
const sf::Vector2f PREVIEW_TEXT_MARGIN = { 0, 220 };
const sf::Vector2f PLAYER_BOXES_MARGIN = { 0, PLAYER_HEALTH_BAR_SIZE.y + PLAYER_WEAPON_BAR_SIZE.y + 10 };
const sf::Vector2f BOSS_BAR_MARGIN = { 0, -280 };
const sf::Vector2f BOSS_HP_LINE_MARGIN = { -BOSS_HP_LINE_SIZE.x / 2.0f + 33, -33 };
const sf::Vector2f WIN_TEXT_MARGIN = {};
const float BOXES_MIDLE_MARGIN = 60;

const int PART_COUNT = 3;

const float PART_COLDOWN = 4.0f;
const float GAME_OVER_COLDOWN = 2;

const sf::Color BOSS_HP_LINE_COLOR = sf::Color(98, 10, 10, 255);
const sf::Color BOSS_HP_BAR_COLOR = sf::Color(255, 255, 255, 240);

enum struct PreviewStatus
{
	HOUSE,
	CELLAR,
	BOX,
	MONSTERS,
};

struct PlayerInterface
{
	sf::Font font;

	sf::RectangleShape playerHealthBar;
	sf::Texture playerHealthBarTexture;
	sf::Text playerHealth;

	sf::RectangleShape playerWeaponBar;
	sf::Texture playerMeleeBarTexture;
	sf::Texture playerShootgunBarTexture;
	sf::Texture playerAkBarTexture;
	sf::Text playerWeapon;
	sf::Text playerAmmo;

	std::vector<sf::RectangleShape> filtres;
	sf::RectangleShape blackFilter;
	sf::Texture blackFilterTexture;
	sf::RectangleShape redFilter;
	sf::Texture redFilterTexture;

	sf::RectangleShape previewImage;
	sf::Texture previewTextures[4];
	sf::Text previewText;

	std::vector<sf::RectangleShape*> boxes;
	sf::Texture boxTexture;

	sf::RectangleShape bossHPLine;
	sf::RectangleShape bossBar;
	sf::Texture bossBarTexture;

	sf::RectangleShape win;
	sf::Texture winTexture;
	sf::Text winText;

	sf::RectangleShape gameOver;
	sf::Texture gameOverTexture;

	float previewPartColdown = 0;
	PreviewStatus currentPart = PreviewStatus::HOUSE;

	bool Init();

	void CreateBoxes(int maxBoxes);

	void UpdateBarsPos(sf::Vector2f const& cameraPos);
	void UpdatePlayerHP(int health);
	void UpdatePlayerWeapon(int weapon, int ammo = 0);
	void UpdatePlayerBoxes(int currentBoxes);
	void UpdateBossBar(int bossMaxHealth, int bossHealth);
	void UpdateWin(sf::Vector2f const& windowCenter);

	bool UpdatePreview(sf::Vector2f const& position, float elapsedTime);

	void Draw(sf::RenderWindow& window);
	void DrawPart(sf::RenderWindow& window);
	void DrawBossBar(sf::RenderWindow& window);
	void DrawWin(sf::RenderWindow& window);
};

std::string IntToStr(int number);
sf::Vector2f GetTextureSize(sf::Texture const& texture);