#pragma once
#include <SFML/Graphics.hpp>

class Player 
{
private:
	const float START_SPEED = 200.f;
	const float START_HEALTH = 100.f;

	sf::Vector2f m_Position;

	sf::Sprite m_Sprite;


	sf::Texture m_Texture;

	sf::Vector2f m_Resolution;

	sf::IntRect m_Arena;

	int m_TileSize = 0;

	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	int m_Health = 0;
	int m_MaxHealth = 0;

	sf::Time m_LastHit;

	float m_Speed = 0.f;

public:
	Player();

	void spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize);

	void resetPlayerStats();

	bool hit(sf::Time timeHit);

	sf::Time getLastHitTime();

	sf::FloatRect getPosition();

	sf::Vector2f getCenter();

	float getRotation();

	sf::Sprite getSprite();

	void moveLeft();
	
	void moveRight();

	void moveUp();

	void moveDown();

	void stopLeft();

	void stopRight();

	void stopUp();

	void stopDown();

	void update(float elapsedTime, sf::Vector2i mousePosition);

	void upgradeSpeed();

	void upgradeHealth();

	void increaseHealthLevel(int amount);

	int getHealth();
};