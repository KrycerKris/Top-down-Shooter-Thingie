#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <format>
#include <iostream>

#include "Defines.h"
#include "Objects.h"
#include "Utils.h"

using namespace sf;
using namespace std;

int main()
{
	srand((unsigned)time(nullptr));
	RenderWindow app(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Doom 1996");
	app.setFramerateLimit(60);

	//Load BG Image
	Texture tBackground;
	tBackground.loadFromFile("Resources/bg.png");
	Sprite sprBackground(tBackground);

	//Load enemy textures
	Texture tEnemy;
	tEnemy.loadFromFile("Resources/enmy.png");
	Texture tPlayerEnemy;
	tPlayerEnemy.loadFromFile("Resources/playr_enmy.png");

	//Load numbers
	Texture t1;
	t1.loadFromFile("Resources/1.png");
	Texture t2;
	t2.loadFromFile("Resources/2.png");
	Texture t3;
	t3.loadFromFile("Resources/3.png");
	sf::Sprite counter;
	counter.setTexture(t1);
	counter.setOrigin(32, 32);
	counter.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	//bool PlayedStart = false;


	sf::Text score;


	//Instantiate player
	Player player;
	player.Start();

	//Clock
	sf::Clock bulletDelay;

	//Array for storing all da bullets
	std::vector<Bullet> bulletInstances;

	//Instantiate enemies, give them textures
	std::vector<Enemy> enemyInstances;
	for (int i = 0; i < ZOMBIE_AMOUNT; i++) {
		enemyInstances.emplace_back(i);
		enemyInstances[i].sprDefault.setTexture(tEnemy);
		enemyInstances[i].player = &player;
		enemyInstances[i].Start();

	}

		//Debug shit
		sf::CircleShape circle;
		circle.setRadius(2);
		circle.setOutlineColor(sf::Color::Red);
		circle.setOutlineThickness(5);



	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

		player.Update();


		//Update enemies' target coords and update them
		for (int i = 0; i < enemyInstances.size(); i++) {
			enemyInstances[i].Update();

			if (nmUtils::DistanceBetween(player.sprDefault.getPosition(), enemyInstances[i].sprDefault.getPosition()) < COLLISION_RANGE) {
				if (player.health > 1) {
					player.TakeDamage();
				}
				else if (player.health == 1) {
					if (player.TakeDamage()) {
						enemyInstances.emplace_back();
						enemyInstances[enemyInstances.size() - 1].sprDefault.setTexture(tPlayerEnemy);
						enemyInstances[enemyInstances.size() - 1].player = &player;
						enemyInstances[enemyInstances.size() - 1].Start();
						enemyInstances[enemyInstances.size() - 1].sprDefault.setPosition(player.sprDefault.getPosition());

					}
				}
			}
			for (int j = 0; j < bulletInstances.size(); j++) {
				if (nmUtils::DistanceBetween(bulletInstances[j].circle.getPosition(), enemyInstances[i].sprDefault.getPosition()) < BULLET_COLLISION) {
					enemyInstances.erase(enemyInstances.begin() + i);
					bulletInstances.erase(bulletInstances.begin() + j);
				}
			}
		}

		//Update bullets
		for (int i = 0; i < bulletInstances.size(); i++) {
			bulletInstances[i].Update();
		}

		//Player movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			player.velocity.y -= ACCELERATION;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			player.velocity.y += ACCELERATION;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			player.velocity.x -= ACCELERATION;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			player.velocity.x += ACCELERATION;
		}



		//Make Player Face Mouse
		std::cout << "Mouse X: " << sf::Mouse::getPosition().x << " || Mouse Y: " << sf::Mouse::getPosition().y;
		player.LookAt(sf::Vector2f(sf::Mouse::getPosition(app).x, sf::Mouse::getPosition(app).y));

		//Fire Gun
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && bulletDelay.getElapsedTime().asSeconds() > 2 && player.health > 0) {
			bulletDelay.restart();
			bulletInstances.emplace_back();
			sf::Vector2f temp1 = player.sprDefault.getPosition();
			sf::Vector2f temp2 = (sf::Vector2f)sf::Mouse::getPosition(app);
			sf::Vector2f temp3 = sf::Vector2f(temp2.x - temp1.x, temp2.y - temp1.y);
			bulletInstances[bulletInstances.size()-1].direction = nmUtils::NormaliseVector2f(temp3);
			bulletInstances[bulletInstances.size() - 1].x = temp1.x;
			bulletInstances[bulletInstances.size() - 1].y = temp1.y;
			bulletInstances[bulletInstances.size()-1].Start();
		}



			//Debug shit
			circle.setPosition(player.sprDefault.getPosition());

		//draw bg
		app.draw(sprBackground);

		//draw enemies
		for (int i = 0; i < enemyInstances.size(); i++) {
			app.draw(enemyInstances[i].sprDefault);
		}

		//draw bullets
		for (int i = 0; i < bulletInstances.size(); i++) {
			app.draw(bulletInstances[i].circle);
		}

		//draw player
		app.draw(player.sprDefault);


		//Debug shit
			circle.setPosition(player.sprDefault.getPosition());
			app.draw(circle);

			circle.setPosition(player.sprDefault.getOrigin());
			app.draw(circle);

			circle.setPosition((sf::Vector2f)sf::Mouse::getPosition(app));
			app.draw(circle);
		//if (!PlayedStart) {
		//	//Starting countdown
		//	sf::Clock startTimer;
		//	while (startTimer.getElapsedTime().asSeconds() < 1) {
		//		counter.setScale(counter.getScale() * REDUCTION_RATE);
		//		app.draw(counter);
		//		app.display();
		//	}
		//	counter.setTexture(t2);
		//	while (startTimer.getElapsedTime().asSeconds() < 2) {
		//		counter.setScale(counter.getScale() * REDUCTION_RATE);
		//		app.draw(counter);
		//		app.display();
		//	}
		//	counter.setTexture(t3);
		//	while (startTimer.getElapsedTime().asSeconds() < 3) {
		//		counter.setScale(counter.getScale() * REDUCTION_RATE);
		//		app.draw(counter);
		//		app.display();
		//	}
		//	PlayedStart = true;
		//}
		app.display();
	}



	return 0;
}


