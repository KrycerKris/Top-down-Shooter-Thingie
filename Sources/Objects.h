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
	void Start();
	void Update();
	sf::Texture tDefault;
	sf::Sprite sprDefault;
	void LookAt(sf::Vector2f);
};

class Enemy {
public:
	float x;
	float y;
	sf::Vector2f direction;
	Player player;
	void Start();
	void Update();

	sf::Vector2f playerPos;

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
