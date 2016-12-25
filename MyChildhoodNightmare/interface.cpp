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
		!filterTexture.loadFromFile("resources/filter.png") ||
		!previewTextures[0].loadFromFile("resources/house.png") ||
		!previewTextures[1].loadFromFile("resources/cellar.png") ||
		!previewTextures[2].loadFromFile("resources/teaserBox.png") ||
		!previewTextures[3].loadFromFile("resources/notAlone.png") ||
		!boxTexture.loadFromFile("resources/box.png"))
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

	playerHealthBar.setSize(GetTextureSize(playerHealthBarTexture));
	playerHealthBar.setTexture(&playerHealthBarTexture);

	playerWeaponBar.setSize(PLAYER_WEAPON_BAR_SIZE);

	gameOver.setTexture(&gameOverTexture);
	gameOver.setSize(GetTextureSize(gameOverTexture));
	gameOver.setOrigin(gameOver.getSize().x / 2.0f , gameOver.getSize().y / 2.0f);

	filter.setSize(RESOLUTION);
	filter.setTexture(&filterTexture);
	filter.setOrigin(RESOLUTION.x / 2.0f, RESOLUTION.y / 2.0f);

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
		//box->setOrigin(BOX_SIZE.x / 2.0f, BOX_SIZE.y / 2.0f);
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

	filter.setPosition(cameraPos);

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
	previewText.setPosition({ position.x, position.y + PREVIEW_TEXT_MARGIN });

	switch (currentPart)
	{
	case PreviewStatus::HOUSE:
		previewImage.setTexture(&previewTextures[0]);
		previewText.setString("There was one old house and wicked family lived in it.");
		break;
	case PreviewStatus::CELLAR:
		previewImage.setTexture(&previewTextures[1]);
		previewText.setString("They had a little child who once was locked in a scary cellar... ");
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

	return false;
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

void PlayerInterface::Draw(RenderWindow& window)
{
	window.draw(filter);
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
