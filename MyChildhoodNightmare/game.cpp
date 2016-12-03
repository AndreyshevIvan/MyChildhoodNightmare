#include "game.h"

using namespace std;
using namespace sf;

bool Game::InitGame()
{
	if (!level.LoadFromFile("resources/firstTileset.tmx"))
	{
		return false;
	}
	if (!player.InitPlayer(level.GetObject("player_spawn")) || !menu.InitMenuItems() || !interface.Init())
	{
		return false;
	}

	auto enemiesSpawns = level.GetObjects("enemy_shadow_spawn");
	for (auto posIt = enemiesSpawns.begin(); posIt != enemiesSpawns.end(); posIt++)
	{
		enemyShadows.push_back(new EnemyShadow(*posIt));
	}

	mapTiles = level.GetAllObjects();
	mapSize = { level.GetTilemapWidth(), level.GetTilemapHeight() };
	camera.reset(sf::FloatRect(0, 0, RESOLUTION.x, RESOLUTION.y));

	difficult = Difficult::EASY;
	menu.Select(CurrentMenu::DIFFICULT, difficult);
	menu.SetMenu(CurrentMenu::START, camera.getCenter());

	return true;
}

void Game::StartGame()
{

}

void Game::SetElapsedTime()
{
	elapsedTime = clock.getElapsedTime().asSeconds();
	clock.restart();
}

bool Game::IsCollidesWithLevel(sf::FloatRect const& rect)
{
	for (unsigned int i = 0; i < mapTiles.size(); i++)
	{
		if (rect.intersects(mapTiles[i].rect) && mapTiles[i].name == "solid")
		{
			return true;
		}
	}
	return false;
}

void Game::ControlPlayer(sf::Event& event)
{
	event;
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
		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
		{
			player.runStatus = RUN_LEFT;
			player.orientationStatus = LEFT;
		}
		if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
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

void Game::ControlMenu(sf::RenderWindow& window, sf::Event& event)
{

	if (Keyboard::isKeyPressed(Keyboard::Escape) && menu.buttonsColdown >= BUTTONS_COLDOWN && menu.currentMenu == CurrentMenu::PAUSE)
	{
		menu.buttonsColdown = 0;
		currentScene = &gameplayScene;
	}
	else
	{
		if (Keyboard::isKeyPressed(Keyboard::F) && menu.buttonsColdown >= BUTTONS_COLDOWN)
		{
			menu.buttonsColdown = 0;
			ControlMenuLogic(window, event);
		}
		else
		{
			if (Keyboard::isKeyPressed(Keyboard::Up) && menu.buttonsColdown >= BUTTONS_COLDOWN)
			{
				menu.SwitchButtonUp();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down) && menu.buttonsColdown >= BUTTONS_COLDOWN)
			{
				menu.SwitchButtonDown();
			}
		}
	}
}

void Game::ControlMenuLogic(sf::RenderWindow& window, sf::Event& event)
{
	event;
	switch (menu.currentMenu)
	{
	case CurrentMenu::START:

		switch (menu.currentButton)
		{
		case 0:
			currentScene = &gameplayScene;
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
			break;
		case 2:
			menu.SetMenu(CurrentMenu::START, camera.getCenter());
			StartGame();
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
	CheckCollidesWithEnemy();
	player.CheckHealth();
	player.UpdateStatuses();
	UpdatePlayerBullets();
}

void Game::UpdatePlayerBullets()
{
	bool isBulletDeleted = false;

	for (auto bulletsIt = player.bullets.begin(); bulletsIt != player.bullets.end();)
	{
		Bullet* bullet = *bulletsIt;
		bullet->Update(elapsedTime);
		if (IsCollidesWithLevel(bullet->collisionRect))
		{
			bulletsIt = player.bullets.erase(bulletsIt);
			delete(bullet);
		}
		else
		{
			for (auto enemiesIt = enemyShadows.begin(); enemiesIt != enemyShadows.end();++enemiesIt)
			{
				EnemyShadow* enemy = *enemiesIt;
				if (enemy->collisionRect.intersects(bullet->collisionRect))
				{
					enemy->health -= bullet->demage;
					bulletsIt = player.bullets.erase(bulletsIt);
					delete(bullet);
					isBulletDeleted = true;
				}
			}
			if (!isBulletDeleted)
			{
				++bulletsIt;
			}
			isBulletDeleted = false;
		}
	}
}

void Game::CheckCollidesWithEnemy()
{
	for (auto enemyIt = enemyShadows.begin(); enemyIt != enemyShadows.end(); enemyIt++)
	{
		EnemyShadow* enemy = *enemyIt;
		if (enemy->collisionRect.intersects(player.collisionRect))
		{
			if (player.enjuredColdown >= INJURED_COLDOWN)
			{
				player.health -= enemy->demage;
				player.enjuredColdown = 0;
			}
		}
	}
}

void Game::UpdateEnemies()
{
	for (auto enemyIt = enemyShadows.begin(); enemyIt != enemyShadows.end();)
	{
		EnemyShadow* enemy = *enemyIt;
		if (enemy->health <= 0)
		{
			enemyIt = enemyShadows.erase(enemyIt);
			delete(enemy);
		}
		else
		{
			enemy->UpdatePos(elapsedTime, mapTiles);
			++enemyIt;
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
	if (player.enjuredColdown <= INJURED_COLDOWN)
	{
		player.enjuredColdown += elapsedTime;
	}
}

void Game::UpdateCamera(sf::RenderWindow& window)
{
	sf::Vector2f cameraCenter = { player.GetCharacterPos().x, player.GetCharacterPos().y - CAMERA_VERTICAL_MARGIN };
	camera.setCenter(cameraCenter);

	if (cameraCenter.x - RESOLUTION.x / 2.0f < 0)
	{
		camera.setCenter(RESOLUTION.x / 2.0f , cameraCenter.y);
		cameraCenter = camera.getCenter();
	}
	if (cameraCenter.x + RESOLUTION.x / 2.0f > mapSize.x)
	{
		camera.setCenter(mapSize.x - RESOLUTION.x / 2.0f, cameraCenter.y);
		cameraCenter = camera.getCenter();
	}
	if (cameraCenter.y - RESOLUTION.y / 2.0f < 0)
	{
		camera.setCenter(cameraCenter.x, RESOLUTION.y / 2.0f);
		cameraCenter = camera.getCenter();
	}
	if (cameraCenter.y + RESOLUTION.y / 2.0f > mapSize.y)
	{
		camera.setCenter(cameraCenter.x, mapSize.y - RESOLUTION.y / 2.0f);
		cameraCenter = camera.getCenter();
	}

	window.setView(camera);
}

void Game::UpdatePlayerInterface()
{
	interface.UpdateBarsPos(camera.getCenter());
	interface.UpdatePlayerHP(player.health);
}

void Game::DrawLevel(sf::RenderWindow& window)
{
	auto camCenter = camera.getCenter();
	sf::FloatRect cameraArea(camCenter.x - RESOLUTION.x / 2.0f, camCenter.y - RESOLUTION.y / 2.0f, RESOLUTION.x, RESOLUTION.y);

	level.Draw(window, cameraArea);
}

void Game::DrawPlayerBullets(sf::RenderWindow& window)
{
	for (auto bulletsIter = player.bullets.begin(); bulletsIter != player.bullets.end();)
	{
		Bullet* bullet = *bulletsIter;
		window.draw(bullet->bodyShape);
		bulletsIter++;
	}
}

void Game::DrawEnemies(sf::RenderWindow& window)
{
	for (auto enemyIt = enemyShadows.begin(); enemyIt != enemyShadows.end(); enemyIt++)
	{
		EnemyShadow* enemy = *enemyIt;
		enemy->Draw(window);
	}
}