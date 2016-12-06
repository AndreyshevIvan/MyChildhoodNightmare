#include "game.h"

using namespace std;
using namespace sf;

bool Game::InitGame()
{
	if (
		!level_1.LoadFromFile("resources/firstTileset.tmx") &&
		!level_2.LoadFromFile("resources/secondTileset.tmx")
		)
	{
		return false;
	}
	if (
		!player.InitPlayer() ||
		!menu.InitMenuItems() ||
		!interface.Init()
		)
	{
		return false;
	}

	currentLevel = &level_1;

	mapTiles = level_1.GetAllObjects();
	mapSize = { level_1.GetTilemapWidth(), level_1.GetTilemapHeight() };
	camera.reset(sf::FloatRect(0, 0, RESOLUTION.x, RESOLUTION.y));

	difficult = Difficult::EASY;
	menu.Select(CurrentMenu::DIFFICULT, difficult);
	menu.SetMenu(CurrentMenu::START, camera.getCenter());

	return true;
}

void Game::StartGame()
{
	for (auto it = enemyShadows.begin(); it != enemyShadows.end();)
	{
		EnemyShadow* enemy = *it;
		it = enemyShadows.erase(it);
		delete(enemy);
	}

	player.Clear();
	player.InitPlayer();

	SpawnEntities();
	currentScene = &gameplayScene;
}

void Game::SpawnEntities()
{
	player.Spawn(currentLevel->GetObject("player_spawn"));

	auto enemiesSpawns = level_1.GetObjects("enemy_shadow_spawn");
	for (auto it = enemiesSpawns.begin(); it != enemiesSpawns.end(); it++)
	{
		enemyShadows.push_back(new EnemyShadow(*it));
	}
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
	if (
		Keyboard::isKeyPressed(Keyboard::Escape) &&
		menu.buttonsColdown >= BUTTONS_COLDOWN
		)
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
		if (Keyboard::isKeyPressed(Keyboard::Num1))
		{
			player.weapon = MELEE;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num2))
		{
			player.weapon = FIREBALL;
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
		if (
			IsCollidesWithLevel(bullet->collisionRect) ||
			!bullet->isLive
			)
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
	for (auto it = enemyShadows.begin(); it != enemyShadows.end(); it++)
	{
		EnemyShadow* enemy = *it;
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
		for (auto enemyIt = enemyShadows.begin(); enemyIt != enemyShadows.end(); enemyIt++)
		{
			EnemyShadow* enemy = *enemyIt;
			if (enemy->collisionRect.intersects(bullet->collisionRect))
			{
				enemy->health -= bullet->demage;
				bullet->isLive = false;
			}
		}
	}
}

void Game::UpdateEnemies()
{
	for (auto it = enemyShadows.begin(); it != enemyShadows.end();)
	{
		EnemyShadow* enemy = *it;
		if (enemy->health <= 0)
		{
			it = enemyShadows.erase(it);
			delete(enemy);
		}
		else
		{
			enemy->UpdatePos(elapsedTime, mapTiles);
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
	interface.UpdateBarsPos(camera.getCenter());
	interface.UpdatePlayerHP(player.health);
}

void Game::DrawLevel(sf::RenderWindow& window)
{
	level_1.Draw(window, GetCameraArea());
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
	for (auto it = enemyShadows.begin(); it != enemyShadows.end(); it++)
	{
		EnemyShadow* enemy = *it;
		if (GetCameraArea().intersects(enemy->collisionRect))
		{
			enemy->Draw(window);
		}
	}
}