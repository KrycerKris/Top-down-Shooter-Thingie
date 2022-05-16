#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <format>
#include "Defines.h"

class EnemyManager;
class BulletManager;

//Entity is supposed to be the very barebones of an object in the game, be it player, enemy or even item
class Entity {
protected:
	//Basic physics stuff
	sf::Vector2f _position;
	sf::Vector2f _velocity;
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


//Bullet
class Bullet : public Entity, public Renderable {
private:
	int _damage;
	sf::Texture _tDefault;
public:
	void Update();
	Bullet(sf::Vector2f position, sf::Vector2f direction, int damage);
	static BulletManager* manager;
};

//Gun
class Gun : public Entity, public Renderable {
private:
	std::string _name;
	int _curClip, _maxClip;
	float _timeReload, _timeFireDelay;
public:
	void Update();
	Gun(std::string name, int curClip, int maxClip, float timeReload, float timeFireDelay);
	Bullet* Fire(sf::Vector2f direction);
	void SetPos(sf::Vector2f position);
};

//Character
class Character : public Entity, public Renderable {
protected:
	int health;
	sf::Clock iFrames;
	bool FrameOdd = false;
public:
	virtual bool Hurt(int damage);
	void LookAt(sf::Vector2f);
};

//Player
class Player : public Character {
private:
	int health = 3;
public:
	Player(sf::Vector2f startPos);
	void Update();
	bool Hurt(int damage);
	Gun* currentGun = nullptr; //BANDAID fix nes ntr jegu dar cia extrapoliuot kazka, nelabai grazu, bet sueis
};

//Enemy
class Enemy : public Character {
private:
	sf::Vector2f direction;
	sf::Vector2f directionOffset;
	Player *player;
	sf::Clock wanderDelay;
public:
	static EnemyManager* manager;
	void Update();
	Enemy(sf::Vector2f startPos, Player* player);
};


