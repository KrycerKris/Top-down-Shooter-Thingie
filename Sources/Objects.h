#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <format>
#include "Defines.h"

//Entity is supposed to be the very barebones of an object in the game, be it player, enemy or even item
class Entity {
protected:
	//Basic physics stuff
	sf::Vector2f position;
	sf::Vector2f velocity;
public:
	virtual void Update() = 0;
	sf::Vector2f GetPos();
	sf::Vector2f GetVel();
	void SetVel(sf::Vector2f);
};

class Renderable {
public:
	sf::Sprite sprDefault;
};

class Gun : public Entity {
private:

public:
};

class Character : public Entity, public Renderable {
protected:
	int health;
	sf::Clock iFrames;
	bool FrameOdd = false;
public:
	virtual bool Hurt();
	void LookAt(sf::Vector2f);
};



class Player : public Character {
private:
	int health = 3;
	sf::Texture tDefault, tHurt2, tHurt1, tEmpty;
public:
	Player(sf::Vector2f startPos);
	void Update();
	bool Hurt();
	Gun* currentGun;
};

class Enemy : public Character {
private:
	sf::Vector2f direction;
	sf::Vector2f directionOffset;
	Player *player;
	sf::Clock wanderDelay;
	sf::Texture tDefault;
public:
	void Update();
	Enemy(sf::Vector2f startPos, Player* player);
};

class Bullet : public Entity {
private:
	sf::CircleShape circle;
	void CheckCollisions();
	int bulletSpeed;
public:
	void Update();
	Bullet(sf::Vector2f direction, int bulletSpeed);
};

