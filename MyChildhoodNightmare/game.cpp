#include "game.h"

using namespace std;
using namespace sf;

bool Game::InitGame()
{
    if (level.LoadFromFile("resources/firstTileset.tmx") &&
        player.InitCharacter(PLAYER_FILE_NAME, PLAYER_SPAWN_POS, PLAYER_SIZE, PLAYER_MOVE_SPEED, PLAYER_JUMP_HEIGHT) &&
        mainMenu.InitMenuItems())
    {
        mapTiles = level.GetAllObjects();
        camera.reset(sf::FloatRect(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT));
        gameStatus = MAIN_MENU;

        return true;
    }
	 
	return false;
}

void Game::UpdateCamera(sf::RenderWindow& window)
{
	camera.setCenter(player.GetCharacterPos());
	window.setView(camera);
}

void Game::SetElapsedTime()
{
	elapsedTime = clock.getElapsedTime().asSeconds();
	clock.restart();
}

void Game::DrawLevel(sf::RenderWindow& window)
{
	level.Draw(window);
}

void Game::DrawCharacter(Character& character, sf::RenderWindow& window)
{
	window.draw(character.bodyShape);
	window.draw(character.collisionShape);
}

void Game::ControlPlayer()
{
    if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Space))
    {
        player.Jump();
    }
    if (Keyboard::isKeyPressed(Keyboard::S))
    {
        player.Seat();
    }
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        player.runStatus = RUN_LEFT;
        player.orientation = LEFT;
    }
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
        player.runStatus = RUN_RIGHT;
        player.orientation = RIGHT;
    }
    if (Keyboard::isKeyPressed(Keyboard::P))
    {
        player.Shoot(level);
    }
    if (Keyboard::isKeyPressed(Keyboard::Escape))
    {
        gameStatus = PAUSE;
    }
    if (Keyboard::isKeyPressed(Keyboard::Num1))
    {
        player.weapon = CRY;
    }
    if (Keyboard::isKeyPressed(Keyboard::Num2))
    {
        player.weapon = PISTOL;
    }
    if (Keyboard::isKeyPressed(Keyboard::Num3))
    {
        player.weapon = AK;
    }
}

void Game::ControlMainMenu(sf::RenderWindow &window)
{
    if (Keyboard::isKeyPressed(Keyboard::F))
    {
        switch ((int)mainMenu.currentButton)
        {
        case 0:
            gameStatus = PLAY;
            m_currentScene = &m_gameplayScene;
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            window.close();
            break;
        default:
            break;
        }
    }
    if ((Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && mainMenu.buttonsColdown >= BUTTONS_COLDOWN)
    {
        mainMenu.buttonsColdown = 0;
        if (mainMenu.currentButton == START)
        {
            mainMenu.currentButton = EXIT;
        }
        else
        {
            mainMenu.currentButton = MainMenu((int)mainMenu.currentButton - 1);
        }
    }
    if ((Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && mainMenu.buttonsColdown >= BUTTONS_COLDOWN)
    {
        mainMenu.buttonsColdown = 0;
        if (mainMenu.currentButton == EXIT)
        {
            mainMenu.currentButton = START;
        }
        else
        {
            mainMenu.currentButton = MainMenu((int)mainMenu.currentButton + 1);
        }
    }
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

		character.collisionShape.move(movement, 0);

		if (IsCollidesWithLevel(character.collisionShape))
		{
			character.collisionShape.move(-movement, 0);
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
	movementY = JUMP_IMPULSE * character.jumpSpeed * elapsedTime;
	
	character.collisionShape.move(0, movementY);

	if (IsCollidesWithLevel(character.collisionShape))
	{
		character.collisionShape.move(0, -movementY);
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

bool Game::IsCollidesWithLevel(sf::RectangleShape& shape)
{
	auto collisionRect = shape.getGlobalBounds();

	for (unsigned int i = 0; i < mapTiles.size(); i++)
	{
		if (collisionRect.intersects(mapTiles[i].rect) && mapTiles[i].name == "solid")
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
    if (gameStatus == MAIN_MENU && mainMenu.buttonsColdown <= BUTTONS_COLDOWN)
    {
        mainMenu.buttonsColdown += elapsedTime;
    }
}

void Game::UpdateBullets(Character& character)
{
	for (character.bulletsIter = character.bullets.begin(); character.bulletsIter != character.bullets.end();)
	{
		Bullet* bullet = *character.bulletsIter;
		bullet->Update(elapsedTime);
		if (bullet->IsLife == false)
		{
			character.bulletsIter = character.bullets.erase(character.bulletsIter);
			delete(bullet);
		}
		else
		{
			character.bulletsIter++;
		}
	}
}

void Game::DrawBullets(sf::RenderWindow& window, Character& character)
{
	for (character.bulletsIter = character.bullets.begin(); character.bulletsIter != character.bullets.end();)
	{
		Bullet* bullet = *character.bulletsIter;
		window.draw(bullet->bodyShape);
		character.bulletsIter++;
	}
}

void Game::UpdateHealthBar()
{
	player.healthBar.setPosition(player.GetCharacterPos() + PLAYER_BAR_DISPLACEMENT);
}

void Game::DrawPlayerBar(sf::RenderWindow& window)
{
	window.draw(player.healthBar);
}
