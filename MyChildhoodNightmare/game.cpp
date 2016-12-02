#include "game.h"

using namespace std;
using namespace sf;

bool Game::InitGame()
{
	if (!level.LoadFromFile("resources/firstTileset.tmx"))
	{
		return false;
	}
	if (!player.InitPlayer(level.GetObject("player_spawn")) || !menu.InitMenuItems())
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

	return true;
}

void Game::StartGame()
{
	player.InitPlayer(level.GetObject("player_spawn"));

	auto enemiesSpawns = level.GetObjects("enemy_shadow_spawn");
	for (auto posIt = enemiesSpawns.begin(); posIt != enemiesSpawns.end(); posIt++)
	{
		enemyShadows.push_back(new EnemyShadow(*posIt));
	}

	currentScene = &gameplayScene;
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
	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		menu.currentMenu = CurrentMenu::PAUSE;
		menu.currentButton = 0;
		currentScene = &menuScene;
	}
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

void Game::ControlMenu(sf::RenderWindow& window, sf::Event& event)
{
	if (Keyboard::isKeyPressed(Keyboard::F) && menu.buttonsColdown >= BUTTONS_COLDOWN)
	{
		menu.buttonsColdown = 0;
		ControlMenuLogic(window, event);
	}
	else 
	{
		auto currMenu = menu.allItems[(size_t)menu.currentMenu];
		auto maxItem = currMenu.size() - 1;

		if (Keyboard::isKeyPressed(Keyboard::Up) && menu.buttonsColdown >= BUTTONS_COLDOWN)
		{
			menu.buttonsColdown = 0;
			if (menu.currentButton == 0)
			{
				menu.currentButton = maxItem;
			}
			else
			{
				menu.currentButton = menu.currentButton - 1;
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down) && menu.buttonsColdown >= BUTTONS_COLDOWN)
		{
			menu.buttonsColdown = 0;
			if (menu.currentButton == maxItem)
			{
				menu.currentButton = 0;
			}
			else
			{
				menu.currentButton = menu.currentButton + 1;
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
			menu.currentMenu = CurrentMenu::DIFFICULT;
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
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			menu.currentMenu = CurrentMenu::START;
			break;
		default:
			break;
		}
		menu.currentButton = 0;
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
			menu.currentMenu = CurrentMenu::START;
			break;
		default:
			break;
		}
		menu.currentButton = 0;
		break;
	default:
		break;
	}
}

void Game::UpdatePlayer()
{
	player.UpdatePos(elapsedTime, mapTiles);
	UpdateBullets();
}

void Game::UpdateEnemies()
{
	for (auto enemyIt = enemyShadows.begin(); enemyIt != enemyShadows.end(); enemyIt++)
	{
		EnemyShadow* enemy = *enemyIt;
		enemy->UpdatePos(elapsedTime, mapTiles);
	}
}

void Game::UpdateBullets()
{
	bool flag = false; // Если последняя пуля сталкивается с врагом, то инкрементируется мёртвый итератор
	if (!player.bullets.empty())
	{
		for (auto bulletsIt = player.bullets.begin(); bulletsIt != player.bullets.end();)
		{
			Bullet* bullet = *bulletsIt;
			bullet->Update(elapsedTime);
			cout << "test" "\n";
			if (IsCollidesWithLevel(bullet->collisionRect))
			{
				bulletsIt = player.bullets.erase(bulletsIt);
				delete(bullet);
			}
			else
			{
				for (auto enemyIt = enemyShadows.begin(); enemyIt != enemyShadows.end();)
				{
					EnemyShadow* enemy = *enemyIt;
					if (bullet->collisionRect.intersects(enemy->collisionRect))
					{
						bulletsIt = player.bullets.erase(bulletsIt);
						delete(bullet);
						enemyIt = enemyShadows.erase(enemyIt);
						delete(enemy);
						flag = true;
					}
					else
					{
						++enemyIt;
					}
				}
				if (!flag)
				{
					++bulletsIt;
				}
			}
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