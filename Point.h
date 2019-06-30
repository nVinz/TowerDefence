#pragma once
#include <SFML/Graphics.hpp>
#include "level.h"
#include "view.h"
#include "Entity.h"

class Point
{
public:
	float x, y;
	int w, h, id;
	Level lvl;
	Point(float X, float Y, int W, int H, int Id)
	{
		lvl.LoadFromFile("resources/map.tmx");
		x = X; y = Y; w = W; h = H; id = Id;
	}
	FloatRect getRect()
	{
		return FloatRect(x, y, w, h);
	}
};


class TowerPoint : public Point
{
public:
	TowerPoint(float X, float Y, int W, int H, int Id) : Point(X, Y, W, H, Id)
	{
	}
};


class StartPoint : public Point
{
public:
	int amount;
	StartPoint(float X, float Y, int W, int H, int Id) : Point(X, Y, W, H, Id)
	{

	}
	/*void spawn(Image &image, String name, Level &lvl, float start_point_left, float start_point_top, list<Entity*> enemy_entities, int a)
	{
		amount = a;
		for (int i = 0; i < amount; i++)
			enemy_entities.push_back(new Enemy(image, name, lvl, start_point_left, start_point_top, 64, 64));
	}*/
};


class End : public Point
{
	//void destroy(Enemy enemy);
};