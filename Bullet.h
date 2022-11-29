#pragma once
#include <SFML/Graphics.hpp>

class Bullet
{
private:
	sf::Vector2f m_Position;

	sf::RectangleShape m_BulletShape;

	bool m_InFlight = false;

	float m_BulletSpeed = 1000;

	float m_BulletDistanceX;
	float m_BulletDistanceY;
	
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

public:
	Bullet();

	void stop();

	bool isInFlight();

	void shoot(float startX, float startY, float xTarget, float yTarget);

	sf::FloatRect getPosition();

	sf::RectangleShape getShape();

	void update(float elapsedTime);
};