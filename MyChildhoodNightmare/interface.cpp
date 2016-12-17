#include "interface.h"

using namespace std;
using namespace sf;

bool PlayerInterface::Init()
{
	if (!playerHealthBarTexture.loadFromFile("resources/healthBar.png") ||
		!playerMeleeBarTexture.loadFromFile("resources/meleeBar.png") ||
		!playerShootgunBarTexture.loadFromFile("resources/shootgunBar.png") ||
		!playerAkBarTexture.loadFromFile("resources/akBar.png"))
	{
		return false;
	}
	if (!font.loadFromFile("resources/nightmarealley.ttf"))
	{
		return false;
	}

	playerHealth = Text("", font, PLAYER_HP_FONT_SIZE);
	playerAmmo = Text("", font, PLAYER_AMMO_FONT_SIZE);

	playerHealthBar.setSize(PLAYER_HEALTH_BAR_SIZE);
	playerHealthBar.setTexture(&playerHealthBarTexture);

	playerWeaponBar.setSize(PLAYER_WEAPON_BAR_SIZE);

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
}

void PlayerInterface::UpdatePlayerHP(float health)
{
	if (health > 0)
	{
		string hpStr = IntToStr(static_cast<int>(health));
		playerHealth.setString(hpStr);
	}
	else
	{
		playerHealth.setString("0");
	}
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
	else
	{
		playerAmmo.setString("0");
	}
}

void PlayerInterface::Draw(RenderWindow& window)
{
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