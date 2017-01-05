#pragma once
#include "stdafx.h"

const sf::Vector2f PLAYER_BAR_POS = { -620, -330 };

const sf::Vector2f PLAYER_HEALTH_BAR_SIZE = { 180, 70 };
const sf::Vector2f PLAYER_WEAPON_BAR_SIZE = { 110, 80 };
const sf::Vector2f PREVIEW_IMAGE_SIZE = { 400, 400 };
const sf::Vector2f BOX_SIZE = { 58, 50 };
const sf::Vector2f BOSS_BAR_SIZE = { 700, 82 };
const sf::Vector2f BOSS_HP_LINE_SIZE = { 630, 30 };
const sf::Vector2f REMARK_CLOUD_SIZE = { 188, 58 };

const int PLAYER_HP_FONT_SIZE = 50;
const int PLAYER_AMMO_FONT_SIZE = 28;
const int PREVIEW_FONT_SIZE = 35;
const int HELP_FONT_SIZE = 15;
const int ANNOUNCEMENT_FONT_SIZE = 20;

const float ANNOUNCEMENT_OUTLINE_THICKNESS = 2;

const sf::Vector2f PLAYER_HP_MARGIN = { 80, 2 };
const sf::Vector2f PLAYER_WEAPON_MARGIN = { 0, PLAYER_HEALTH_BAR_SIZE.y };
const sf::Vector2f PLAYER_AMMO_MARGIN = { 88, 121 };
const sf::Vector2f PREVIEW_TEXT_MARGIN = { 0, 220 };
const sf::Vector2f PLAYER_BOXES_MARGIN = { 0, PLAYER_HEALTH_BAR_SIZE.y + PLAYER_WEAPON_BAR_SIZE.y + 10 };
const sf::Vector2f BOSS_BAR_MARGIN = { 0, -280 };
const sf::Vector2f BOSS_HP_LINE_MARGIN = { -BOSS_HP_LINE_SIZE.x / 2.0f + 33, -33 };
const sf::Vector2f HELP_TEXT_MARGIN = { -665 , 350 };
const float BOXES_MIDLE_MARGIN = 60;
const int MAX_ANNOUNCEMENT_MARGIN = 40;
const sf::Vector2f REMARK_MARGIN = { -53, -100 };
const sf::Vector2f REMARK_TEXT_MARGIN = { REMARK_CLOUD_SIZE.x / 2.0f, 18 };

const int PART_COUNT = 3;

const float PART_DURATION = 3.45f;
const float PART_TRANSPARENCY_DURATION = 3.15f;
const float ANNOUNCEMENT_DURATION = 1;
const float ANNOUNCEMENT_TRANSPARENCY_DURATION = 0.5f;
const float REMARK_DURATION = 2.5f;
const float REMARK_TRANSPARENCY_DURATION = 0.5f;

const sf::Color BOSS_HP_LINE_COLOR = sf::Color(98, 10, 10, 255);
const sf::Color BOSS_HP_BAR_COLOR = sf::Color(255, 255, 255, 240);

const float DEMAGE_ANNOUNCEMENT_SPEED = 30;

const std::vector<std::string> KILL_REMARKS = {
	"EASY BREEZY",
	"NEXT PLEASE"
};

enum struct PreviewStatus
{
	HOUSE,
	CELLAR,
	BOX,
	MONSTERS,
	CONTROLLER,
};

enum struct RemarkType
{
	KILL,
	DEATH,
	BONUS,
};

struct Remark
{
	Remark(RemarkType const& type)
	{
		std::vector<std::string> REMARKS;

		switch (type)
		{
		case RemarkType::KILL:
			REMARKS = KILL_REMARKS;
		default:
			break;
		}

		remark = REMARKS[rand() % REMARKS.size()];

		cloudTexture.loadFromFile("resources/cloud.png");
		font.loadFromFile("resources/nightmarealley.ttf");

		cloud.setTexture(&cloudTexture);
		cloud.setSize(REMARK_CLOUD_SIZE);

		remarkText = sf::Text(remark, font, HELP_FONT_SIZE);
		remarkText.setOrigin(remarkText.getGlobalBounds().width / 2.0f, remarkText.getGlobalBounds().height / 2.0f);
		remarkText.setFillColor(sf::Color::Black);
	}

	sf::Font font;
	std::string remark;
	sf::Text remarkText;
	sf::RectangleShape cloud;
	sf::Texture cloudTexture;
	float duration = 0;

	void Update(sf::Vector2f const& position, float elapsedTime);
	void Draw(sf::RenderWindow& window);
};

struct PlayerInterface
{
	sf::Font font;

	sf::RectangleShape playerHealthBar;
	sf::Texture playerHealthBarTexture;
	sf::Text playerHealth;
	Remark* remark = nullptr;

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
	sf::Texture previewTextures[5];
	sf::Text previewText;
	PreviewStatus currentPreviewScene;
	sf::Text helpText;

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

	std::vector<sf::Text> demageAnnouncementText;
	std::vector<float> demageAnnouncementDuration;

	float previewPartColdown = 0;
	PreviewStatus currentPart = PreviewStatus::HOUSE;

	bool Init();

	void CreateBoxes(std::map<Level*, int> const& boxesMap, Level* level);
	void CreateRemark(RemarkType const& type);

	void UpdateRemark(float elapsedTime, sf::Vector2f const& position);
	void UpdateBarsPos(sf::Vector2f const& cameraPos);
	void UpdatePlayerHP(int health);
	void UpdatePlayerWeapon(int weapon, int ammo = 0);
	void UpdatePlayerBoxes(int currentBoxes);
	void UpdateBossBar(int bossMaxHealth, int bossHealth);
	void UpdateWin(sf::Vector2f const& windowCenter);
	
	bool UpdatePreview(sf::Vector2f const& position, float elapsedTime);
	void UpdateHelpButton(std::string const& helpStr, sf::Vector2f const& cameraPos);

	void CreateAnnouncement(sf::Vector2f const& position, std::string const& str);
	void UpdateAnnouncement(float elapsedTime);

	void Draw(sf::RenderWindow& window);
	void DrawPart(sf::RenderWindow& window);
	void DrawBossBar(sf::RenderWindow& window);
	void DrawWin(sf::RenderWindow& window);
	void DrawAnnouncement(sf::RenderWindow& window);
};

std::string IntToStr(int number);
sf::Vector2f GetTextureSize(sf::Texture const& texture);