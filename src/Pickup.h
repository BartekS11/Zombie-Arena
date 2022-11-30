#pragma once
#include <SFML/Graphics.hpp>

class Pickup
{
private:
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;

	sf::Sprite m_Sprite;
	sf::IntRect m_Arena;
	int m_Value = 0;
	int m_Type = 0;

	bool m_Spawned;
	float m_SecondsSinceSpawn = 0.f;
	float m_SecondsSinceDeSpawn = 0.f;
	float m_SecondsToLive = 0.f;
	float m_SecondsToWait = 0.f;

public:
	Pickup(int type);
	
	void setArena(sf::IntRect arena);

	void spawn();

	sf::FloatRect getPosition();

	sf::Sprite getSprite();
	
	void update(float elapsedTime);

	bool isSpawned();

	int gotIt();

	void upgrade();
};