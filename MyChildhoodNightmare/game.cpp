#include "game.h"

using namespace std;
using namespace sf;

bool Game::InitGame()
{
	if (!level.LoadFromFile("resources/firstTileset.tmx"))
	{
		return false;
	}
	if (!player.InitPlayer(level.GetObject("player_spawn")) || !mainMenu.InitMenuItems())
	{
		return false;
	}

	mapTiles = level.GetAllObjects();
	mapSize = { level.GetTilemapWidth(), level.GetTilemapHeight() };
	camera.reset(sf::FloatRect(0, 0, RESOLUTION.x, RESOLUTION.y));
	gameStatus = MAIN_MENU;

	return true;
}

void Game::SetElapsedTime()
{
	elapsedTime = clock.getElapsedTime().asSeconds();
	clock.restart();
}

void Game::ControlPlayer(sf::RenderWindow& window, sf::Event& event)
{
	if (event.key.code == sf::Keyboard::Escape)
	{
		window.close();
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
	if (Keyboard::isKeyPressed(Keyboard::P))
	{
		player.Attack();
	}
	if (Keyboard::isKeyPressed(Keyboard::Num1))
	{
		player.weapon = MELEE;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num2))
	{
		player.weapon = FIREBALL;
	}
}

void Game::ControlMainMenu(sf::RenderWindow &window, sf::Event& event)
{
	if (Keyboard::isKeyPressed(Keyboard::F))
	{
		switch (mainMenu.currentButton)
		{
		case  MainMenu::START:
			gameStatus = PLAY;
			currentScene = &gameplayScene;
			break;
		case  MainMenu::CHANGE_MAP:
			break;
		case  MainMenu::DIFFICULT:
			break;
		case  MainMenu::EXIT:
			window.close();
			break;
		default:
			break;
		}
	}
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
	{
		if (mainMenu.currentButton == MainMenu::START)
		{
			mainMenu.currentButton = MainMenu::EXIT;
		}
		else
		{
			mainMenu.currentButton = MainMenu((int)mainMenu.currentButton - 1);
		}
	}
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
	{
		if (mainMenu.currentButton == MainMenu::EXIT)
		{
			mainMenu.currentButton = MainMenu::START;
		}
		else
		{
			mainMenu.currentButton = MainMenu((int)mainMenu.currentButton + 1);
		}
	}
}

void Game::UpdatePlayer()
{
	UpdateCharacterPos(player);
	UpdateBullets();
}

void Game::UpdateCharacterPos(Character& character)
{
	if (character.runStatus != NOT_RUN)
	{
		float movement = character.moveSpeed * elapsedTime;

		if (character.jumpStatus == FLY)
		{
			movement *= FLYING_SLOWDOWN;
		}
		if (character.runStatus == RUN_LEFT)
		{
			movement = -movement;
		}

		character.collisionRect.left += movement;

		if (IsCollidesWithLevel(character.collisionRect))
		{
			character.collisionRect.left -= movement;
		}
	}

	UpdateGravity(character);

	character.runStatus = NOT_RUN;
	character.bodyShape.setPosition(character.GetCharacterPos());
}

void Game::UpdateGravity(Character& character)
{
	float movementY = character.jumpSpeed;

	character.jumpSpeed = character.jumpSpeed + G * elapsedTime;
	movementY = character.jumpSpeed * elapsedTime;

	character.collisionRect.top += movementY;

	if (IsCollidesWithLevel(character.collisionRect))
	{
		character.collisionRect.top -= movementY;
		if (movementY > 0)
		{
			character.jumpStatus = ON_GROUND;
		}
		character.jumpSpeed = 0;
	}
	else
	{
		character.jumpStatus = FLY;
	}
}

void Game::UpdateBullets()
{
	for (auto bulletsIter = player.bullets.begin(); bulletsIter != player.bullets.end();)
	{
		Bullet* bullet = *bulletsIter;
		bullet->Update(elapsedTime);
		if (IsCollidesWithLevel(bullet->collisionRect))
		{
			bulletsIter = player.bullets.erase(bulletsIter);
			delete(bullet);
		}
		else
		{
			bulletsIter++;
		}
	}
}

bool Game::IsCollidesWithLevel(sf::FloatRect& rect)
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

void Game::UpdateColdowns()
{
	if (gameStatus == PLAY && player.shootColdown <= MAX_WEAPON_COLDOWN)
	{
		player.shootColdown += elapsedTime;
	}
}

void Game::UpdateCamera(sf::RenderWindow& window)
{
	sf::Vector2f cameraCenter = { player.GetCharacterPos().x, player.GetCharacterPos().y - CAMERA_VERTICAL_MARGIN };
	camera.setCenter(cameraCenter);
	float leftEdgeX = cameraCenter.x - RESOLUTION.x / 2.0f;
	float rightEdgeX = cameraCenter.x + RESOLUTION.x / 2.0f;
	float topEdgeY = cameraCenter.y - RESOLUTION.y / 2.0f;
	float bottomEdgeY = cameraCenter.y + RESOLUTION.y / 2.0f;

	if (leftEdgeX < 0)
	{
		camera.setCenter(RESOLUTION.x / 2.0f , cameraCenter.y);
		cameraCenter = camera.getCenter();
	}
	if (rightEdgeX > mapSize.x)
	{
		camera.setCenter(mapSize.x - RESOLUTION.x / 2.0f, cameraCenter.y);
		cameraCenter = camera.getCenter();
	}
	if (topEdgeY < 0)
	{
		camera.setCenter(cameraCenter.x, RESOLUTION.y / 2.0f);
		cameraCenter = camera.getCenter();
	}
	if (bottomEdgeY > mapSize.y)
	{
		camera.setCenter(cameraCenter.x, mapSize.y - RESOLUTION.y / 2.0f);
		cameraCenter = camera.getCenter();
	}

	window.setView(camera);
}

void Game::DrawLevel(sf::RenderWindow& window)
{
	level.Draw(window);
}

void Game::DrawCharacter(Character& character, sf::RenderWindow& window)
{
	window.draw(character.bodyShape);
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