#include "Objects.h"
#include "Utils.h"

void Player::LookAt(sf::Vector2f target) {

	sf::Vector2f target2 = sprDefault.getPosition();
	sprDefault.setRotation(nmUtils::AngleBetween(target2, target));
}

void Player::Start() {
	tDefault.loadFromFile("Resources/playr.png");
	sprDefault.setTexture(tDefault);
	sprDefault.setOrigin(40, 40);

	//x = sprDefault.getPosition().x;
	//y = sprDefault.getPosition().y;
	x = WINDOW_WIDTH / 2;
	y = WINDOW_HEIGHT / 2;
	sprDefault.setPosition(x, y);
}

void Player::Update() {
	if (y > WINDOW_HEIGHT && velocity.y > 0) {
		velocity.y = 0;
	}
	if (y < 0 && velocity.y < 0) {
		velocity.y = 0;
	}
	if (x > WINDOW_HEIGHT && velocity.x > 0) {
		velocity.x = 0;
	}
	if (x < 0 && velocity.x < 0) {
		velocity.x = 0;
	}

	
	x += velocity.x;
	y += velocity.y;

	velocity *= DRAG;



	sprDefault.setPosition(x, y);
}
void Enemy::LookAt(sf::Vector2f target) {

	sf::Vector2f target2 = sprDefault.getPosition();
	sprDefault.setRotation(nmUtils::AngleBetween(target2, target));
}

void Enemy::Start() {
		sprDefault.setOrigin(40, 40);

		x = rand() % WINDOW_WIDTH;
		y = rand() % WINDOW_HEIGHT;
		sprDefault.setPosition(x, y);
		directionOffset = nmUtils::RandVector2f();
}

void Enemy::Update() {
	const sf::Vector2f &playerPos = player->sprDefault.getPosition();
	direction = sf::Vector2f(playerPos.x - x, playerPos.y - y);
	direction = nmUtils::NormaliseVector2f(direction);

	if (wanderDelay.getElapsedTime().asSeconds() > 3) {
		directionOffset = nmUtils::RandVector2f();
		wanderDelay.restart();
	}
	direction += directionOffset;
	x += direction.x * ZOMBIE_SPEED;
	y += direction.y * ZOMBIE_SPEED;

	LookAt(playerPos);
	sprDefault.setPosition(x, y);
}

void Bullet::Start() {
	circle.setRadius(2);
	circle.setFillColor(sf::Color::Black);
}

void Bullet::Update() {
	x += direction.x * BULLET_SPEED;
	y += direction.y * BULLET_SPEED;
	circle.setPosition(x, y);
}