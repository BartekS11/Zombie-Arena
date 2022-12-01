#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
#include "SoundHandler.h"
#include "TextHandler.h"

int main()
{
	TextureHolder holder;
	SoundHandler soundHandler;
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

	State state = State::GAME_OVER;

	sf::Vector2f resolution;
	resolution.x = sf::VideoMode::getDesktopMode().width;
	resolution.y = sf::VideoMode::getDesktopMode().height;

	sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Zombie Arena", sf::Style::Fullscreen);

	sf::View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	sf::Clock clock;

	sf::Time gameTimeTotal;

	sf::Vector2f mouseWorldPosition;

	sf::Vector2i mouseScreenPosition;

	Player player;

	sf::IntRect arena;

	sf::VertexArray background;

	sf::Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	SoundHandler::LoadNecessarySoundsToBuffers();
	SoundHandler::LoadNecessarySoundsToSFMLSounds();

	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	sf::Time lastPressed;
	
	window.setMouseCursorVisible(true);
	sf::Sprite spriteCrosshair;
	sf::Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	int score = 0;
	int hiScore = 0;

	sf::Sprite spriteGameOver;
	sf::Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	sf::View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	sf::Sprite spriteAmmoIcon;

	sf::Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);

	sf::Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	TextHandler pausedText(font, 400, 400, 
		sf::Color::White, 155, "Press Enter \nto continue");

	TextHandler gameOverText(font, 250, 850,
		sf::Color::White, 125, "Press Enter to play");

	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";

	TextHandler levelUpText(font, 150, 250,
		sf::Color::White, 80, levelUpStream.str());

	TextHandler ammoText(font, 200, 980,
		sf::Color::White, 55);

	TextHandler scoreText(font, 20, 0,
		sf::Color::White, 55);

	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}

	std::stringstream s;
	s << "Hi Score:" << hiScore;

	TextHandler hiScoreText(font, 1400, 0,
		sf::Color::White, 55, s.str());

	TextHandler zombiesRemainingText(font, 1500, 980,
		sf::Color::White, 55, "Zombies: 100");

	int wave = 0;
	TextHandler waveNumberText(font, 1250, 980,
		sf::Color::White, 55, "Wave: 0");

	sf::RectangleShape healthBar;
	healthBar.setFillColor(sf::Color::Red);
	healthBar.setPosition(450, 980);
	
	int framesSinceLastHUDUpdate = 0;
	int fpsMeasurementFrameInterval = 1000;

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Return && 
					state == State::PLAYING)
				{
					state = State::PAUSED;
				}
				else if (event.key.code == sf::Keyboard::Return &&
						 state == State::PAUSED)
				{
					state = State::PLAYING;
					clock.restart();
				}
				else if (event.key.code == sf::Keyboard::Return &&
						 state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
					wave = 0;
					score = 0;

					currentBullet = 0;
					bulletsSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;
					player.resetPlayerStats();
				}

				if (state == State::PLAYING)
				{
					if (event.key.code == sf::Keyboard::R)
					{
						if (bulletsSpare >= clipSize)
						{
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
							SoundHandler::playSound("reload", 30);
						}
						else if (bulletsSpare > 0)
						{
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
							SoundHandler::playSound("reload", 30);
						}
						else
						{
							SoundHandler::playSound("reload_Failed", 25);
						}
					}
					if (bulletsInClip == 0 && sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						bulletsInClip = clipSize;
						bulletsSpare -= clipSize;
						SoundHandler::playSound("reload", 30);
					}
				}
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}

		if (state == State::PLAYING)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds()
				   > 1000 / fireRate && bulletsInClip > 0)
				{
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y,
												 mouseWorldPosition.x, mouseWorldPosition.y);
					currentBullet++;
					if (currentBullet > 99)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;

					SoundHandler::playSound("shoot", 10);
					bulletsInClip--;

				}
			}
		}

		if (state == State::LEVELING_UP)
		{
			if (event.key.code == sf::Keyboard::Num1)
			{
				fireRate++;
				state = State::PLAYING;
			}
			if (event.key.code == sf::Keyboard::Num2)
			{
				clipSize += clipSize;
				state = State::PLAYING;
			}
			if (event.key.code == sf::Keyboard::Num3)
			{
				player.upgradeHealth();
				state = State::PLAYING;
			}
			if (event.key.code == sf::Keyboard::Num4)
			{
				player.upgradeSpeed();
				state = State::PLAYING;
			}
			if (event.key.code == sf::Keyboard::Num5)
			{
				healthPickup.upgrade();
				state = State::PLAYING;
			}
			if (event.key.code == sf::Keyboard::Num6)
			{
				ammoPickup.upgrade();
				state = State::PLAYING;
			}

			if (state == State::PLAYING)
			{
				wave++;
				arena.width = 800 * wave;
				arena.height = 800 * wave;
				arena.left = 0;
				arena.top = 0;

				int tileSize = createBackground(background, arena);
				player.spawn(arena, resolution, tileSize);

				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				numZombies = 5 * wave;

				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				SoundHandler::playSound("powerup", 15);
				clock.restart();
			}
		}
		if (state == State::PLAYING)
		{
			sf::Time dt = clock.restart();

			gameTimeTotal += dt;

			float dtAsSeconds = dt.asSeconds();

			mouseScreenPosition = sf::Mouse::getPosition();

			mouseWorldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(), mainView);

			spriteCrosshair.setPosition(mouseWorldPosition);
			
			player.update(dtAsSeconds, sf::Mouse::getPosition());

			sf::Vector2f playerPosition(player.getCenter());

			mainView.setCenter(player.getCenter());

			for (auto i = 0; i < numZombies; i++)
			{
				if (zombies[i].is_Alive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}
			
			for (auto i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}

			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			for (auto i = 0; i < 100; i++)
			{
				for (auto j = 0; j < numZombies; j++)
				{
					if(bullets[i].isInFlight() && zombies[j].is_Alive())
					{
						if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
						{
							bullets[i].stop();

							if (zombies[j].hit())
							{
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}
								numZombiesAlive--;

								if (numZombiesAlive == 0)
								{
									state = State::LEVELING_UP;
								}
							}
							SoundHandler::playSound("splat", 15);
						}
					}
				}
			}
			for (auto i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].is_Alive())
				{
					if (player.hit(gameTimeTotal))
					{
						SoundHandler::playSound("hit", 25);
					}
					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;

						std::ofstream outputFile("gamedata/scores.txt");
						outputFile << hiScore;
						outputFile.close();
					}
				}
			}
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletsSpare += ammoPickup.gotIt();
				SoundHandler::playSound("reload", 25);
			}
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
				SoundHandler::playSound("pickup", 25);
			}
			
			healthBar.setSize(sf::Vector2f(player.getHealth() * 3, 50));
			framesSinceLastHUDUpdate++;

			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;

				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.updateString(ssAmmo.str());

				ssScore << "Score:" << score;
				scoreText.updateString(ssScore.str());

				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.updateString(ssHiScore.str());

				ssWave << "Wave:" << wave;
				waveNumberText.updateString(ssWave.str());

				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.updateString(ssZombiesAlive.str());

				framesSinceLastHUDUpdate = 0;
			}
		}
		if (state == State::PLAYING)
		{
			window.clear();
			window.setView(mainView);

			window.draw(background, &textureBackground);
			
			for (auto i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}

			for (auto i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}

			window.draw(player.getSprite());
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}

			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}

			window.draw(spriteCrosshair);

			window.setView(hudView);
			window.draw(spriteAmmoIcon);
			window.draw(ammoText.getsfText());
			window.draw(scoreText.getsfText());
			window.draw(hiScoreText.getsfText());
			window.draw(healthBar);
			window.draw(waveNumberText.getsfText());
			window.draw(zombiesRemainingText.getsfText());
		}

		if (state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText.getsfText());
		}

		if (state == State::PAUSED)
		{
			window.draw(pausedText.getsfText());
		}

		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText.getsfText());
			window.draw(scoreText.getsfText());
			window.draw(hiScoreText.getsfText());
		}
		window.display();
	}
	delete[] zombies;
	return 0;
}

