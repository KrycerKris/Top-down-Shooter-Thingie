#pragma once
#include <vector>
using namespace std;

class Enemy;
class Player;
class Bullet;
class Entity;

class CollisionManager {
private:
	vector<Enemy*>* _enemies;
	Player* _player;
	vector<Bullet*>* _bullets;
public:
	CollisionManager(Player* player, vector<Enemy*>* enemies, vector<Bullet*>* bullets);
	CollisionManager() {}
	Enemy* CheckColl(Entity* collider);
	void Set(Player* player, vector<Enemy*>* enemies, vector<Bullet*>* bullets);
};

class EnemyManager {
	//TODO
};

extern CollisionManager colManager;