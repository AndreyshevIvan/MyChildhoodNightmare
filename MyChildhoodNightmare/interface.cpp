#include "interface.h"

using namespace std;
using namespace sf;

bool PlayerInterface::Init()
{
	if (!playerHealthBarTexture.loadFromFile("resources/healthBar.png"))
	{
		return false;
	}
	if (!font.loadFromFile("resources/nightmarealley.ttf"))
	{
		return false;
	}

	playerHealth = Text("", font, PLAYER_HP_FONT_SIZE);

	playerHealthBar.setSize(HEALTH_BAR_SIZE);
	playerHealthBar.setTexture(&playerHealthBarTexture);

	return true;
}

void PlayerInterface::UpdateBarsPos(Vector2f const& cameraPos)
{
	sf::Vector2f playerHPPos = {
		cameraPos.x - HEALTH_BAR_POS.x,
		cameraPos.y - HEALTH_BAR_POS.y
	};
	playerHealthBar.setPosition(playerHPPos);
	playerHealth.setPosition
	(
		playerHPPos.x + PLAYER_HP_MARGIN.x,
		playerHPPos.y + PLAYER_HP_MARGIN.y
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

void PlayerInterface::Draw(RenderWindow& window)
{
	window.draw(playerHealthBar);
	window.draw(playerHealth);
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