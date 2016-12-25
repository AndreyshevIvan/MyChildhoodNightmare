#include "game.h"

using namespace std;
using namespace sf;

bool Game::InitGame()
{
	if (!level_1.LoadFromFile("resources/firstTileset.tmx") ||
		!level_2.LoadFromFile("resources/secondTileset.tmx") ||
		!backgroundTexture_level_1.loadFromFile("resources/background_level_1.png"))
	{
		return false;
	}

	currentLevel = &level_1;

	if (!player.InitPlayer() || !menu.InitMenuItems() || !interface.Init())
	{
		return false;
	}

	mapSize = { level_1.GetTilemapWidth(), level_1.GetTilemapHeight() };
	camera.reset(sf::FloatRect(0, 0, RESOLUTION.x, RESOLUTION.y));

	difficult = Difficult::EASY;
	menu.Select(CurrentMenu::DIFFICULT, difficult);
	menu.SetMenu(CurrentMenu::START, camera.getCenter());

	background_level_1.setTexture(&backgroundTexture_level_1);
	const sf::Vector2f BACKGROUND_LVL_1_SIZE = {
		static_cast<float>(backgroundTexture_level_1.getSize().x),
		static_cast<float>(backgroundTexture_level_1.getSize().y)
	};
	background_level_1.setSize(BACKGROUND_LVL_1_SIZE);
	background_level_1.setOrigin(
		background_level_1.getGlobalBounds().width / 2.0f,
		background_level_1.getGlobalBounds().height / 2.0f
	);

	buttonColdown = 0;

	boxesCoundMap = {
		{ &level_1, 3 },
		{ &level_2, 3 },
		{ &level_3, 1 }
	};

	return true;
}

void Game::StartGame(Level& level)
{
	ClearScene();

	currentLevel = &level;

	interface.CreateBoxes(boxesCoundMap.find(currentLevel)->second);

	blocks = currentLevel->GetObjects("solid");
	lava = currentLevel->GetObjects("lava");

	player.Clear();
	player.InitPlayer();

	SpawnEntities();

	currentScene = &gameplayScene;
}

void Game::CheckCompletedLevel()
{
	bool IsChange = false;
	auto door_level_2 = currentLevel->GetObject("level2").rect;
	auto door_level_3 = currentLevel->GetObject("level3").rect;
	auto playerRect = player.collisionRect;

	int currBoxes = player.boxes;
	int neededBoxes = boxesCoundMap.find(currentLevel)->second;

	if (playerRect.intersects(door_level_2) && currBoxes == neededBoxes)
	{
		IsChange = true;
		StartGame(level_2);
	}
	else if (playerRect.intersects(door_level_3) && currBoxes == neededBoxes)
	{
		IsChange = true;
		StartGame(level_3);
	}
}

void Game::ClearScene()
{
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		Character* enemy = *it;
		it = enemies.erase(it);
		delete(enemy);
	}

	for (auto it = bonuses.begin(); it != bonuses.end();)
	{
		Bonus* bonus = *it;
		it = bonuses.erase(it);
		delete(bonus);
	}
}

void Game::SpawnEntities()
{
	std::vector<Object> shadowsSpawns = currentLevel->GetObjects("enemy_shadow_spawn");
	std::vector<Object> clownsSpawns = currentLevel->GetObjects("enemy_clown_spawn");
	std::vector<Object> birdsSpawns = currentLevel->GetObjects("enemy_bird_spawn");
	std::vector<Object> bonusesSpawns = currentLevel->GetObjects("bonus_spawn");
	std::vector<Object> itemsBoxSpawns = currentLevel->GetObjects("item_box_spawn");

	for (auto const& shadowSpawn : shadowsSpawns)
	{
		sf::Vector2f pos = { shadowSpawn.rect.left, shadowSpawn.rect.top };
		enemies.push_back(new Enemy(pos, EnemyType::SHADOW));
	}

	for (auto const& clownSpawn : clownsSpawns)
	{
		sf::Vector2f pos = { clownSpawn.rect.left, clownSpawn.rect.top };
		enemies.push_back(new Enemy(pos, EnemyType::CLOWN));
	}

	for (auto const& birdSpawn : birdsSpawns)
	{
		sf::Vector2f pos = { birdSpawn.rect.left, birdSpawn.rect.top };
		enemies.push_back(new Enemy(pos, EnemyType::BIRD));
	}

	for (auto const& bonusSpawn : bonusesSpawns)
	{
		sf::Vector2f bonusPos = { bonusSpawn.rect.left, bonusSpawn.rect.top };
		bonuses.push_back(new Bonus(bonusPos));
	}

	for (auto const& itemsSpawn : itemsBoxSpawns)
	{
		sf::Vector2f itemPos = { itemsSpawn.rect.left, itemsSpawn.rect.top };
		bonuses.push_back(new Bonus(itemPos, Items::BOX));
	}

	sf::FloatRect posRect = currentLevel->GetObject("player_spawn").rect;
	player.Spawn({ posRect.left, posRect.top });
}

void Game::SetElapsedTime()
{
	elapsedTime = clock.getElapsedTime().asSeconds();
	clock.restart();
}

sf::FloatRect Game::GetCameraArea()
{
	auto camCenter = camera.getCenter();
	sf::FloatRect cameraArea(
		camCenter.x - RESOLUTION.x / 2.0f,
		camCenter.y - RESOLUTION.y / 2.0f, RESOLUTION.x, RESOLUTION.y
	);
	
	return cameraArea;
}

bool Game::IsCollidesWithLevel(sf::FloatRect const& rect)
{
	for (unsigned i = 0; i < blocks.size(); i++)
	{
		if (rect.intersects(blocks[i].rect) && blocks[i].name == "solid")
		{
			return true;
		}
	}
	return false;
}

void Game::ControlPlayer()
{
	if (Keyboard::isKeyPressed(Keyboard::Escape) && menu.buttonsColdown >= BUTTONS_COLDOWN)
	{
		menu.SetMenu(CurrentMenu::PAUSE, camera.getCenter());
		currentScene = &menuScene;
	}
	else if (player.existStatus != ExistenceStatus::DEAD)
	{
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			player.Jump();
		}
		if (Keyboard::isKeyPressed(Keyboard::A) ||
			Keyboard::isKeyPressed(Keyboard::Left))
		{
			player.runStatus = RUN_LEFT;
			player.orientationStatus = LEFT;
		}
		if (Keyboard::isKeyPressed(Keyboard::D) ||
			Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.runStatus = RUN_RIGHT;
			player.orientationStatus = RIGHT;
		}
		if (Keyboard::isKeyPressed(Keyboard::O) && buttonColdown >= BUTTONS_COLDOWN)
		{
			buttonColdown = 0;
			player.SwitchWeapon();
		}
		if (Keyboard::isKeyPressed(Keyboard::P))
		{
			player.Attack();
		}
	}
}

void Game::ControlMenu(sf::RenderWindow& window)
{
	if (Keyboard::isKeyPressed(Keyboard::Escape) &&
		menu.buttonsColdown >= BUTTONS_COLDOWN &&
		menu.currentMenu == CurrentMenu::PAUSE)
	{
		menu.buttonsColdown = 0;
		currentScene = &gameplayScene;
	}
	else
	{
		if (Keyboard::isKeyPressed(Keyboard::F) &&
			menu.buttonsColdown >= BUTTONS_COLDOWN)
		{
			menu.buttonsColdown = 0;
			ControlMenuLogic(window);
		}
		else
		{
			if (Keyboard::isKeyPressed(Keyboard::Up) &&
				menu.buttonsColdown >= BUTTONS_COLDOWN)
			{
				menu.SwitchButtonUp();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down) &&
				menu.buttonsColdown >= BUTTONS_COLDOWN)
			{
				menu.SwitchButtonDown();
			}
		}
	}
}

void Game::ControlMenuLogic(sf::RenderWindow& window)
{
	switch (menu.currentMenu)
	{
	case CurrentMenu::START:

		switch (menu.currentButton)
		{
		case 0:
			StartGame(level_1);
			break;
		case 1:
			menu.SetMenu(CurrentMenu::DIFFICULT, camera.getCenter());
			break;
		case 2:	
			window.close();
			break;
		default:
			break;
		}
		menu.currentButton = 0;
		break;
	case CurrentMenu::DIFFICULT:

		switch (menu.currentButton)
		{
		case 0:
			difficult = Difficult::EASY;
			break;
		case 1:
			difficult = Difficult::NORMAL;
			break;
		case 2:
			difficult = Difficult::HARD;
			break;
		case 3:
			menu.SetMenu(CurrentMenu::START, camera.getCenter());
			break;
		default:
			break;
		}
		menu.Select(CurrentMenu::DIFFICULT, difficult);
		break;

	case CurrentMenu::PAUSE:

		switch (menu.currentButton)
		{
		case 0:
			currentScene = &gameplayScene;
			break;
		case 1:
			StartGame(*currentLevel);
			break;
		case 2:
			menu.SetMenu(CurrentMenu::START, camera.getCenter());
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void Game::ControlGameOver(sf::RenderWindow& window)
{
	(void)window;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		StartGame(*currentLevel);
	}
	if (sf::Keyboard::isKeyPressed(Keyboard::Escape))
	{
		menu.SetMenu(CurrentMenu::START, camera.getCenter());
		currentScene = &menuScene;
	}
}

void Game::UpdatePlayer()
{
	player.UpdatePos(elapsedTime, blocks);
	player.UpdateHealthStatus();
	player.UpdateStatuses();
	player.UpdateTexture();

	if (player.existStatus == ExistenceStatus::DEAD)
	{
		auto bodyRotation = player.bodyShape.getRotation();
		player.bodyShape.setRotation(bodyRotation + DEAD_ROTATION * elapsedTime / GAME_OVER_COLDOWN);

		if (gameOverColdown >= GAME_OVER_COLDOWN)
		{
			currentScene = &gameOverScene;
		}
	}

	CheckCompletedLevel();
}

void Game::UpdateBullets()
{
	for (auto it = player.characterBullets.begin(); it != player.characterBullets.end();)
	{
		Bullet* bullet = *it;
		bullet->Update(elapsedTime);
		if (IsCollidesWithLevel(bullet->collisionRect) || !bullet->isLive)
		{
			it = player.characterBullets.erase(it);
			delete(bullet);
		}
		else
		{
			++it;
		}
	}

	for (auto it = enemyBullets.begin(); it != enemyBullets.end();)
	{
		Bullet* bullet = *it;
		bullet->Update(elapsedTime);
		if (IsCollidesWithLevel(bullet->collisionRect) || !bullet->isLive)
		{
			it = enemyBullets.erase(it);
			delete(bullet);
		}
		else
		{
			++it;
		}
	}
}

void Game::CheckEntitiesCollides()
{
	EnemyPlayerCollides();
	PlayerBulletsEnemyCollides();
	EnemyBulletsPlayerCollides();
	BonusesPlayerCollides();
	PlayerLavaCollides();
}

void Game::EnemyBulletsPlayerCollides()
{
	for (auto bullet : enemyBullets)
	{
		if (player.collisionRect.intersects(bullet->collisionRect))
		{
			if (player.injuredColdown >= INJURED_COLDOWN)
			{
				player.health -= bullet->demage;
				bullet->isLive = false;
			}
		}
	}
}

void Game::PlayerLavaCollides()
{
	for (auto lavaBlock : this->lava)
	{
		if (lavaBlock.rect.intersects(player.collisionRect))
		{
			if (player.injuredColdown >= INJURED_COLDOWN)
			{
				player.health -= LAVA_DEMAGE;
				player.injuredColdown = 0;
			}
		}
	}
}

void Game::BonusesPlayerCollides()
{
	for (auto it = bonuses.begin(); it != bonuses.end();)
	{
		Bonus* bonus = *it;
		if (bonus->collisionRect.intersects(player.collisionRect) && player.AddBonusEffect(*bonus))
		{
			it = bonuses.erase(it);
			delete(bonus);
		}
		else
		{
			++it;
		}
	}
}

void Game::PlayerBulletsEnemyCollides()
{
	for (auto bullet : player.characterBullets)
	{
		for (auto enemy : enemies)
		{
			if (enemy->collisionRect.intersects(bullet->collisionRect))
			{
				enemy->health -= bullet->demage;
				enemy->activityStatus = EnemyActivity::PURSUIT;
				bullet->isLive = false;
			}
		}
	}
}

void Game::EnemyPlayerCollides()
{
	for (auto enemy : enemies)
	{
		if (enemy->collisionRect.intersects(player.collisionRect))
		{
			if (player.injuredColdown >= INJURED_COLDOWN)
			{
				player.health -= enemy->demage;
				player.injuredColdown = 0;
			}
		}
	}
}

void Game::UpdateEnemies()
{
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		Enemy* enemy = *it;
		enemy->UpdateAI(elapsedTime, player, blocks, enemyBullets);
		if (enemy->existStatus != ExistenceStatus::LIVE)
		{
			CreateBonus(enemy->GetCharacterPos(), bonuses);
			it = enemies.erase(it);
			delete(enemy);
		}
		else
		{
			++it;
		}
	}
}

void Game::UpdateBonuses()
{
	for (auto bonus : bonuses)
	{
		bonus->Update(elapsedTime, blocks);
	}
}

void Game::UpdateColdowns()
{
	if (player.shootColdown <= MAX_WEAPON_COLDOWN)
	{
		player.shootColdown += elapsedTime;
	}
	if (menu.buttonsColdown <= BUTTONS_COLDOWN)
	{
		menu.buttonsColdown += elapsedTime;
	}
	if (player.injuredColdown <= INJURED_COLDOWN)
	{
		player.injuredColdown += elapsedTime;
	}
	if (buttonColdown <= BUTTONS_COLDOWN)
	{
		buttonColdown += elapsedTime;
	}
	if (player.existStatus == ExistenceStatus::DEAD)
	{
		gameOverColdown += elapsedTime;
	}

	for (auto enemy : enemies)
	{
		if (enemy->shootColdown <= CLOWN_SHOOT_COLDOWN)
		{
			enemy->shootColdown += elapsedTime;
		}
	}
}

void Game::UpdateCamera(sf::RenderWindow& window)
{
	sf::Vector2f halfWindow = { RESOLUTION.x / 2.0f , RESOLUTION.y / 2.0f };
	sf::Vector2f cameraCenter = {
		player.GetCharacterPos().x,
		player.GetCharacterPos().y - CAMERA_VERTICAL_MARGIN
	};
	camera.setCenter(cameraCenter);

	if (cameraCenter.x - halfWindow.x < 0)
	{
		camera.setCenter(halfWindow.x, cameraCenter.y);
		cameraCenter = camera.getCenter();
	}
	if (cameraCenter.x + halfWindow.x > mapSize.x)
	{
		camera.setCenter(mapSize.x - halfWindow.x, cameraCenter.y);
		cameraCenter = camera.getCenter();
	}
	if (cameraCenter.y - halfWindow.y < 0)
	{
		camera.setCenter(cameraCenter.x, halfWindow.y);
		cameraCenter = camera.getCenter();
	}
	if (cameraCenter.y + halfWindow.y > mapSize.y)
	{
		camera.setCenter(cameraCenter.x, mapSize.y - halfWindow.y);
	}

	window.setView(camera);
}

void Game::UpdateInterface()
{
	int weaponId = int(player.currentWeapon);

	interface.UpdateBarsPos(camera.getCenter());
	interface.UpdatePlayerHP(player.health);
	interface.UpdatePlayerWeapon(weaponId, player.ammo[weaponId]);
	interface.UpdatePlayerBoxes(player.boxes);
}

void Game::UpdateBackground()
{
	float bgPosX_Percent = (camera.getCenter().x - RESOLUTION.x / 2.0f) / (currentLevel->GetTilemapWidth() - RESOLUTION.x);
	float bgPosY_Percent = (camera.getCenter().y - RESOLUTION.y / 2.0f) / (currentLevel->GetTilemapHeight() - RESOLUTION.y);

	float bgAllowedWidthX = currentLevel->GetTilemapWidth() - background_level_1.getSize().x;
	float bgAllowedWidthY = currentLevel->GetTilemapHeight() - background_level_1.getSize().y;

	float bgPosX_Pixel = background_level_1.getSize().x / 2.0f + bgPosX_Percent * bgAllowedWidthX;
	float bgPosY_Pixel = background_level_1.getSize().y / 2.0f + bgPosY_Percent * bgAllowedWidthY;

	background_level_1.setPosition(bgPosX_Pixel, bgPosY_Pixel);
}

void Game::DrawLevel(sf::RenderWindow& window)
{
	currentLevel->Draw(window, GetCameraArea());
}

void Game::DrawBullets(sf::RenderWindow& window) // player bullets generally in window
{
	for (auto const& playerBullet : player.characterBullets)
	{
		window.draw(playerBullet->bodyShape);
	}

	for (auto const& bullet : enemyBullets)
	{
		if (GetCameraArea().intersects(bullet->collisionRect))
		{
			window.draw(bullet->bodyShape);
		}
	}
}

void Game::DrawEnemies(sf::RenderWindow& window)
{
	for (auto const& enemy : enemies)
	{
		if (GetCameraArea().intersects(enemy->collisionRect))
		{
			enemy->DrawCharacter(window);
		}
	}
}

void Game::DrawBonuses(sf::RenderWindow& window)
{
	for (auto const& bonus : bonuses)
	{
		if (GetCameraArea().intersects(bonus->collisionRect))
		{
			bonus->DrawBonus(window);
		}
	}
}