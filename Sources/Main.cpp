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



	BulletManager bulletMangr;
	EnemyManager enemyMangr;

	vector<Enemy*> *enemyList = enemyMangr.GetEnemyVector();
	vector<Bullet*> *bulletList = bulletMangr.GetBulletVector();

	Enemy::manager = &enemyMangr;
	Bullet::manager = &bulletMangr;
	//|-----------------------------|
	//|PLAYER/ENEMY SHIT STARTS HERE|
	//|-----------------------------|

	//Renderables array, gonna store all entities(?) that want to be rendered
	// pair<Renderable object, Render priority>
	//Priority will render from 0 -> inf, so bigger the value the more on top it will render
	//vector<pair<int, Renderable*>> renderables;
	//We gotta update shit too
	//vector<Entity*> updateables;



	//Instantiate player
	Player player(Vector2f(480, 480));
	//renderables.push_back({0, &player});
	//updateables.push_back(&player);
	player.currentGun = new Gun("Cummy", 6, 6, 0.5f, 2.f);

	//Zombie spawn delay
	sf::Clock zombieSpawnDelay;
	int zombieSpawnDelayRANDOFFSET = rand() % 4;

	//Instantiate enemies, give them textures
	for (int i = 0; i < ZOMBIE_AMOUNT; i++) {
		sf::Vector2f randSpawn = nmUtils::RandSpawn();
		cout << randSpawn.x << " " << randSpawn.y;
		enemyMangr.SpawnEnemy(randSpawn, &player);
	}

	//CREATE THE MANAGER
	colManager.Set(&player, enemyList, bulletList);

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

		//Techniskai turetu but PlayerController klasej, but nera pakankai input kad justifyint development time :^)
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

		//SHOOT STYLE
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && player.currentGun != nullptr) {
			sf::Vector2f aimDirection = nmUtils::NormaliseVector2f(
				nmUtils::VectorBetweenPoints(player.GetPos(), (sf::Vector2f)sf::Mouse::getPosition(app))
			);
				bulletMangr.SpawnBullet(player.currentGun->Fire(aimDirection));
		}

		//Make Player Face Mouse
		std::cout << "Mouse X: " << sf::Mouse::getPosition().x << " || Mouse Y: " << sf::Mouse::getPosition().y;
		player.LookAt(sf::Vector2f(sf::Mouse::getPosition(app).x, sf::Mouse::getPosition(app).y));


		//Update the updatables
		player.Update();

		for (auto& entity : *bulletList) {
			entity->Update();
		}

		for (auto& entity : *enemyList) {
			entity->Update();
		}

		enemyMangr.DeleteQueue();
		bulletMangr.DeleteQueue();

		//draw bg
		app.draw(sprBackground);

		app.draw(player.sprDefault);
		for (auto& entity : *bulletList) {
			app.draw(entity->sprDefault);
		}
		for (auto& entity : *enemyList) {
			app.draw(entity->sprDefault);
		}

			//Debug shit
			circle.setPosition((sf::Vector2f)sf::Mouse::getPosition(app));
			app.draw(circle);

		app.display();
	}



	return 0;
}