#include "game.h"

using namespace std;
using namespace sf;

bool Game::InitGame()
{
	if (!level_1.LoadFromFile("resources/firstTileset.tmx") ||
		!backgroundTexture_level_1.loadFromFile("resources/background_level_1.png"))
	{
		return false;
	}

	currentLevel = &level_1;

	if (!player.InitPlayer() || !menu.InitMenuItems() || !interface.Init())
	{
		return false;
	}

	mapTiles = level_1.GetAllObjects();
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

	return true;
}

void Game::StartGame()
{
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		Character* enemy = *it;
		it = enemies.erase(it);
		delete(enemy);
	}

	player.Clear();
	player.InitPlayer();

	SpawnEntities();
	currentScene = &gameplayScene;
}

void Game::SpawnEntities()
{
	std::vector<Object> shadowsSpawns = currentLevel->GetObjects("enemy_shadow_spawn");
	std::vector<Object> clownsSpawns = currentLevel->GetObjects("enemy_clown_spawn");

	for (auto it = shadowsSpawns.begin(); it != shadowsSpawns.end(); it++)
	{
		enemies.push_back(new Enemy(it->rect, EnemyType::SHADOW));
	}

	for (auto it = clownsSpawns.begin(); it != clownsSpawns.end(); it++)
	{
		enemies.push_back(new Enemy(it->rect, EnemyType::CLOWN));
	}

	player.Spawn(currentLevel->GetObject("player_spawn"));
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
	for (unsigned i = 0; i < mapTiles.size(); i++)
	{
		if (rect.intersects(mapTiles[i].rect) && mapTiles[i].name == "solid")
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
	else
	{
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			player.Jump();
		}
		if (
			Keyboard::isKeyPressed(Keyboard::A) ||
			Keyboard::isKeyPressed(Keyboard::Left)
			)
		{
			player.runStatus = RUN_LEFT;
			player.orientationStatus = LEFT;
		}
		if (
			Keyboard::isKeyPressed(Keyboard::D) ||
			Keyboard::isKeyPressed(Keyboard::Right)
			)
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
	if (
		Keyboard::isKeyPressed(Keyboard::Escape) &&
		menu.buttonsColdown >= BUTTONS_COLDOWN &&
		menu.currentMenu == CurrentMenu::PAUSE
		)
	{
		menu.buttonsColdown = 0;
		currentScene = &gameplayScene;
	}
	else
	{
		if (
			Keyboard::isKeyPressed(Keyboard::F) &&
			menu.buttonsColdown >= BUTTONS_COLDOWN
			)
		{
			menu.buttonsColdown = 0;
			ControlMenuLogic(window);
		}
		else
		{
			if (
				Keyboard::isKeyPressed(Keyboard::Up) &&
				menu.buttonsColdown >= BUTTONS_COLDOWN
				)
			{
				menu.SwitchButtonUp();
			}
			else if (
				Keyboard::isKeyPressed(Keyboard::Down) &&
				menu.buttonsColdown >= BUTTONS_COLDOWN
				)
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
			StartGame();
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
			StartGame();
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

void Game::UpdatePlayer()
{
	player.UpdatePos(elapsedTime, mapTiles);
	player.UpdateHealthStatus();
	player.UpdateStatuses();
}

void Game::UpdateBullets()
{
	for (auto it = player.bullets.begin(); it != player.bullets.end();)
	{
		Bullet* bullet = *it;
		bullet->Update(elapsedTime);
		if (IsCollidesWithLevel(bullet->collisionRect) || !bullet->isLive)
		{
			it = player.bullets.erase(it);
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
	for (auto it = enemies.begin(); it != enemies.end(); it++)
	{
		Character* enemy = *it;
		if (enemy->collisionRect.intersects(player.collisionRect))
		{
			if (player.injuredColdown >= INJURED_COLDOWN)
			{
				player.health -= enemy->demage;
				player.injuredColdown = 0;
			}
		}
	}

	for (auto bullIt = player.bullets.begin(); bullIt != player.bullets.end(); bullIt++)
	{
		Bullet* bullet = *bullIt;
		for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); enemyIt++)
		{
			Character* enemy = *enemyIt;
			if (enemy->collisionRect.intersects(bullet->collisionRect))
			{
				enemy->health -= bullet->demage;
				if (enemy->health <= 0)
				{
					player.points += POINTS_FOR_KILL_SHADOW;
				}

				bullet->isLive = false;
			}
		}
	}
}

void Game::UpdateEnemies()
{
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		Enemy* enemy = *it;
		if (enemy->health <= 0)
		{
			it = enemies.erase(it);
			delete(enemy);
		}
		else
		{
			enemy->UpdatePos(elapsedTime, mapTiles);
			enemy->UpdateAI(player, *currentLevel);
			++it;
		}
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
		cameraCenter = camera.getCenter();
	}

	window.setView(camera);
}

void Game::UpdateInterface()
{
	int weaponId = int(player.currentWeapon);

	interface.UpdateBarsPos(camera.getCenter());
	interface.UpdatePlayerHP(player.health);
	interface.UpdatePlayerWeapon(weaponId, player.ammo[weaponId]);
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

void Game::DrawBullets(sf::RenderWindow& window)
{
	for (auto it = player.bullets.begin(); it != player.bullets.end(); it++)
	{
		Bullet* bullet = *it;
		if (GetCameraArea().intersects(bullet->collisionRect))
		{
			window.draw(bullet->bodyShape);
		}
	}
}

void Game::DrawEnemies(sf::RenderWindow& window)
{
	for (auto it = enemies.begin(); it != enemies.end(); it++)
	{
		Character* enemy = *it;
		enemy->Draw(window);
	}
}