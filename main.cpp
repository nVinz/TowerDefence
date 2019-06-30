#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include "Entity.h"
#include "Point.h"
#include "Entity_Bullet.h"
#include "MyCommand.h"
//#include "MyEnemyCommand.h"
#include "MyFactory.h"
#include "Menu.h"
using namespace sf; using namespace std;

int main()
{
	//  WINDOW  //
	RenderWindow window(VideoMode(1216, 960), "Tower Defence"); window.setFramerateLimit(60);
	Image icon; icon.loadFromFile("resources/icon.png"); window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	//menu(window);

	///  VIEW  ///
	View view;
	view.setSize(1281, 1024);
	view.move(140, 12);


	///  VARS  ///
	bool placed = false, firstEnemySpawned = false;
	int money = 80, wave_count = 1;
	bool wave_counter[10] = { false, false, false, false, false, false, false, false, false, false };

	enum { tower1, tower2 } tower_state;
	enum { enemy1, enemy2 } enemy_state;

	/// TEXTURES, SPRITES, FONTS  ///
	Texture start_t; start_t.loadFromFile("images/buttons/game_start.png");
	Sprite start_s; start_s.setTexture(start_t); start_s.setPosition(1042, 18);

	Texture menu1_t; menu1_t.loadFromFile("images/button_template3.png");
	Sprite menu1_s; menu1_s.setTexture(menu1_t); menu1_s.setPosition(1042, 82);

	Texture menu2_t; menu2_t.loadFromFile("images/button_template2.png");
	Sprite menu2_s; menu2_s.setTexture(menu2_t); menu2_s.setPosition(1042, 189);

	Texture menu3_t; menu3_t.loadFromFile("images/button_template2.png");
	Sprite menu3_s; menu3_s.setTexture(menu3_t); menu3_s.setPosition(1042, 407);

	Texture money_t; money_t.loadFromFile("images/textures/money.png");
	Sprite money_s; money_s.setTexture(money_t); money_s.setPosition(1048, 100);

	Texture trash_t; trash_t.loadFromFile("images/buttons/trash.png");
	Sprite trash_s; trash_s.setTexture(trash_t); trash_s.setPosition(1048, 800);

	///  TOWERS  ///
	CircleShape range; range.setRadius(140); range.setFillColor(sf::Color(255, 255, 255, 6));
	range.setOutlineThickness(3); range.setOutlineColor(sf::Color(153, 255, 153, 40));

	///  TOWER1  ///
	Texture tower1_t; tower1_t.loadFromFile("images/buttons/tower1.png");
	Sprite tower1_s; tower1_s.setTexture(tower1_t); tower1_s.setPosition(1048, 200);
	Sprite tower1_s2; tower1_s2.setTexture(tower1_t); tower1_s2.setPosition(1048, 200);
	Texture tower1_i_t; tower1_i_t.loadFromFile("images/buttons/tower1_info.png");
	Sprite tower1_i_s; tower1_i_s.setTexture(tower1_i_t); tower1_i_s.setPosition(1042, 407);
	const int tower1_cost = 10;

	///  TOWER2  ///
	Texture tower2_t; tower2_t.loadFromFile("images/buttons/tower2.png");
	Sprite tower2_s; tower2_s.setTexture(tower2_t); tower2_s.setPosition(1120, 200);
	Sprite tower2_s2; tower2_s2.setTexture(tower2_t); tower2_s2.setPosition(1048, 200);
	Texture tower2_i_t; tower2_i_t.loadFromFile("images/buttons/tower2_info.png");
	Sprite tower2_i_s; tower2_i_s.setTexture(tower2_i_t); tower2_i_s.setPosition(1042, 407);
	const int tower2_cost = 30;

	Texture wrench_t; wrench_t.loadFromFile("images/buttons/game_wrench.png");
	Sprite wrench_s; wrench_s.setTexture(wrench_t); wrench_s.setPosition(1008, 120);

	Texture undo_t; undo_t.loadFromFile("images/buttons/game_undo.png");
	Sprite undo_s; undo_s.setTexture(undo_t); //undo_s.setPosition(1008, 192); undo_s.setColor(Color(180, 180, 180));


	Texture stop_t; stop_t.loadFromFile("images/buttons/game_stop.png");
	Sprite stop_s; stop_s.setTexture(stop_t); stop_s.setPosition(640, 8);

	Font font; font.loadFromFile("resources/font.ttf");

	Text money_text(to_wstring(money), font, 54);
	money_text.setPosition(1108, 100);

	Text tower1_text("Tower 1", font, 54);
	tower1_text.setString("Tower 1");
	tower1_text.setPosition(1108, 200);
	//  //  //

	//  LEVEL  //
	Level lvl; lvl.LoadFromFile("resources/map.tmx");
	Object menu_obj = lvl.GetObject("menu");
	//  //  //

	//  POINTS  //
	Object start_point_o = lvl.GetObject("startPoint");
	StartPoint start_point(start_point_o.rect.left, start_point_o.rect.top, 32, 64, 0);
	Object end_point = lvl.GetObject("endPoint");
	vector<Object> places = lvl.GetObjects("place"); vector<Object>::iterator places_it;
	//  //  //

	//  TOWERS  //
	Image tower1_image; tower1_image.loadFromFile("images/textures/tower1.png");
	Image tower2_image; tower2_image.loadFromFile("images/textures/tower2.png");
	vector<Object> ts = lvl.GetObjects("tower");
	bool *towerNotStaying = new bool[places.size()]; float rotation; int towerCounter = 0;
	bool building = false;
	//  //  //

	//  ENEMIES  //
	Image enemy1_image; enemy1_image.loadFromFile("images/textures/enemy1.png");
	Image enemy2_image; enemy2_image.loadFromFile("images/textures/enemy2.png");
	int enemy_counter = 0;
	//  //  //

	//  BULLETS  //
	Image bullet_image; bullet_image.loadFromFile("images/textures/bullet.png");
	list<Bullet*> bullet_entities; list<Bullet*>::iterator b_it;
	int bullet_counter = 0;
	//  //  //

	//  EXPLOSIONS  //
	Image explosion_image; explosion_image.loadFromFile("images/textures/explosion.png");
	list<Explosion*> explosion_entities; list<Bullet*>::iterator e_it;
	//  //  //

	//  COMMAND  //
	//TowerList towers;
	MyCommandManager myCommandManager;
	Start start; start.state = false;
	std::shared_ptr<MyCommand> start_command(new StartCommand(start));
	bool undo_flag = true;

	std::shared_ptr<MyCommand> remove_tower;
	//  //  //

	//  FACTORY  //
	EnemyList enemies;
	//MyEnemyCommandManager myEnemyCommandManager;
	AbstractEnemyFactory* myEnemyFactory = new EnemyFactory;

	TowerList towers;
	AbstractTowerFactory * myTowerFactory = new TowerFactory;
	//  //  //

	Clock clock_main, clock_enemies, clock_shoots, clock_wave;
	Time time_enemies, time_shoots, ttime_wave; Time delayTime = seconds(1.0);
	while (window.isOpen())
	{
		//  WINDOW VARS  //
		float time_main = clock_main.getElapsedTime().asMicroseconds(); time_main = time_main / 800;
		time_enemies = clock_enemies.getElapsedTime();
		time_shoots = clock_shoots.getElapsedTime();
		ttime_wave = clock_wave.getElapsedTime();
		clock_main.restart();

		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);
		Event event;

		//  //  //

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || ( Keyboard::isKeyPressed(Keyboard::Escape) && start.state == false))
				window.close();
		}

		window.setView(view);
		window.clear();
		lvl.Draw(window);

		//  GUI  //
		if (building == false) 
		{
			tower1_s.setColor(Color(255, 255, 255));
			tower2_s.setColor(Color(255, 255, 255));
		}
		start_s.setColor(Color(255, 255, 255));
		stop_s.setColor(Color(255, 255, 255));
		if (undo_flag == false || towerCounter == 0) { undo_s.setColor(Color(180, 180, 180)); } else { undo_s.setColor(Color(255, 255, 255)); }

		if (tower1_s.getGlobalBounds().contains(pos.x, pos.y)) { tower1_s.setColor(Color(180, 180, 180)); }
		if (tower2_s.getGlobalBounds().contains(pos.x, pos.y)) { tower2_s.setColor(Color(180, 180, 180)); }
		if (undo_s.getGlobalBounds().contains(pos.x, pos.y)) { undo_s.setColor(Color(180, 180, 180)); }
		if (start_s.getGlobalBounds().contains(pos.x, pos.y)) { start_s.setColor(Color(180, 180, 180)); }
		if (stop_s.getGlobalBounds().contains(pos.x, pos.y)) { stop_s.setColor(Color(180, 180, 180)); }
		//  //  //

		//  TOWERS  //
		if (placed)
		{
			for (towers.iterator = towers.tlist.begin(); towers.iterator != towers.tlist.end(); towers.iterator++)
			{
				for (enemies.iterator = enemies.elist.begin(); enemies.iterator != enemies.elist.end(); enemies.iterator++)
				{
					Enemy *e = *enemies.iterator;
					if ((*enemies.iterator)->sprite.getGlobalBounds().intersects((*towers.iterator)->range.getGlobalBounds()))
					{
						///  ROTATION  ///
						rotation = (atan2((*enemies.iterator)->y - (*towers.iterator)->y, (*enemies.iterator)->x - (*towers.iterator)->x)) * 180 / 3.14159265;
						(*towers.iterator)->sprite.setRotation(rotation);
						
						///  BULLET SPAWNING  ///
						if (time_shoots.asSeconds() >= delayTime.asSeconds())
						{
							bullet_entities.push_back(new Bullet(bullet_image, "bullet", lvl, (*towers.iterator)->getX(), (*towers.iterator)->getY() + 16, 16, 16, rotation, bullet_counter, (*enemies.iterator), (*towers.iterator)->damage));
							clock_shoots.restart(); bullet_counter++;
						}
					}
				}

				///  UPDATING, DRAWING  ///
				(*towers.iterator)->update(time_main);
				window.draw((*towers.iterator)->place_s);
				window.draw((*towers.iterator)->sprite);

				if ((*towers.iterator)->sprite.getGlobalBounds().contains(pos.x, pos.y))
				{
					window.draw((*towers.iterator)->range);
					//window.draw(tower1_i_s);
				}
			}
		}

		///  PLACING  ///
		if (tower1_s.getGlobalBounds().contains(pos.x, pos.y))
		{
			window.draw(tower1_i_s);
			if(Mouse::isButtonPressed(Mouse::Left)) tower_state = tower1;
		}
		else if (tower2_s.getGlobalBounds().contains(pos.x, pos.y))
		{
			window.draw(tower2_i_s);
			if(Mouse::isButtonPressed(Mouse::Left)) tower_state = tower2;
		}
		else
		{
			window.draw(menu3_s);
		}

		if ((Mouse::isButtonPressed(Mouse::Left) && tower1_s.getGlobalBounds().contains(pos.x, pos.y)) && money >= tower1_cost)
		{
			tower1_s.setColor(Color(180, 180, 180));
			building = true;
		}
		else if ((Mouse::isButtonPressed(Mouse::Left) && tower2_s.getGlobalBounds().contains(pos.x, pos.y)) && money >= tower2_cost)
		{
			tower2_s.setColor(Color(180, 180, 180));
			building = true;
		}

		

		if (building == true)
		{
			if (Mouse::isButtonPressed(Mouse::Left) && trash_s.getGlobalBounds().contains(pos.x, pos.y))
			{
				building = false;
			}
			for (towers.iterator = towers.tlist.begin(); towers.iterator != towers.tlist.end(); towers.iterator++) window.draw((*towers.iterator)->range);

			range.setPosition(pos.x - 138, pos.y - 134);

			if (tower_state == tower1)
			{
				tower1_s2.setColor(Color(255, 128, 128));
				tower1_s2.setPosition(pos.x - 32, pos.y - 32);
				range.setOutlineColor(sf::Color(255, 128, 128, 40));

				for (places_it = places.begin(); places_it != places.end(); places_it++)
				{
					if (places_it->rect.contains(pos.x, pos.y))
					{
						for (int i = 0; i < places.size(); i++)
						{
							if (towerNotStaying[i] != false && places[i].rect.contains(pos.x, pos.y))
							{
								tower1_s2.setColor(Color(128, 255, 128));
								range.setOutlineColor(sf::Color(153, 255, 153, 40));
							}
						}
					}
				}
				window.draw(tower1_s2);
				window.draw(range);
			}
			else if (tower_state == tower2)
			{
				tower2_s2.setColor(Color(255, 128, 128));
				tower2_s2.setPosition(pos.x - 32, pos.y - 32);
				range.setOutlineColor(sf::Color(255, 128, 128, 40));

				for (places_it = places.begin(); places_it != places.end(); places_it++)
				{
					if (places_it->rect.contains(pos.x, pos.y))
					{
						for (int i = 0; i < places.size(); i++)
						{
							if (towerNotStaying[i] != false && places[i].rect.contains(pos.x, pos.y))
							{
								tower2_s2.setColor(Color(128, 255, 128));
								range.setOutlineColor(sf::Color(153, 255, 153, 40));
							}
						}
					}
				}
				window.draw(tower2_s2);
				window.draw(range);
			}
		}

		if (Mouse::isButtonPressed(Mouse::Left) && building == true)
		{
			for (places_it = places.begin(); places_it != places.end(); places_it++)
			{
				if (places_it->rect.contains(pos.x, pos.y))
				{
					for (int i = 0; i < places.size(); i++)
					{
						if (towerNotStaying[i] != false && places[i].rect.contains(pos.x, pos.y) && money >= tower1_cost)
						{
							if (tower_state == tower1)
							{
								myTowerFactory->createTower1(towers, tower1_image, lvl, places_it->rect.left, places_it->rect.top, 32, 32, towerCounter);
								money -= tower1_cost;
							}
							else if (tower_state == tower2)
							{
								myTowerFactory->createTower2(towers, tower2_image, lvl, places_it->rect.left, places_it->rect.top, 32, 32, towerCounter);
								money -= tower1_cost + 20;
							}
							//std::shared_ptr<MyCommand> place_tower2Command(new PlaceTowerCommand(towers, tower_image, lvl, places_it->rect.left, places_it->rect.top, 32, 32, towerCounter, "tower1"));
							//myCommandManager.execute(place_tower2Command);
							//remove_tower = place_tower1Command;

							money_text.setString(to_wstring(money));
							towerCounter++; placed = true; undo_flag = true; building = false; towerNotStaying[i] = false;
						}
					}
				}
			}
		}

		///  REMOVING  ///
		if ((Mouse::isButtonPressed(Mouse::Left) && undo_s.getGlobalBounds().contains(pos.x, pos.y)) && undo_flag == true)
		{
			undo_flag = false;
			for (int i = 0; i < towers.tlist.size(); i++)
			{
				if (towers.tlist.size() == towerCounter)
				{
					//myCommandManager.undo(remove_tower);

					towerCounter--;
					money += tower1_cost; money_text.setString(to_wstring(money));

					for (int j = 0; j < ts.size(); j++) { towerNotStaying[j] = true; }

					break;
				}
			}
		}
		//  //  //


		//  START  //
		if ((Mouse::isButtonPressed(Mouse::Left) && start_s.getGlobalBounds().contains(pos.x, pos.y)) && start.state == false)
		{
			cout << "START" << endl;

			clock_wave.restart();

			myCommandManager.execute(start_command);

		}
		if (start.state == true)
		{
			window.draw(stop_s);
			///  ENEMY SPAWNING  ///
			if (time_enemies.asSeconds() >= delayTime.asSeconds() && enemy_counter < 4 * wave_count)
			{
				if (wave_count % 2 != 0)
				{
					myEnemyFactory->createEnemy1(enemies, enemy1_image, lvl, start_point_o.rect.left, start_point_o.rect.top - 32, 64, 64, enemy_counter);
					enemy_state = enemy1;
				}
				else
				{
					myEnemyFactory->createEnemy2(enemies, enemy2_image, lvl, start_point_o.rect.left, start_point_o.rect.top - 32, 64, 64, enemy_counter);
					enemy_state = enemy2;
				}

				clock_enemies.restart();
				enemy_counter++; firstEnemySpawned = true;
			}

			///  ENEMY UPDATING  ///
			for (enemies.iterator = enemies.elist.begin(); enemies.iterator != enemies.elist.end();)
			{
				Enemy *e = *enemies.iterator;
				e->update(time_main);
				if (e->life == false)
				{
					cout << "Enemy #" << e->id << " dead" << endl;
					enemies.iterator = enemies.elist.erase(enemies.iterator); delete e; money += 10;
					money_text.setString(to_wstring(money));
				}
				else enemies.iterator++;
			}
			for (enemies.iterator = enemies.elist.begin(); enemies.iterator != enemies.elist.end(); enemies.iterator++)
			{
				(*enemies.iterator)->update(time_main);
				window.draw((*enemies.iterator)->sprite);
			}


			///  BULLET UPDATING  ///
			for (b_it = bullet_entities.begin(); b_it != bullet_entities.end();)
			{
				Bullet *b = *b_it;
				b->update(time_main);
				if (b->life == false)
				{
					cout << "Bullet #" << b->id << ": dead" << endl;
					//explosion_entities.push_back(new Explosion(explosion_image, "explosion", lvl, (*b_it)->x, (*b_it)->y, 16, 16, bullet_counter));
					b_it = bullet_entities.erase(b_it); delete b;
				}
				else b_it++;
			}
			for (b_it = bullet_entities.begin(); b_it != bullet_entities.end(); b_it++)
			{
				(*b_it)->update(time_main);
				window.draw((*b_it)->sprite);
			}

			///  WAVE END  ///
			if (enemies.elist.empty() && firstEnemySpawned == true)
			{
				cout << "WAVE ENDED #" << wave_count << endl;
				cout << "Wave time: " << ttime_wave.asSeconds() << " seconds" << endl;

				firstEnemySpawned = false; wave_counter[wave_count - 1] = true; 
				wave_count++; enemy_counter = 0;

				//myCommandManager.undo(start_command);
				start.state = false;
			}

		}
		//  //  //


		//  STOP  //
		if ((Mouse::isButtonPressed(Mouse::Left) && stop_s.getGlobalBounds().contains(pos.x, pos.y)) && start.state == true)
		{
			cout << "STOP" << endl;

			myCommandManager.undo(start_command);  //start.state = false;

			window.draw(start_s);
			clock_enemies.restart();
			clock_main.restart();
			clock_shoots.restart();
		}
		if (!start.state)
		{
			for (b_it = bullet_entities.begin(); b_it != bullet_entities.end(); b_it++) { window.draw((*b_it)->sprite); }
			for (enemies.iterator = enemies.elist.begin(); enemies.iterator != enemies.elist.end(); enemies.iterator++)
				{ window.draw((*enemies.iterator)->sprite); }

			window.draw(start_s);

			clock_enemies.restart();
			clock_main.restart();
			clock_shoots.restart();
		}
		//  //  //


		window.draw(trash_s);

		window.draw(menu1_s);
		window.draw(menu2_s);

		window.draw(tower1_s);
		window.draw(tower2_s);
		//window.draw(wrench_s);
		//window.draw(undo_s);
		window.draw(money_s);
		window.draw(money_text);
		window.display();
	}

	return 0;
}