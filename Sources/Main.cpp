#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <format>
#include <iostream>
#include <fstream>

#include "Defines.h"
#include "Objects.h"
#include "Utils.h"

using namespace sf;
using namespace std;

int main()
{
	srand((unsigned)time(nullptr));
	RenderWindow app(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "World War Z-1");
	app.setFramerateLimit(60);
	app.setMouseCursorVisible(false);

	//Load Highscore
	int highscore=0;
	{
		ifstream highscoreFile("highscore.txt");
		if (!(highscoreFile >> highscore)) {
			highscore = 0;
		}
		highscoreFile.close();
	}

	ofstream highscoreFile("highscore.txt");


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
	counter.setTexture(t3);
	counter.setOrigin(32, 32);
	counter.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	bool PlayedStart = false;
	
	//Add score
	sf::Text scoreLabel;
	scoreLabel.setPosition(5, 0);
	scoreLabel.setFont(font);
	scoreLabel.setCharacterSize(36);
	scoreLabel.setFillColor(sf::Color::White);
	scoreLabel.setOutlineColor(sf::Color::Black);
	scoreLabel.setOutlineThickness(5);
	scoreLabel.setString("Score: ");

	sf::Text scoreText;
	int score = 0;
	scoreText.setPosition(125, 0);
	scoreText.setFont(font);
	scoreText.setCharacterSize(36);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setOutlineColor(sf::Color::Black);
	scoreText.setOutlineThickness(5);

	sf::Text highScoreLabel;
	highScoreLabel.setPosition(5, 40);
	highScoreLabel.setFont(font);
	highScoreLabel.setCharacterSize(20);
	highScoreLabel.setFillColor(sf::Color::White);
	highScoreLabel.setOutlineColor(sf::Color::Black);
	highScoreLabel.setOutlineThickness(3);
	highScoreLabel.setString("High Score: ");

	sf::Text highScoreText;
	highScoreText.setPosition(125, 40);
	highScoreText.setFont(font);
	highScoreText.setCharacterSize(20);
	highScoreText.setFillColor(sf::Color::White);
	highScoreText.setOutlineColor(sf::Color::Black);
	highScoreText.setOutlineThickness(3);
	highScoreText.setString(sf::String(to_string(highscore)));

	sf::RectangleShape blackening;
	blackening.setSize(sf::Vector2((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
	blackening.setFillColor(sf::Color(0,0,0,0));

	sf::Text YouDied;
	YouDied.setPosition(130, 280);
	YouDied.setFont(font);
	YouDied.setCharacterSize(50);
	YouDied.setFillColor(sf::Color::Transparent);
	YouDied.setOutlineColor(sf::Color::Transparent);
	YouDied.setOutlineThickness(7);
	YouDied.setString("You have perished.");

	sf::Text NewHighScore;
	NewHighScore.setPosition(175, 350);
	NewHighScore.setFont(font);
	NewHighScore.setCharacterSize(20);
	NewHighScore.setFillColor(sf::Color::Transparent);
	NewHighScore.setOutlineColor(sf::Color::Transparent);
	NewHighScore.setOutlineThickness(4);
	NewHighScore.setString("You did get a new high score though!");

	//Instantiate player
	Player player;
	player.Start();

	//Clocks
	sf::Clock bulletDelay;

	//Zombie spawn delay
	sf::Clock zombieSpawnDelay;
	int zombieSpawnDelayRANDOFFSET = rand() % 4;

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
			{
				app.close();
				highscoreFile << highscore;
				highscoreFile.close();
			}
		}

		player.Update();

		//Update enemies' target coords and update them
		for (int i = 0; i < enemyInstances.size(); i++) {
			enemyInstances[i].Update();

			//Collision
			if (nmUtils::DistanceBetween(player.sprDefault.getPosition(), enemyInstances[i].sprDefault.getPosition()) < COLLISION_RANGE) {
				if (player.health > 1) {
					player.TakeDamage();
				}
				else if (player.health == 1) {
					//Death
					if (player.TakeDamage()) {
						enemyInstances.emplace_back();
						enemyInstances[enemyInstances.size() - 1].sprDefault.setTexture(tPlayerEnemy);
						enemyInstances[enemyInstances.size() - 1].player = &player;
						enemyInstances[enemyInstances.size() - 1].Start();
						enemyInstances[enemyInstances.size() - 1].sprDefault.setPosition(player.sprDefault.getPosition());
						enemyInstances[enemyInstances.size() - 1].x = player.sprDefault.getPosition().x;
						enemyInstances[enemyInstances.size() - 1].y = player.sprDefault.getPosition().y;
						if (score > highscore) {
							highscoreFile << score;
							NewHighScore.setFillColor(sf::Color::Yellow);
							NewHighScore.setOutlineColor(sf::Color(150, 0, 0, 255));
						}
						highscoreFile.close();
						YouDied.setFillColor(sf::Color::Red);
						YouDied.setOutlineColor(sf::Color(150, 0, 0, 255));
						blackening.setFillColor(sf::Color(0, 0, 0, 120));
						player.x = rand() % 300 - 150;
						player.y = rand() % 300 - 150;
						if (player.x > 0) player.x += WINDOW_WIDTH;
						if (player.y > 0) player.y += WINDOW_HEIGHT;
						highScoreText.setString(sf::String(to_string(score)));
					}
				}
			}
			//Kill enemies w/ bullets
			for (int j = 0; j < bulletInstances.size(); j++) {
				if (nmUtils::DistanceBetween(bulletInstances[j].circle.getPosition(), enemyInstances[i].sprDefault.getPosition()) < BULLET_COLLISION) {
					enemyInstances.erase(enemyInstances.begin() + i);
					bulletInstances.erase(bulletInstances.begin() + j);
					zombieSpawnDelay.restart();
					score += 10;
				}
			}
		}

		//Update bullets
		for (int i = 0; i < bulletInstances.size(); i++) {
			bulletInstances[i].Update();
		}

		//Player movement
		if (player.health != 0) {
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
		}


		//Make Player Face Mouse
		std::cout << "Mouse X: " << sf::Mouse::getPosition().x << " || Mouse Y: " << sf::Mouse::getPosition().y;
		player.LookAt(sf::Vector2f(sf::Mouse::getPosition(app).x, sf::Mouse::getPosition(app).y));

		//Fire Gun
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && bulletDelay.getElapsedTime().asSeconds() > 1.5 && player.health > 0) {
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


		//Update text
		scoreText.setString(sf::String(to_string(score)));
		if (score >= 100) {
			scoreText.setFillColor(sf::Color::Yellow);
			scoreText.setOutlineColor(sf::Color(255, 165, 0, 255));
			scoreText.setOutlineThickness(5);
		}
		if (score >= 250) {
			scoreText.setFillColor(sf::Color(255, 165, 0, 255));
			scoreText.setOutlineColor(sf::Color::Red);
			scoreText.setStyle(sf::Text::Bold);
			scoreText.setOutlineThickness(5);
		}
		if (score >= 1000) {
			scoreText.setFillColor(sf::Color::Red);
			scoreText.setOutlineColor(sf::Color::Black);
			scoreText.setOutlineThickness(5);
		}

		//Respawn enemies
		if (enemyInstances.size() < ZOMBIE_AMOUNT && zombieSpawnDelay.getElapsedTime().asSeconds() > 3 - (ZOMBIE_AMOUNT - enemyInstances.size())) {
			enemyInstances.emplace_back();
			enemyInstances[enemyInstances.size() - 1].sprDefault.setTexture(tEnemy);
			enemyInstances[enemyInstances.size() - 1].player = &player;
			enemyInstances[enemyInstances.size() - 1].Start();
			zombieSpawnDelay.restart();
			zombieSpawnDelayRANDOFFSET = rand() % 4;
		}

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

		app.draw(scoreLabel);
		app.draw(scoreText);
		app.draw(highScoreLabel);
		app.draw(highScoreText);
		app.draw(blackening);
		app.draw(YouDied);
		app.draw(NewHighScore);

			//Debug shit

			circle.setPosition((sf::Vector2f)sf::Mouse::getPosition(app));
			app.draw(circle);

		if (!PlayedStart) {
			//Starting countdown
			sf::Clock startTimer;
			while (startTimer.getElapsedTime().asSeconds() < 1) {
				//draw enemies
				app.draw(sprBackground);
				for (int i = 0; i < enemyInstances.size(); i++) {
					app.draw(enemyInstances[i].sprDefault);
				}
				app.draw(player.sprDefault);
				app.draw(counter);
				app.display();
			}
			counter.setTexture(t2);
			while (startTimer.getElapsedTime().asSeconds() < 2) {
				app.draw(sprBackground);
				//draw enemies
				for (int i = 0; i < enemyInstances.size(); i++) {
					app.draw(enemyInstances[i].sprDefault);
				}
				app.draw(player.sprDefault);
				app.draw(counter);
				app.display();
			}
			counter.setTexture(t1);
			while (startTimer.getElapsedTime().asSeconds() < 3) {
				app.draw(sprBackground);
				//draw enemies
				for (int i = 0; i < enemyInstances.size(); i++) {
					app.draw(enemyInstances[i].sprDefault);
				}
				app.draw(player.sprDefault);
				app.draw(counter);
				app.display();
			}
			PlayedStart = true;
		}
		app.display();
	}



	return 0;
}