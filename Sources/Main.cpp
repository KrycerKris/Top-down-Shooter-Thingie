#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <format>
#include <iostream>
#include <fstream>

#include "Defines.h"
#include "Objects.h"
#include "Utils.h"
#include "Managers.h"

using namespace sf;
using namespace std;

int main()
{
	srand((unsigned)time(nullptr));
	RenderWindow app(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "World War Z-1");
	app.setFramerateLimit(60);
	app.setMouseCursorVisible(false);

	//Load BG Image
	Texture tBackground;
	tBackground.loadFromFile("Resources/bg.png");
	Sprite sprBackground(tBackground);

	//Load enemy textures
	Texture tEnemy;
	tEnemy.loadFromFile("Resources/enmy.png");
	Texture tPlayerEnemy;
	tPlayerEnemy.loadFromFile("Resources/playr_enmy.png");

	//Load font
	sf::Font font;
	font.loadFromFile("Resources/arialbd.ttf");


	//Load numbers
	Texture t1;
	t1.loadFromFile("Resources/1.png");
	Texture t2;
	t2.loadFromFile("Resources/2.png");
	Texture t3;
	t3.loadFromFile("Resources/3.png");
	sf::Sprite counter;	

	//|-----------------------------|
	//|PLAYER/ENEMY SHIT STARTS HERE|
	//|-----------------------------|

	//Renderables array, gonna store all entities(?) that want to be rendered
	// pair<Renderable object, Render priority>
	//Priority will render from 0 -> inf, so bigger the value the more on top it will render
	vector<pair<int, Renderable*>> renderables;
	//We gotta update shit too
	vector<Entity*> updateables;

	//Instantiate player
	Player player(Vector2f(480, 480));
	renderables.push_back({0, &player});
	updateables.push_back(&player);

	//Clocks
	sf::Clock bulletDelay;

	//Zombie spawn delay
	sf::Clock zombieSpawnDelay;
	int zombieSpawnDelayRANDOFFSET = rand() % 4;

	//Array for storing all da bullets and enemies
	std::vector<Bullet*> bulletInstances;

	//Instantiate enemies, give them textures
	//PLEASE PLEASE PLEASE PLEASE PLEASE PLEASE PLEASE CHECK FOR MEMORY LEAKS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	std::vector<Enemy*> enemyInstances(ZOMBIE_AMOUNT);
	for (int i = 0; i < ZOMBIE_AMOUNT; i++) {
		sf::Vector2f randSpawn = nmUtils::RandSpawn();
		cout << randSpawn.x << " " << randSpawn.y;
		enemyInstances[i] = new Enemy(nmUtils::RandSpawn(), &player);
		enemyInstances[i]->sprDefault.setTexture(tEnemy);
		updateables.push_back(enemyInstances[i]);
		renderables.push_back({ 2, enemyInstances[i] });
	}

	//CREATE THE MANAGER
	colManager.Set(&player, &enemyInstances, &bulletInstances);

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
			{
				app.close();
			}
		}

		//Player movement
		Vector2f playerVelCurrent = player.GetVel();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			playerVelCurrent = sf::Vector2f(playerVelCurrent.x, playerVelCurrent.y - ACCELERATION);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			playerVelCurrent = sf::Vector2f(playerVelCurrent.x, playerVelCurrent.y + ACCELERATION);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			playerVelCurrent = sf::Vector2f(playerVelCurrent.x - ACCELERATION, playerVelCurrent.y);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			playerVelCurrent = sf::Vector2f(playerVelCurrent.x + ACCELERATION, playerVelCurrent.y);
		}
		player.SetVel(playerVelCurrent);

		//Update the updatables
		for (auto entity : updateables) {
			entity->Update();
		}

		//Make Player Face Mouse
		std::cout << "Mouse X: " << sf::Mouse::getPosition().x << " || Mouse Y: " << sf::Mouse::getPosition().y;
		player.LookAt(sf::Vector2f(sf::Mouse::getPosition(app).x, sf::Mouse::getPosition(app).y));


		//draw bg
		app.draw(sprBackground);


		for (auto& rendered : renderables) {
			app.draw(rendered.second->sprDefault);
		}

		//Old drawing code, to be edited
		/*
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

		*/
			//Debug shit
			circle.setPosition((sf::Vector2f)sf::Mouse::getPosition(app));
			app.draw(circle);

		app.display();
	}



	return 0;
}