#include "Objects.h"
#include "Utils.h"
#include "Managers.h"


sf::Vector2f Entity::GetPos() {
	return position;
}

sf::Vector2f Entity::GetVel() {
	return velocity;
}

void Entity::SetVel(sf::Vector2f newVel) {
	velocity = newVel;
}

void Character::LookAt(sf::Vector2f target) {

	sf::Vector2f target2 = position;
	sprDefault.setRotation(nmUtils::AngleBetween(target2, target));
}

Player::Player(sf::Vector2f startPos) {
	tDefault.loadFromFile("Resources/playr.png");
	tHurt1.loadFromFile("Resources/playr_h1.png");
	tHurt2.loadFromFile("Resources/playr_h2.png");
	sprDefault.setTexture(tDefault);
	sprDefault.setOrigin(40, 40);
	position = startPos;

	sprDefault.setPosition(position.x, position.y);
}


void Player::Update() {
	if (position.y > WINDOW_HEIGHT && velocity.y > 0) {
		velocity.y = 0;
	}
	if (position.y < 0 && velocity.y < 0) {
		velocity.y = 0;
	}
	if (position.x > WINDOW_HEIGHT && velocity.x > 0) {
		velocity.x = 0;
	}
	if (position.x < 0 && velocity.x < 0) {
		velocity.x = 0;
	}

	position.x += velocity.x;
	position.y += velocity.y;

	velocity *= DRAG;

	sprDefault.setPosition(position.x, position.y);

	if (FrameOdd && iFrames.getElapsedTime().asSeconds() < IFRAME_LENGTH) {
		sprDefault.setColor(sf::Color::Transparent);
		FrameOdd = false;
	}
	else {
		sprDefault.setColor(sf::Color::White);
		FrameOdd = true;
	}

	if (health == 0) sprDefault.setColor(sf::Color::Transparent);

	if (colManager.CheckCollPlayer()) { Player::Hurt(); }
}
bool Character::Hurt() {
	health--;
	return true;
}

bool Player::Hurt() {
	if (iFrames.getElapsedTime().asSeconds() > IFRAME_LENGTH) {
		health--;
		if (health == 2) sprDefault.setTexture(tHurt1);
		if (health == 1) sprDefault.setTexture(tHurt2);

		iFrames.restart();
		return true;
	}
	else {
		return false;
	}
}


Enemy::Enemy(sf::Vector2f startPos, Player *playerStart) {
	sprDefault.setOrigin(40, 40);

	//Rand spawn pos code, leaving it for future me <3 :***
	/*
	position.x = rand() % 100 - 50;
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
	position = startPos;
	sprDefault.setPosition(startPos);
	directionOffset = nmUtils::RandVector2f();

	player = playerStart;

}

void Enemy::Update() {
	const sf::Vector2f &playerPos = player->sprDefault.getPosition();
	direction = sf::Vector2f(playerPos.x - position.x, playerPos.y - position.y);
	direction = nmUtils::NormaliseVector2f(direction);

	if (wanderDelay.getElapsedTime().asSeconds() > 3) {
		directionOffset = nmUtils::RandVector2f();
		wanderDelay.restart();
	}
	direction += directionOffset;
	position.x += direction.x * ZOMBIE_SPEED;
	position.y += direction.y * ZOMBIE_SPEED;

	LookAt(playerPos);
	sprDefault.setPosition(position.x, position.y);
}

Bullet::Bullet(sf::Vector2f direction, int bulletSpeed) {
	circle.setRadius(2);
	circle.setFillColor(sf::Color::Black);

	velocity = direction * float(bulletSpeed);
}

void Bullet::Update() {
	//Notice: no drag
	position.x += velocity.x * BULLET_SPEED;
	position.y += velocity.y * BULLET_SPEED;
	circle.setPosition(position.x, position.y);
	CheckCollisions();
}

void Bullet::CheckCollisions() {
	//TODO
}