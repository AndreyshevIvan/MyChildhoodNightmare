#include "game.h"

using namespace std;
using namespace sf;

enum
{
	// Start menu buttons
	START_MENU_START = 0,
	START_MENU_OPTIONS,
	START_MENU_CLOSE,
	// Options menu buttons
	OPTIONS_MENU_EASY = 0,
	OPTIONS_MENU_NORMAL,
	OPTIONS_MENU_HARD,
	OPTIONS_MENU_BACK,
	// Pause menu buttons
	PAUSE_MENU_RESUME = 0,
	PAUSE_MENU_RESTART,
	PAUSE_MENU_EXIT,
};

template <class TContainer, class TPredicate>
void erase_if(TContainer &container, TPredicate && predicate)
{
	auto newEnd = std::remove_if(std::begin(container), std::end(container), [&](auto &pBullet) {
		return predicate(pBullet);
	});
	container.erase(newEnd, container.end());
}

bool Game::InitGame()
{
	if (!level_0.LoadFromFile("resources/previewTileset.tmx") ||
		!level_1.LoadFromFile("resources/firstTileset.tmx") ||
		!level_2.LoadFromFile("resources/secondTileset.tmx") ||
		!backgroundTexture_level_1.loadFromFile("resources/background_level_1.png") ||
		!backgroundTexture_level_preview.loadFromFile("resources/background_level_preview.png"))
	{
		return false;
	}


	if (!player.InitPlayer() ||
		!menu.InitMenuItems() ||
		!interface.Init() ||
		!gameSound.Init())
	{
		return false;
	}

	camera.reset(sf::FloatRect(0, 0, RESOLUTION.x, RESOLUTION.y));

	difficult = Difficult::EASY;
	menu.Select(CurrentMenu::DIFFICULT, difficult);
	menu.SetMenu(CurrentMenu::START, camera.getCenter());

	const sf::Vector2f BG_LVL_0_SISE = GetTextureSize(backgroundTexture_level_preview);
	const sf::Vector2f BG_LVL_1_SISE = GetTextureSize(backgroundTexture_level_1);

	background_level_1.setTexture(&backgroundTexture_level_1);
	background_level_1.setSize(BG_LVL_1_SISE);
	background_level_1.setOrigin(BG_LVL_1_SISE.x / 2.0f, BG_LVL_1_SISE.y / 2.0f);

	background_level_preview.setTexture(&backgroundTexture_level_preview);
	background_level_preview.setSize(BG_LVL_0_SISE);
	background_level_preview.setOrigin(BG_LVL_0_SISE.x / 2.0f, BG_LVL_0_SISE.y / 2.0f);

	buttonColdown = 0;

	return true;
}

void Game::StartGame(Level& level)
{
	ClearScene();

	currentLevel = &level;
	mapSize = { currentLevel->GetTilemapWidth(), currentLevel->GetTilemapHeight() };

	interface.CreateBoxes(boxesCoundMap.find(currentLevel)->second);

	blocks = currentLevel->GetObjects("solid");
	lava = currentLevel->GetObjects("lava");

	player.Clear();
	player.InitPlayer();

	switch (difficult)
	{
	case Difficult::EASY:
		bonusProbability = BONUS_PROBABILITY_EASY;
		break;
	case Difficult::NORMAL:
		bonusProbability = BONUS_PROBABILITY_NORMAL;
		break;
	case Difficult::HARD:
		bonusProbability = BONUS_PROBABILITY_HARD;
		break;
	default:
		break;
	}
	
	SpawnEntities();

	currentScene = &gameplayScene;
}

void Game::CheckCompletedLevel()
{
	auto door_level_1 = currentLevel->GetObject("level1").rect;
	auto door_level_2 = currentLevel->GetObject("level2").rect;
	auto playerRect = player.collisionRect;
	auto winBlock = currentLevel->GetObject("win").rect;

	int currBoxes = player.boxes;
	int neededBoxes = boxesCoundMap.find(currentLevel)->second;

	if (playerRect.intersects(door_level_1) && currBoxes == neededBoxes)
	{
		StartGame(level_1);
	}
	else if (playerRect.intersects(door_level_2) && currBoxes == neededBoxes)
	{
		StartGame(level_2);
	}
	else if (playerRect.intersects(winBlock) && currBoxes == neededBoxes)
	{
		currentScene = &winScene;
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
	SpawnBonuses();

	std::vector<Object> shadowsSpawns = currentLevel->GetObjects("enemy_shadow_spawn");
	std::vector<Object> clownsSpawns = currentLevel->GetObjects("enemy_clown_spawn");
	std::vector<Object> ghostSpawns = currentLevel->GetObjects("enemy_bird_spawn");
	std::vector<Object> spidersSpawns = currentLevel->GetObjects("enemy_spider_spawn");
	std::vector<Object> bosesSpawns = currentLevel->GetObjects("enemy_boss_spawn");
	std::vector<Object> itemsBoxSpawns = currentLevel->GetObjects("item_box_spawn");

	for (auto const& bossSpawn : bosesSpawns)
	{
		sf::Vector2f pos = { bossSpawn.rect.left, bossSpawn.rect.top };
		enemies.push_back(new Enemy(pos, EnemyType::BOSS));
	}

	for (auto const& spiderSpawn : spidersSpawns)
	{
		sf::Vector2f pos = { spiderSpawn.rect.left, spiderSpawn.rect.top };
		enemies.push_back(new Enemy(pos, EnemyType::SPIDER));
	}

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

	for (auto const& birdSpawn : ghostSpawns)
	{
		sf::Vector2f pos = { birdSpawn.rect.left, birdSpawn.rect.top };
		enemies.push_back(new Enemy(pos, EnemyType::GHOST));
	}

	for (auto const& itemsSpawn : itemsBoxSpawns)
	{
		sf::Vector2f itemPos = { itemsSpawn.rect.left, itemsSpawn.rect.top };
		bonuses.push_back(new Bonus(itemPos, Items::BOX));
	}

	sf::FloatRect posRect = currentLevel->GetObject("player_spawn").rect;
	player.Spawn({ posRect.left, posRect.top });
}

void Game::SpawnBonuses()
{
	std::vector<Object> bonusesSpawns = currentLevel->GetObjects("bonus_spawn");

	for (auto const& bonusSpawn : bonusesSpawns)
	{
		sf::Vector2f bonusPos = { bonusSpawn.rect.left, bonusSpawn.rect.top };
		bonuses.push_back(new Bonus(bonusPos));
	}

	std::vector<Object> bonusesSpawnsEasy;
	std::vector<Object> bonusesSpawnsNormal;

	if (difficult == Difficult::EASY)
	{
		bonusesSpawnsEasy = currentLevel->GetObjects("bonus_spawn_easy");
		bonusesSpawnsNormal = currentLevel->GetObjects("bonus_spawn_normal");
	}
	else if (difficult == Difficult::NORMAL)
	{
		bonusesSpawnsNormal = currentLevel->GetObjects("bonus_spawn_normal");
	}

	for (auto const& bonusSpawn : bonusesSpawnsEasy)
	{
		sf::Vector2f bonusPos = { bonusSpawn.rect.left, bonusSpawn.rect.top };
		bonuses.push_back(new Bonus(bonusPos));
	}

	for (auto const& bonusSpawn : bonusesSpawnsNormal)
	{
		sf::Vector2f bonusPos = { bonusSpawn.rect.left, bonusSpawn.rect.top };
		bonuses.push_back(new Bonus(bonusPos));
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
	return std::any_of(blocks.begin(), blocks.end(), [&](const Object &block) {
		return (rect.intersects(block.rect) && block.name == "solid");
	});
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
		}
		if (Keyboard::isKeyPressed(Keyboard::D) ||
			Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.runStatus = RUN_RIGHT;
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
	if (gameSound.mainMenu.getStatus() != sf::Sound::Playing)
	{
		gameSound.mainMenu.play();
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape) &&
		menu.buttonsColdown >= BUTTONS_COLDOWN &&
		menu.currentMenu == CurrentMenu::PAUSE)
	{
		menu.buttonsColdown = 0;
		currentScene = &gameplayScene;
	}
	else if ((Keyboard::isKeyPressed(Keyboard::F) || Keyboard::isKeyPressed(Keyboard::Return)) &&
			menu.buttonsColdown >= BUTTONS_COLDOWN)
	{
		menu.buttonsColdown = 0;
		ControlMenuLogic(window);
	}
	else if (Keyboard::isKeyPressed(Keyboard::Up) &&
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

void Game::ControlMenuLogic(sf::RenderWindow& window)
{
	switch (menu.currentMenu)
	{
	case CurrentMenu::START:
		switch (menu.currentButton)
		{
		case START_MENU_START:
			gameSound.mainMenu.stop();
			StartGame(level_0);
			break;
		case START_MENU_OPTIONS:
			menu.SetMenu(CurrentMenu::DIFFICULT, camera.getCenter());
			break;
		case START_MENU_CLOSE:
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
		case OPTIONS_MENU_EASY:
			difficult = Difficult::EASY;
			break;
		case OPTIONS_MENU_NORMAL:
			difficult = Difficult::NORMAL;
			break;
		case OPTIONS_MENU_HARD:
			difficult = Difficult::HARD;
			break;
		case OPTIONS_MENU_BACK:
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
		case PAUSE_MENU_RESUME:
			gameSound.mainMenu.stop();
			currentScene = &gameplayScene;
			break;
		case PAUSE_MENU_RESTART:
			gameSound.mainMenu.stop();
			StartGame(*currentLevel);
			break;
		case PAUSE_MENU_EXIT:
			gameSound.mainMenu.stop();
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
	player.UpdateOrientation();
	player.UpdatePos(elapsedTime, blocks);
	player.UpdateHealthStatus();
	player.UpdateStatuses();
	player.UpdateTexture();

	if (player.existStatus == ExistenceStatus::DEAD)
	{
		if (gameSound.playerDeath.getStatus() != sf::Music::Playing)
		{
			gameSound.playerDeath.play();
		}

		auto bodyRotation = player.bodyShape.getRotation();
		player.bodyShape.setRotation(bodyRotation + DEAD_ROTATION * elapsedTime / GAME_OVER_COLDOWN);

		if (gameOverColdown >= GAME_OVER_COLDOWN)
		{
			gameOverColdown = 0;
			currentScene = &gameOverScene;
		}
	}

	CheckCompletedLevel();
}

void Game::UpdateBullets()
{
	auto updateBullets = [&](vector<Bullet *> &bullets) {
		for (Bullet *pBullet : bullets)
		{
			pBullet->Update(elapsedTime);
		}
		erase_if(bullets, [&](Bullet *pBullet) {
			bool dead = (IsCollidesWithLevel(pBullet->collisionRect) || !pBullet->isLive);
			if (dead)
			{
				delete(pBullet);
			}
			return dead;
		});
	};

	updateBullets(player.characterBullets);
	updateBullets(enemyBullets);
}

void Game::UpdateEnemies()
{
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		Enemy* enemy = *it;
		enemy->UpdateAI(elapsedTime, player, blocks, enemyBullets);
		if (enemy->existStatus == ExistenceStatus::DEAD)
		{
			if (enemy->enemyType != EnemyType::BOSS)
			{
				CreateBonus(enemy->GetCharacterPos(), bonuses, bonusProbability);
			}
			else
			{
				bonuses.push_back(new Bonus(enemy->GetCharacterPos(), Items::BOX));
			}
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
				interface.CreateDemageAnnouncement(bullet->bodyShape.getPosition(), bullet->demage);
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
				player.health -= enemy->touchDemage;
				player.injuredColdown = 0;
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
		if (enemy->enemyType == EnemyType::CLOWN && enemy->shootColdown <= CLOWN_SHOOT_COLDOWN)
		{
			enemy->shootColdown += elapsedTime;
		}

		if (enemy->enemyType == EnemyType::BOSS && enemy->shootColdown <= BOSS_SHOOT_COLDOWN)
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
	interface.UpdatePlayerBoxes(player.boxes);
	interface.UpdateDemageAnnouncement(elapsedTime);

	for (auto enemy : enemies)
	{
		if (enemy->enemyType == EnemyType::BOSS)
		{
			interface.UpdateBossBar(BOSS_START_HEALTH, enemy->health);
		}
	}
}

void Game::UpdateBackground()
{
	auto mapWidth = currentLevel->GetTilemapWidth();
	auto mapHeight = currentLevel->GetTilemapHeight();
	auto backgroundWidth = currentBackground.getSize().x;
	auto backgroundHeight = currentBackground.getSize().y;

	if (currentLevel == &level_0)
	{
		currentBackground = background_level_preview;
	}
	else
	{
		currentBackground = background_level_1;
	}

	float bgPosX_Percent = (camera.getCenter().x - RESOLUTION.x / 2.0f) / (mapWidth - RESOLUTION.x);
	float bgPosY_Percent = (camera.getCenter().y - RESOLUTION.y / 2.0f) / (mapHeight - RESOLUTION.y);

	float bgAllowedWidthX = mapWidth - backgroundWidth;
	float bgAllowedWidthY = mapHeight - backgroundHeight;

	float bgPosX_Pixel = backgroundWidth / 2.0f + bgPosX_Percent * bgAllowedWidthX;
	float bgPosY_Pixel = backgroundHeight / 2.0f + bgPosY_Percent * bgAllowedWidthY;

	currentBackground.setPosition(bgPosX_Pixel, bgPosY_Pixel);
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

void Game::DrawInterface(sf::RenderWindow& window)
{
	interface.Draw(window);

	for (auto const& enemy : enemies)
	{
		if (enemy->enemyType == EnemyType::BOSS)
		{
			interface.DrawBossBar(window);
		}
	}
}