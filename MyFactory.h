#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <stack>
#include <memory>
#include <list>
#include "MyCommand.h"
#include "Entity.h"
using namespace std;

class AbstractEnemyFactory
{
public:
	virtual void createEnemy1(EnemyList &my_enemy_list, Image enemy_image, Level lvl, float X, float Y, int W, int H, int Id) = 0;
	virtual void createEnemy2(EnemyList &my_enemy_list, Image enemy_image, Level lvl, float X, float Y, int W, int H, int Id) = 0;
};

class EnemyFactory : public AbstractEnemyFactory {
public:
	EnemyList *myEnemyList; MyCommandManager myCommandManager;
	void createEnemy1(EnemyList &my_enemy_list, Image enemy_image, Level lvl, float x, float y, int w, int h, int id)
	{
		std::shared_ptr<MyCommand> addEnemy1(new AddEnemyCommand(my_enemy_list, enemy_image, lvl, x, y, w, h, id, "enemy1", 100));
		myCommandManager.execute(addEnemy1);
	}
	void createEnemy2(EnemyList &my_enemy_list, Image enemy_image, Level lvl, float x, float y, int w, int h, int id)
	{
		std::shared_ptr<MyCommand> addEnemy2(new AddEnemyCommand(my_enemy_list, enemy_image, lvl, x, y, w, h, id, "enemy2", 150));
		myCommandManager.execute(addEnemy2);
	}
};

class AbstractTowerFactory
{
public:
	virtual void createTower1(TowerList &my_tower_list, Image tower_image, Level lvl, float X, float Y, int W, int H, int Id) = 0;
	virtual void createTower2(TowerList &my_tower_list, Image tower_image, Level lvl, float X, float Y, int W, int H, int Id) = 0;
};

class TowerFactory : public AbstractTowerFactory
{
public:
	TowerList * myTowerList; MyCommandManager myCommandManager;
	void createTower1(TowerList &my_tower_list, Image tower_image, Level lvl, float x, float y, int w, int h, int id)
	{
		std::shared_ptr<MyCommand> placeTower1(new PlaceTowerCommand(my_tower_list, tower_image, lvl, x, y, w, h, id, "tower1", 10));
		myCommandManager.execute(placeTower1);
	}
	void createTower2(TowerList &my_tower_list, Image tower_image, Level lvl, float x, float y, int w, int h, int id)
	{
		std::shared_ptr<MyCommand> placeTower2(new PlaceTowerCommand(my_tower_list, tower_image, lvl, x, y, w, h, id, "tower2", 40));
		myCommandManager.execute(placeTower2);
	}
};