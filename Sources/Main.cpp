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

	//Instantiate player
	Player player;
	player.Start();

	//Instantiate enemies, give them textures
	std::vector<Enemy> enemyInstances;
	for (int i = 0; i < ZOMBIE_AMOUNT; i++) {
		enemyInstances.emplace_back(i);
		enemyInstances[i].sprDefault.setTexture(tEnemy);
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
		for (int i = 0; i < ZOMBIE_AMOUNT; i++) {
			enemyInstances[i].playerPos = player.sprDefault.getPosition();
			enemyInstances[i].Update();

			if (nmUtils::DistanceBetween(enemyInstances[i].playerPos, enemyInstances[i].sprDefault.getPosition()) < COLLISION_RANGE ) {
				player.sprDefault.setTexture(tEnemy);
			}
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

		//Check if player is colliding with any of the enemies



			//Debug shit
			circle.setPosition(player.sprDefault.getPosition());

		//draw bg
		app.draw(sprBackground);

		//draw enemies
		for (int i = 0; i < ZOMBIE_AMOUNT; i++) {
			app.draw(enemyInstances[i].sprDefault);
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


		app.display();
	}



	return 0;
}


