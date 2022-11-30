#pragma once
#include <SFML/Graphics.hpp>

class Bullet
{
private:
	sf::Vector2f m_Position;

	sf::RectangleShape m_BulletShape;

	bool m_InFlight = false;

	float m_BulletSpeed = 1000.f;

	float m_BulletDistanceX = 0.f;
	float m_BulletDistanceY = 0.f;
	
	float m_MaxX = 0.f;
	float m_MinX = 0.f;
	float m_MaxY = 0.f;
	float m_MinY = 0.f;

public:
	Bullet();

	void stop();

	bool isInFlight();

	void shoot(float startX, float startY, float xTarget, float yTarget);

	sf::FloatRect getPosition();

	sf::RectangleShape getShape();

	void update(float elapsedTime);
};