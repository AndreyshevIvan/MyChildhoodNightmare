#include "interface.h"

using namespace std;
using namespace sf;

const sf::Vector2f RESOLUTION = { 1366 , 768 };

enum
{
	SWORD = 0,
	SHOOTGUN = 1,
	AK = 2,
};

bool PlayerInterface::Init()
{
	if (!playerHealthBarTexture.loadFromFile("resources/healthBar.png") ||
		!playerMeleeBarTexture.loadFromFile("resources/meleeBar.png") ||
		!playerShootgunBarTexture.loadFromFile("resources/shootgunBar.png") ||
		!playerAkBarTexture.loadFromFile("resources/akBar.png") ||
		!gameOverTexture.loadFromFile("resources/gameOver.png") ||
		!blackFilterTexture.loadFromFile("resources/blackFilter.png") ||
		!redFilterTexture.loadFromFile("resources/redFilter.png") ||
		!previewTextures[0].loadFromFile("resources/house.png") ||
		!previewTextures[1].loadFromFile("resources/cellar.png") ||
		!previewTextures[2].loadFromFile("resources/teaserBox.png") ||
		!previewTextures[3].loadFromFile("resources/notAlone.png") ||
		!boxTexture.loadFromFile("resources/box.png") ||
		!bossBarTexture.loadFromFile("resources/bossBar.png") ||
		!winTexture.loadFromFile("resources/win.png"))
	{
		return false;
	}

	if (!font.loadFromFile("resources/nightmarealley.ttf"))
	{
		return false;
	}

	playerHealth = Text("", font, PLAYER_HP_FONT_SIZE);
	playerAmmo = Text("", font, PLAYER_AMMO_FONT_SIZE);
	previewText = Text("", font, PREVIEW_FONT_SIZE);
	winText = Text("", font, PREVIEW_FONT_SIZE);
	helpText = Text("", font, HELP_FONT_SIZE);

	playerHealthBar.setSize(GetTextureSize(playerHealthBarTexture));
	playerHealthBar.setTexture(&playerHealthBarTexture);
	playerWeaponBar.setSize(PLAYER_WEAPON_BAR_SIZE);

	bossBar.setSize(BOSS_BAR_SIZE);
	bossBar.setOrigin(BOSS_BAR_SIZE.x / 2.0f, BOSS_BAR_SIZE.y / 2.0f);
	bossBar.setTexture(&bossBarTexture);
	bossBar.setFillColor(BOSS_HP_BAR_COLOR);
	bossHPLine.setSize(BOSS_HP_LINE_SIZE);
	bossHPLine.setFillColor(BOSS_HP_LINE_COLOR);

	gameOver.setTexture(&gameOverTexture);
	gameOver.setSize(GetTextureSize(gameOverTexture));
	gameOver.setOrigin(gameOver.getSize().x / 2.0f , gameOver.getSize().y / 2.0f);

	win.setTexture(&winTexture);
	win.setSize(GetTextureSize(winTexture));
	win.setOrigin(win.getSize().x / 2.0f, win.getSize().y / 2.0f);
	winText.setString("You've conquered your nightmares and escaped!");
	winText.setOrigin(winText.getGlobalBounds().width / 2.0f, winText.getGlobalBounds().height / 2.0f);

	blackFilter.setSize(RESOLUTION);
	blackFilter.setTexture(&blackFilterTexture);
	blackFilter.setOrigin(RESOLUTION.x / 2.0f, RESOLUTION.y / 2.0f);

	previewImage.setSize(PREVIEW_IMAGE_SIZE);
	previewImage.setOrigin(PREVIEW_IMAGE_SIZE.x / 2.0f, PREVIEW_IMAGE_SIZE.y / 2.0f);

	return true;
}

void PlayerInterface::CreateBoxes(int maxBoxes)
{
	boxes.clear();

	for (int i = 0; i < maxBoxes; i++)
	{
		boxes.push_back(new sf::RectangleShape(BOX_SIZE));
	}

	for (auto box : boxes)
	{
		box->setTexture(&boxTexture);
	}
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

	bossBar.setPosition(cameraPos + BOSS_BAR_MARGIN);
	bossHPLine.setPosition(cameraPos + BOSS_BAR_MARGIN + BOSS_HP_LINE_MARGIN);

	blackFilter.setPosition(cameraPos);

	for (auto box : boxes)
	{
		box->setPosition(cameraPos + PLAYER_BAR_POS + PLAYER_BOXES_MARGIN);
	}
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
	case SWORD:
		playerWeaponBar.setTexture(&playerMeleeBarTexture);
		break;
	case SHOOTGUN:
		playerWeaponBar.setTexture(&playerShootgunBarTexture);
		break;
	case AK:
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

bool PlayerInterface::UpdatePreview(sf::Vector2f const& position, float elapsedTime)
{
	if (previewPartColdown >= PART_COLDOWN)
	{
		if (currentPart == PreviewStatus::MONSTERS)
		{
			return true;
		}

		previewPartColdown = 0;
		currentPart = PreviewStatus(static_cast<int>(currentPart) + 1);
	}

	previewPartColdown += elapsedTime;
	previewImage.setPosition({ position.x, position.y });
	previewText.setOrigin(previewText.getGlobalBounds().width / 2.0f, previewText.getGlobalBounds().height / 2.0f);
	previewText.setPosition(position + PREVIEW_TEXT_MARGIN );

	switch (currentPart)
	{
	case PreviewStatus::HOUSE:
		previewImage.setTexture(&previewTextures[0]);
		previewText.setString("There was one old house and wicked family lived in it...");
		break;
	case PreviewStatus::CELLAR:
		previewImage.setTexture(&previewTextures[1]);
		previewText.setString("They had a little child who once was locked in a scary cellar...");
		break;
	case PreviewStatus::BOX:
		previewImage.setTexture(&previewTextures[2]);
		previewText.setString("The only way out of this place was through the window by putting boxes on each other...");
		break;
	case PreviewStatus::MONSTERS:
		previewImage.setTexture(&previewTextures[3]);
		previewText.setString("But it turned out to be not so easy when his nightmares started overtaking him...");
		break;
	default:
		break;
	}

	UpdateHelpButton("Press \"ENTER\" to skip", position);

	return false;
}

void PlayerInterface::UpdateHelpButton(std::string const& helpStr, sf::Vector2f const& cameraPos)
{
	(void)helpStr;
	helpText.setString(helpStr);
	helpText.setPosition(cameraPos + HELP_TEXT_MARGIN);
}

void PlayerInterface::UpdatePlayerBoxes(int currentBoxes)
{
	(void)currentBoxes;

	float firstPosX = boxes[0]->getPosition().x;
	int boxNumber = 0;

	for (auto box : boxes)
	{
		if (boxNumber < currentBoxes)
		{
			box->setFillColor(Color(255, 255, 255, 255));
		}
		else
		{
			box->setFillColor(Color(255, 255, 255, 40));
		}
		box->setPosition({ firstPosX + boxNumber * BOXES_MIDLE_MARGIN, box->getPosition().y });
		boxNumber += 1;
	}
}

void PlayerInterface::UpdateBossBar(int bossMaxHealth, int bossHealth)
{
	float healthLineSize = static_cast<float>(bossHealth) / static_cast<float>(bossMaxHealth) * BOSS_HP_LINE_SIZE.x;
	bossHPLine.setSize({ healthLineSize , BOSS_HP_LINE_SIZE.y });
}

void PlayerInterface::UpdateWin(sf::Vector2f const& windowCenter)
{
	win.setPosition(windowCenter);
	winText.setPosition(windowCenter + PREVIEW_TEXT_MARGIN);
	UpdateHelpButton("Press \"ENTER\" to go main menu", windowCenter);
}

void PlayerInterface::Draw(RenderWindow& window)
{
	window.draw(blackFilter);
	window.draw(playerHealthBar);
	window.draw(playerHealth); 
	window.draw(playerWeaponBar);
	window.draw(playerAmmo);

	for (auto it = boxes.begin(); it != boxes.end(); it++)
	{
		sf::RectangleShape* box = *it;
		window.draw(*box);
	}
}

void PlayerInterface::DrawPart(sf::RenderWindow& window)
{
	window.clear(sf::Color::Black);
	window.draw(previewImage);
	window.draw(previewText);
	window.draw(helpText);
}

void PlayerInterface::DrawBossBar(sf::RenderWindow& window)
{
	window.draw(bossHPLine);
	window.draw(bossBar);
}

void PlayerInterface::DrawWin(sf::RenderWindow& window)
{
	window.clear(sf::Color::Black);
	window.draw(win);
	window.draw(winText);
	window.draw(helpText);
}

string IntToStr(int number)
{
	if (number >= 0)
	{
		return std::to_string(number);
	}
	else
	{
		return "0";
	}
}

sf::Vector2f GetTextureSize(sf::Texture const& texture)
{
	const sf::Vector2f size = {
		static_cast<float>(texture.getSize().x),
		static_cast<float>(texture.getSize().y)
	};

	return size;
}
