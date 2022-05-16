#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include "Defines.h"

using namespace std;

class Enemy;
class Player;
class Bullet;
class Entity;


class TextureManager {
public:
	static TextureManager& Get()
	{
		static TextureManager instance;
		return instance;
	}

	//Player
	sf::Texture tDefaultPlayer;
	sf::Texture tHurt1Player, tHurt2Player;

	//Enemy
	sf::Texture tDefaultEnemy;

	//Bullet
	sf::Texture tDefaultBullet;

	//Guns
	sf::Texture tPistol, tAssaultRifle;

	//Peter

	TextureManager();
};


class CollisionManager {
private:
	vector<Enemy*>* _enemies;
	Player* _player;
	vector<Bullet*>* _bullets;
public:
	CollisionManager(Player* player, vector<Enemy*>* enemies, vector<Bullet*>* bullets);
	CollisionManager() : _enemies(nullptr), _player(nullptr), _bullets(nullptr) {}
	Enemy* CheckColl(Entity* collider);
	void Set(Player* player, vector<Enemy*>* enemies, vector<Bullet*>* bullets);
};

class BulletManager {
private:
	vector<Bullet*> _bullets;
public:
	BulletManager();
	Bullet* SpawnBullet(sf::Vector2f position, sf::Vector2f direction, int damage);
	Bullet* SpawnBullet(Bullet *bullet);
	void DespawnBullet(Bullet *bullet);
	vector<Bullet*>* GetBulletVector();
};

class EnemyManager {
private:
	vector<Enemy*> _enemies;
public:
	EnemyManager();
	Enemy* SpawnEnemy(sf::Vector2f startPos, Player* player);
	void DespawnEnemy(Enemy* enemy);
	vector<Enemy*>* GetEnemyVector();
};

extern CollisionManager colManager;
//extern TextureManager textureManager;
