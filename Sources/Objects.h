#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <format>
#include "Defines.h"

class Player {
public:
	float x;
	float y;
	sf::Vector2f velocity;
	
	int health = 3;
	sf::Clock iFrames;
	bool FrameOdd = false;

	sf::Texture tDefault, tHurt2, tHurt1, tEmpty;
	sf::Sprite sprDefault;

	void Start();
	void Update();
	void LookAt(sf::Vector2f);
	bool TakeDamage();
};

class Enemy {
public:
	float x;
	float y;
	sf::Vector2f direction;
	sf::Vector2f directionOffset;
	Player *player;
	sf::Clock wanderDelay;
	void Start();
	void Update();
	sf::Texture tDefault;
	sf::Sprite sprDefault;
	void LookAt(sf::Vector2f);
};

class Bullet {
public:
	float x;
	float y;
	sf::Vector2f direction;
	sf::CircleShape circle;
	void Start();
	void Update();
};
