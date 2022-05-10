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

