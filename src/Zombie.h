#pragma once
#include <SFML/Graphics.hpp>

class Zombie
{
private:
	const float BLOATER_SPEED = 40.f;
	const float CHASER_SPEED = 80.f;
	const float CRAWLER_SPEED = 20.f;

	const float BLOATER_HEALTH = 5.f;
	const float CHASER_HEALTH = 1.f;
	const float CRAWLER_HEALTH = 3.f;

	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;

	sf::Vector2f m_Position;

	sf::Sprite m_Sprite;

	float m_Speed;

	float m_Health;

	bool m_Alive;

public:
	bool hit();

	bool is_Alive();

	void spawn(float startX, float startY, int type, int seed);

	sf::FloatRect getPosition();

	sf::Sprite getSprite();

	void update(float elapsedTime, sf::Vector2f playerLocation);
};
