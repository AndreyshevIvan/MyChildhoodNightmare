#include "interface.h"

using namespace std;
using namespace sf;

const sf::Vector2f RESOLUTION = { 1366 , 768 };

enum
{
	// Weapon type
	PISTOL = 0,
	SHOOTGUN = 1,
	AK = 2,
	// Ammo count
	INFINITY_AMMO = INT_MAX,
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
	playerWeaponBar.setSize(WEAPON_BAR_SIZE);

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

void PlayerInterface::CreateBoxes(std::map<Level*, int> const& boxesMap, Level* level)
{
	const int maxBoxes = boxesMap.find(level)->second;
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

void PlayerInterface::CreateAnnouncement(sf::Vector2f const& position, std::string const& str)
{
	auto marginX = static_cast<float>(rand() % MAX_ANNOUNCEMENT_MARGIN + 1);
	auto marginY = static_cast<float>(rand() % MAX_ANNOUNCEMENT_MARGIN + 1);

	sf::Vector2f margin(marginX, marginY / 2.0f);

	sf::Text announcement = Text(str, font, ANNOUNCEMENT_FONT_SIZE);
	announcement.setOutlineThickness(ANNOUNCEMENT_OUTLINE_THICKNESS);
	announcement.setOutlineColor(sf::Color::Black);
	announcement.setOrigin(announcement.getGlobalBounds().width / 2.0f, announcement.getGlobalBounds().height / 2.0f);
	announcement.setPosition(position + margin);

	demageAnnouncementText.push_back(announcement);
	demageAnnouncementDuration.push_back(0);
}

void PlayerInterface::CreateRemark(RemarkType const& type)
{
	if (remark == nullptr)
	{
		const float probability = remarkSystem.find(type)->second->second;

		if (rand() % PROBABILITY_SIZE < static_cast<int>(probability * PROBABILITY_SIZE))
		{
			const size_t remarksCount = remarkSystem.find(type)->second->first.size();
			const int randomRemarkNumber = rand() % remarksCount;
			const std::string text = remarkSystem.find(type)->second->first[randomRemarkNumber];
			remark = new Remark(text);
		}
	}
}

void PlayerInterface::UpdateRemark(float elapsedTime, sf::Vector2f const& position)
{
	if (remark != nullptr)
	{
		if (remark->duration >= REMARK_DURATION)
		{
			delete remark;
			remark = nullptr;
		}
		else
		{
			remark->Update(position, elapsedTime);
		}
	}
}

void PlayerInterface::UpdateBarsPos(Vector2f const& cameraPos)
{
	sf::Vector2f playerHealthPos = cameraPos + PLAYER_BAR_POS;
	sf::Vector2f playerWeaponBarPos = playerHealthPos + PLAYER_WEAPON_MARGIN;

	playerHealthBar.setPosition(playerHealthPos);
	playerWeaponBar.setPosition(playerWeaponBarPos);

	playerHealth.setPosition(playerHealthPos + PLAYER_HP_MARGIN);

	playerAmmo.setOrigin(playerAmmo.getGlobalBounds().width / 2.0f, playerAmmo.getGlobalBounds().height / 2.0f );
	playerAmmo.setPosition(playerHealthPos + PLAYER_AMMO_MARGIN);

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
	case PISTOL:
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

	if (ammo >= 0 && ammo != INFINITY_AMMO)
	{
		string ammoStr = IntToStr(ammo);
		playerAmmo.setString(ammoStr);
	}
	else
	{
		playerAmmo.setString("");
	}
}

bool PlayerInterface::UpdatePreview(sf::Vector2f const& position, float elapsedTime)
{
	if (partDuration >= PART_DURATION)
	{
		if (currentPart == PreviewStatus::MONSTERS)
		{
			return true;
		}

		partDuration = 0;
		currentPart = PreviewStatus(static_cast<int>(currentPart) + 1);
		previewText.setFillColor(sf::Color::White);
		previewImage.setFillColor(sf::Color::White);
	}

	partDuration += elapsedTime;
	previewImage.setPosition({ position.x, position.y });
	previewText.setOrigin(previewText.getGlobalBounds().width / 2.0f, previewText.getGlobalBounds().height / 2.0f);
	previewText.setPosition(position + PREVIEW_TEXT_MARGIN );

	const sf::Color color = previewImage.getFillColor();
	previewImage.setFillColor(Extinguish(elapsedTime, color, partDuration, PART_DURATION));
	previewText.setFillColor(Extinguish(elapsedTime, color, partDuration, PART_DURATION));

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
	helpText.setString(helpStr);
	helpText.setPosition(cameraPos + HELP_TEXT_MARGIN);
}

void PlayerInterface::UpdatePlayerBoxes(int currentBoxes)
{
	if (!boxes.empty())
	{
		float firstPosX = boxes.front()->getPosition().x;
		int boxNumber = 0;

		for (auto box : boxes)
		{
			if (boxNumber < currentBoxes)
			{
				box->setFillColor(sf::Color::White);
			}
			else
			{
				box->setFillColor(NOT_FOUND_BOX_COLOR);
			}
			box->setPosition({ firstPosX + boxNumber * BOXES_MIDLE_MARGIN, box->getPosition().y });
			boxNumber++;
		}
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

void PlayerInterface::UpdateAnnouncement(float elapsedTime)
{
	for (size_t elementNumber = 0; elementNumber < demageAnnouncementDuration.size();)
	{
		float* duration = &demageAnnouncementDuration[elementNumber];
		sf::Text* text = &demageAnnouncementText[elementNumber];

		if (*duration >= ANNOUNCEMENT_DURATION)
		{
			demageAnnouncementDuration.erase(demageAnnouncementDuration.begin() + elementNumber);
			demageAnnouncementText.erase(demageAnnouncementText.begin() + elementNumber);
		}
		else
		{
			text->move(0, -DEMAGE_ANNOUNCEMENT_SPEED * elapsedTime);

			const sf::Color color = text->getFillColor();
			sf::Color newColor(Extinguish(elapsedTime, color, *duration, ANNOUNCEMENT_DURATION));
			text->setFillColor(newColor);
			text->setOutlineColor(sf::Color(0, 0, 0, newColor.a));

			*duration += elapsedTime;
			++elementNumber;
		}
	}
}

void PlayerInterface::Draw(RenderWindow& window)
{
	window.draw(blackFilter);
	window.draw(playerHealthBar);
	window.draw(playerHealth); 
	window.draw(playerWeaponBar);
	window.draw(playerAmmo);
	DrawAnnouncement(window);

	if (remark != nullptr)
	{
		remark->Draw(window);
	}

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

void PlayerInterface::DrawAnnouncement(sf::RenderWindow& window)
{
	for (auto announcement : demageAnnouncementText)
	{
		window.draw(announcement);
	}
}

Remark::Remark(std::string const& text)
	:remark(text)
{
	cloudTexture.loadFromFile("resources/cloud.png");
	font.loadFromFile("resources/nightmarealley.ttf");

	cloud.setTexture(&cloudTexture);
	cloud.setSize(REMARK_CLOUD_SIZE);

	remarkText = sf::Text(remark, font, HELP_FONT_SIZE);
	remarkText.setOrigin(remarkText.getGlobalBounds().width / 2.0f, remarkText.getGlobalBounds().height / 2.0f);
	remarkText.setFillColor(sf::Color::Black);
}

void Remark::Update(sf::Vector2f const& position, float elapsedTime)
{
	cloud.setPosition(position + REMARK_MARGIN);
	remarkText.setPosition(cloud.getPosition() + REMARK_TEXT_MARGIN);
	duration += elapsedTime;

	cloud.setFillColor(Extinguish(elapsedTime, cloud.getFillColor(), duration, REMARK_DURATION));
	remarkText.setFillColor(Extinguish(elapsedTime, remarkText.getFillColor(), duration, REMARK_DURATION));
}

void Remark::Draw(sf::RenderWindow& window)
{
	window.draw(cloud);
	window.draw(remarkText);
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

sf::Color Extinguish(
	float elapsedTime,
	sf::Color const& color,
	float duration,
	float maxDuration,
	float transExtinguishDur
)
{
	float avalableDuration = maxDuration - transExtinguishDur;
	//std::cout << transExtinguishDur;
	sf::Color newColor;
	if (duration >= avalableDuration)
	{
		sf::Uint8 transparency = color.a;
		sf::Uint8 step = static_cast<sf::Uint8>(255 * elapsedTime / transExtinguishDur);
		newColor = sf::Color(255, 255, 255, transparency - step);
		std::cout << static_cast<int>(transparency - step) << "\n";

		return newColor;
	}

	return color;
}