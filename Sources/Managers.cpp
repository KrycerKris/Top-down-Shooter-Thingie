#include "Managers.h"
#include "Objects.h"
#include "Utils.h"
CollisionManager colManager;



CollisionManager::CollisionManager(Player* player, vector<Enemy*>* enemies, vector<Bullet*>* bullets) {
	_enemies = enemies;
	_player = player;
	_bullets = bullets;
}

void CollisionManager::Set(Player* player, vector<Enemy*>* enemies, vector<Bullet*>* bullets) {
	_enemies = enemies;
	_player = player;
	_bullets = bullets;
}

Enemy* CollisionManager::CheckColl(Entity *collider) {
	for (auto& enemy : *_enemies) {
		if (nmUtils::DistanceBetween(collider->GetPos(), enemy->GetPos()) <= COLLISION_RANGE) {
			return enemy;
		}
	}

	return nullptr;
}

TextureManager::TextureManager() {
	// Load Class Assets
	tDefaultPlayer.loadFromFile("Resources/playr.png");
	tHurt1Player.loadFromFile("Resources/playr_h1.png");
	tHurt2Player.loadFromFile("Resources/playr_h2.png");

	tDefaultEnemy.loadFromFile("Resources/enmy.png");

	tDefaultBullet.loadFromFile("Resources/bullet.png");

	tPistol.loadFromFile("Resources/peter.png");

	
}

EnemyManager::EnemyManager() {
	_enemies.reserve(ENEMY_MAXCOUNT);
}

Enemy* EnemyManager::SpawnEnemy(sf::Vector2f startPos, Player* player) {
	_enemies.push_back(new Enemy(startPos, player));
	return *_enemies.rbegin();
}

void EnemyManager::DespawnEnemy(Enemy *enemy) {
	auto it = find(_enemies.begin(), _enemies.end(), enemy);
	if (it != _enemies.end()) {
		delete* it;
		_enemies.erase(it);
	}
}

vector<Enemy*>* EnemyManager::GetEnemyVector() {
	return &_enemies;
}

BulletManager::BulletManager() {
	_bullets.reserve(BULLET_MAXCOUNT);
}

Bullet* BulletManager::SpawnBullet(sf::Vector2f position, sf::Vector2f direction, int damage) {
	_bullets.push_back(new Bullet(position, direction, damage));
	return *_bullets.rbegin();
}

Bullet* BulletManager::SpawnBullet(Bullet *bullet) {
	if(bullet != nullptr)
		_bullets.push_back(bullet);
	return bullet;
}

void BulletManager::DespawnBullet(Bullet* bullet) {
	auto it = find(_bullets.begin(), _bullets.end(), bullet);
	if (it != _bullets.end()) {
		delete *it;
		_bullets.erase(it);
	}
}

vector<Bullet*>* BulletManager::GetBulletVector() {
	return &_bullets;
}