#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <stack>
#include <memory>
#include <list>
#include "Entity.h"
using namespace std;

class MyCommand
{
public:
	virtual void execute() = 0;
	virtual void undo() = 0;
	virtual void redo() = 0;
	virtual void del() = 0;
};

class TowerList {
public:
	list<Tower*> tlist; list<Tower*>::iterator iterator;
	TowerList() {}
	void push_back(Image tower_image, Level lvl, float X, float Y, int W, int H, int Id, string name, int damage_)
	{
		tlist.push_back(new Tower(tower_image, name, lvl, X, Y, 32, 32, Id, damage_));
		cout << "| Tower placed (name: " << name << ", ID: " << Id << ")" << endl;
	}
	void pop_back()
	{
		tlist.pop_back();
		cout << "| Last tower removed" << endl;
	}
	void remove(Image tower_image, Level lvl, float X, float Y, int W, int H, int Id, string name, int damage_)
	{
		tlist.remove(new Tower(tower_image, name, lvl, X, Y, 32, 32, Id, damage_));
		cout << "| Tower removed" << endl;
	}
};

class PlaceTowerCommand : public MyCommand
{
private:
	TowerList * tower_list;
	Image tower_image; Level lvl; float x, y; int w, h, id, damage; string name;
public:
	PlaceTowerCommand(TowerList &tower_list_, Image tower_image_, Level lvl_, float X, float Y, int W, int H, int Id, string name_, int damage_)
		: tower_list(&tower_list_), tower_image(tower_image_), lvl(lvl_), x(X), y(Y), w(W), h(H), id(Id), name(name_), damage(damage_) {}

	void execute() { tower_list->push_back(tower_image, lvl, x, y, w, h, id, name, damage); }
	void undo() { tower_list->pop_back(); }
	void redo() { tower_list->push_back(tower_image, lvl, x, y, w, h, id, name, damage); }
	void del() { tower_list->push_back(tower_image, lvl, x, y, w, h, id, name, damage); }
};

class EnemyList {
public:
	list<Enemy*> elist; list<Enemy*>::iterator iterator;
	EnemyList() {}
	void add(Image enemy_image, Level lvl, float X, float Y, int W, int H, int Id, string name, int health_)
	{
		elist.push_back(new Enemy(enemy_image, name, lvl, X, Y, W, H, Id, health_));
		cout << "| Enemy  spawned (name: " << name << ", ID: " << Id << ")" << endl;
	}
	void remove()
	{
		elist.pop_back();
		cout << "| Enemy dead" << endl;
	}
};

class AddEnemyCommand : public MyCommand
{
private:
	EnemyList * my_enemy_list;
	Image enemy_image; Level lvl; float x, y; int w, h, id, health; string name;
public:
	AddEnemyCommand(EnemyList &my_enemy_list_, Image enemy_image_, Level lvl_, float X, float Y, int W, int H, int Id, string name_, int health_)
		: my_enemy_list(&my_enemy_list_), enemy_image(enemy_image_), lvl(lvl_), x(X), y(Y), w(W), h(H), id(Id), name(name_), health(health_) {}

	void execute() { my_enemy_list->add(enemy_image, lvl, x, y, w, h, id, name, health); }
	void undo() {}
	void redo() {}
	void del() {}
};


class Start
{
public:
	bool state;
	Start(){}
};

class StartCommand : public MyCommand
{
public:
	Start * s;

	StartCommand(Start &state_) : s(&state_) {}

	void execute() { s->state = true; }
	void undo() { s->state = false; }
	void redo() { s->state = true; }
	void del() {}
};




typedef std::stack<std::shared_ptr<MyCommand> > my_commandStack_t;

class MyCommandManager
{
private:
	my_commandStack_t my_UndoStack;
	my_commandStack_t my_RedoStack;
public:
	void execute(std::shared_ptr<MyCommand> command)
	{
		my_RedoStack = my_commandStack_t();
		command->execute();
		my_UndoStack.push(command);
	}
	void undo(std::shared_ptr<MyCommand> command)
	{
		if (my_UndoStack.size() <= 0) return;
		my_UndoStack.top()->undo();
		my_RedoStack.push(my_UndoStack.top());
		my_UndoStack.pop();
	}
	void redo(std::shared_ptr<MyCommand> command)
	{
		if (my_RedoStack.size() <= 0) return;
		my_RedoStack.top()->redo();
		my_UndoStack.push(my_RedoStack.top());
		my_RedoStack.pop();
	}
};