#include "Objects.h"
#include "Utils.h"
#include "Managers.h"

EnemyManager* Enemy::manager;
BulletManager* Bullet::manager;

sf::Vector2f Entity::GetPos() {
	return _position;
}

sf::Vector2f Entity::GetVel() {
	return _velocity;
}

void Entity::SetVel(sf::Vector2f newVel) {
	_velocity = newVel;
}

void Character::LookAt(sf::Vector2f target) {

	sf::Vector2f target2 = _position;
	sprDefault.setRotation(nmUtils::AngleBetween(target2, target));
}

bool Character::Hurt(int damage) {
	health -= damage;
	return true;
}

Player::Player(sf::Vector2f startPos) {
	_position = startPos;
	sprDefault.setTexture(TextureManager::Get().tDefaultPlayer);
	sprDefault.setOrigin(40, 40);
	sprDefault.setPosition(_position.x, _position.y);
}


void Player::Update() {
	if (_position.y > WINDOW_HEIGHT && _velocity.y > 0) {
		_velocity.y = 0;
	}
	if (_position.y < 0 && _velocity.y < 0) {
		_velocity.y = 0;
	}
	if (_position.x > WINDOW_HEIGHT && _velocity.x > 0) {
		_velocity.x = 0;
	}
	if (_position.x < 0 && _velocity.x < 0) {
		_velocity.x = 0;
	}

	_position.x += _velocity.x;
	_position.y += _velocity.y;

	_velocity *= DRAG;

	sprDefault.setPosition(_position.x, _position.y);

	if (FrameOdd && iFrames.getElapsedTime().asSeconds() < IFRAME_LENGTH) {
		sprDefault.setColor(sf::Color::Transparent);
		FrameOdd = false;
	}
	else {
		sprDefault.setColor(sf::Color::White);
		FrameOdd = true;
	}

	if (health == 0) sprDefault.setColor(sf::Color::Transparent);
	currentGun->SetPos(_position);
	if (colManager.CheckColl(this)) { Player::Hurt(1); }
}

bool Player::Hurt(int damage) {
	if (iFrames.getElapsedTime().asSeconds() > IFRAME_LENGTH) {
		health -= damage;
		if (health == 2) sprDefault.setTexture(TextureManager::Get().tHurt1Player);
		if (health == 1) sprDefault.setTexture(TextureManager::Get().tHurt2Player);

		iFrames.restart();
		return true;
	}
	else {
		return false;
	}
}




Enemy::Enemy(sf::Vector2f startPos, Player *playerStart) {
	sprDefault.setOrigin(40, 40);
	health = 10;
	//Rand spawn pos code, leaving it for future me <3 :***
	/*
	_position.x = rand() % 100 - 50;
	y = rand() % 100 - 50;
	if (x > 0) x += WINDOW_WIDTH;
	if (y > 0) y += WINDOW_HEIGHT;
	

	while (nmUtils::DistanceBetween(sprDefault.getPosition(), player->sprDefault.getPosition()) < SPAWN_RANGE) {
		x = rand() % 100 - 50;
		y = rand() % 100 - 50;
		if (x > 0) x += WINDOW_WIDTH;
		if (y > 0) y += WINDOW_HEIGHT;
	}
	*/
	sprDefault.setTexture(TextureManager::Get().tDefaultEnemy);
	_position = startPos;
	sprDefault.setPosition(startPos);
	directionOffset = nmUtils::RandVector2f();

	player = playerStart;

}

void Enemy::Update() {
	const sf::Vector2f &playerPos = player->sprDefault.getPosition();
	direction = sf::Vector2f(playerPos.x - _position.x, playerPos.y - _position.y);
	direction = nmUtils::NormaliseVector2f(direction);

	if (wanderDelay.getElapsedTime().asSeconds() > 3) {
		directionOffset = nmUtils::RandVector2f();
		wanderDelay.restart();
	}
	direction += directionOffset;
	_position.x += direction.x * ZOMBIE_SPEED;
	_position.y += direction.y * ZOMBIE_SPEED;

	LookAt(playerPos);
	sprDefault.setPosition(_position.x, _position.y);

	if (health < 0) {
		Enemy::manager->EnqueueDelete(this);
	}
}

Bullet::Bullet(sf::Vector2f position, sf::Vector2f direction, int damage) {
	_position = position;
	sf::Vector2f zeroVector = sf::Vector2f(0,0);
	sprDefault.setRotation(90.+nmUtils::AngleBetween(zeroVector, direction));
	sprDefault.setTexture(TextureManager::Get().tDefaultBullet);
	_velocity = direction * float(BULLET_SPEED);
	_damage = damage;
}

void Bullet::Update() {
	//Notice: no drag
	_position.x += _velocity.x;
	_position.y += _velocity.y;
	sprDefault.setPosition(_position.x, _position.y);
	Enemy* hitObj = colManager.CheckColl(this);
	if (hitObj) {
		hitObj->Hurt(_damage);
		Bullet::manager->EnqueueDelete(this);
	}

	if (_position.x > WINDOW_WIDTH || _position.x < 0
		|| _position.y > WINDOW_HEIGHT, _position.y < 0) {
		Bullet::manager->EnqueueDelete(this);
	}
}

void Gun::Update() {
}

void Gun::SetPos(sf::Vector2f position) {
	_position = position;
}

Gun::Gun(std::string name, int curClip, int maxClip, float timeReload, float timeFireDelay) {
	_name = name;
	_curClip = curClip;
	_maxClip = maxClip;
	_timeReload = timeReload;
	_timeFireDelay = timeFireDelay;
}

Bullet* Gun::Fire(sf::Vector2f direction) {
	if (_curClip > 0 
		&& cl_timeFire.getElapsedTime().asSeconds() >= _timeFireDelay
		&& cl_timeReload.getElapsedTime().asSeconds() >= _timeReload) {
		return new Bullet(_position, direction, 1);
		cl_timeFire.restart();

	}
	return nullptr;
}