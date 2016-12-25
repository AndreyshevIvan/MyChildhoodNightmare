#include "interface.h"

using namespace std;
using namespace sf;

const sf::Vector2f RESOLUTION = { 1366 , 768 };

bool PlayerInterface::Init()
{
	if (!playerHealthBarTexture.loadFromFile("resources/healthBar.png") ||
		!playerMeleeBarTexture.loadFromFile("resources/meleeBar.png") ||
		!playerShootgunBarTexture.loadFromFile("resources/shootgunBar.png") ||
		!playerAkBarTexture.loadFromFile("resources/akBar.png") ||
		!gameOverTexture.loadFromFile("resources/gameOver.png") ||
		!filterTexture.loadFromFile("resources/filter.png"))
	{
		return false;
	}
	if (!font.loadFromFile("resources/nightmarealley.ttf"))
	{
		return false;
	}

	playerHealth = Text("", font, PLAYER_HP_FONT_SIZE);
	playerAmmo = Text("", font, PLAYER_AMMO_FONT_SIZE);

	playerHealthBar.setSize(GetTextureSize(playerHealthBarTexture));
	playerHealthBar.setTexture(&playerHealthBarTexture);

	playerWeaponBar.setSize(PLAYER_WEAPON_BAR_SIZE);

	gameOver.setTexture(&gameOverTexture);
	gameOver.setSize(GetTextureSize(gameOverTexture));
	gameOver.setOrigin(gameOver.getSize().x / 2.0f , gameOver.getSize().y / 2.0f);

	filter.setSize(RESOLUTION);
	filter.setTexture(&filterTexture);
	filter.setOrigin(RESOLUTION.x / 2.0f, RESOLUTION.y / 2.0f);

	return true;
}

void PlayerInterface::UpdateBarsPos(Vector2f const& cameraPos)
{
	sf::Vector2f playerHealthPos = cameraPos + PLAYER_BAR_POS;
	sf::Vector2f playerWeaponBarPos = playerHealthPos + PLAYER_WEAPON_MARGIN;

	playerHealthBar.setPosition(playerHealthPos);
	playerWeaponBar.setPosition(playerWeaponBarPos);

	playerHealth.setPosition
	(
		playerHealthPos.x + PLAYER_HP_MARGIN.x,
		playerHealthPos.y + PLAYER_HP_MARGIN.y
	);

	playerAmmo.setOrigin(playerAmmo.getGlobalBounds().width / 2.0f, playerAmmo.getGlobalBounds().height / 2.0f );
	playerAmmo.setPosition
	(
		playerHealthPos.x + PLAYER_AMMO_MARGIN.x,
		playerHealthPos.y + PLAYER_AMMO_MARGIN.y
	);

	filter.setPosition(cameraPos);
}

void PlayerInterface::UpdatePlayerHP(int health)
{
	string hpStr = IntToStr(health);
	playerHealth.setString(hpStr);
}

void PlayerInterface::UpdatePlayerWeapon(int weapon, int ammo)
{
	switch (weapon)
	{
	case 0:
		playerWeaponBar.setTexture(&playerMeleeBarTexture);
		break;
	case 1:
		playerWeaponBar.setTexture(&playerShootgunBarTexture);
		break;
	case 2:
		playerWeaponBar.setTexture(&playerAkBarTexture);
		break;
	default:
		break;
	}

	if (ammo > 0)
	{
		string ammoStr = IntToStr(ammo);
		playerAmmo.setString(ammoStr);
	}
	else if (ammo < 0)
	{
		playerAmmo.setString("");
	}
	else
	{
		playerAmmo.setString("0");
	}
}

void PlayerInterface::Draw(RenderWindow& window)
{
	window.draw(filter);
	window.draw(playerHealthBar);
	window.draw(playerHealth); 
	window.draw(playerWeaponBar);
	window.draw(playerAmmo);
}

string IntToStr(int number)
{
	string result = "";

	while (number > 0)
	{
		int digit = number % 10;
		number = (number - digit) / 10;

		result = char(digit + '0') + result;
	}

	return result;
}

sf::Vector2f GetTextureSize(sf::Texture const& texture)
{
	const sf::Vector2f size = {
		static_cast<float>(texture.getSize().x),
		static_cast<float>(texture.getSize().y)
	};

	return size;
}