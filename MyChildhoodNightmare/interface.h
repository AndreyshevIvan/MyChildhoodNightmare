#pragma once
#include "stdafx.h"

const sf::Vector2f PLAYER_BAR_POS = { -620, -330 };

const sf::Vector2f PLAYER_HEALTH_BAR_SIZE = { 180, 70 };
const sf::Vector2f PLAYER_WEAPON_BAR_SIZE = { 110, 80 };

const int PLAYER_HP_FONT_SIZE = 50;
const int PLAYER_AMMO_FONT_SIZE = 28;
const int PREVIEW_FONT_SIZE = 38;

const sf::Vector2f PLAYER_HP_MARGIN = { 80, 2 };
const sf::Vector2f PLAYER_WEAPON_MARGIN = { 0, PLAYER_HEALTH_BAR_SIZE.y };
const sf::Vector2f PLAYER_AMMO_MARGIN = { 88, 121 };

const float PART_COLDOWN = 3.0f;
const int PART_COUNT = 3;
const sf::Vector2f PREVIEW_IMAGE_SIZE = { 300, 300 };
const float TEXT_MARGIN = 170;

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

	sf::RectangleShape filter;
	sf::Texture filterTexture;

	sf::RectangleShape previewImage;
	sf::Texture previewTextures[4];
	sf::Text previewText;

	sf::RectangleShape gameOver;
	sf::Texture gameOverTexture;

	float previewPartColdown = 0;
	PreviewStatus currentPart = PreviewStatus::HOUSE;

	bool Init();

	void UpdateBarsPos(sf::Vector2f const& cameraPos);
	void UpdatePlayerHP(int health);
	void UpdatePlayerWeapon(int weapon, int ammo = 0);

	bool UpdatePreview(sf::Vector2f const& position, float elapsedTime);

	void Draw(sf::RenderWindow& window);
	void DrawPart(sf::RenderWindow& window);
};

std::string IntToStr(int number);
sf::Vector2f GetTextureSize(sf::Texture const& texture);