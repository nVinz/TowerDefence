#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "level.h"
#include "view.h"
#include "Point.h"
using namespace sf;
using namespace std;

class Entity
{
public:
	std::vector<Object> obj;
	float dx, dy, x, y, speed;
	int w, h, id;
	Texture texture;
	Sprite sprite;
	String name;
	Entity() {}
	Entity(Image &image, String Name, float X, float Y, int W, int H, int Id)
	{
		x = X; y = Y; w = W; h = H; name = Name; id = Id;
		speed = 0; dx = 0; dy = 0;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(32, 32);

	}
	FloatRect getRect()
	{
		return FloatRect(x, y, w, h);
	}
	virtual void update(float time) = 0;
	float getWidth() { return w; }
	float getHeight() { return h; }
	float getX() { return x; }
	float getY() { return y; }
	void setWidth(float width) { w = width; }
	void setHeight(float height) { h = height; }
};


class Enemy :public Entity
{
public:
	enum { left, right, up, down } state;
	Object end;

	vector<Object> turn_down;
	vector<Object> turn_up;
	vector<Object> turn_left;
	vector<Object> turn_right;

	bool life;
	float speed = 0.05;
	int health;
	Enemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H, int Id, int health_) : Entity(image, Name, X, Y, W, H, Id)
	{
		obj = lvl.GetObjects("place");
		end = lvl.GetObject("endPoint");
		turn_down = lvl.GetObjects("turn_down");
		turn_up = lvl.GetObjects("turn_up");
		turn_left = lvl.GetObjects("turn_left");
		turn_right = lvl.GetObjects("turn_right");
		//if (name == "enemy1") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dy = speed; state = down; id = Id; health = health_; 
			life = true;
		//}
	}
	void move(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)
		{
			if (getRect().intersects(end.rect))
			{
				if (health > 0)
				{
					health = 0;
					std::cout << "Enemy #" << id << ": finished\n";
				}
			}

			if (getRect().intersects(obj[i].rect))
			{
				if (state == down || state == up)
				{
					for (int r = 0; r < turn_right.size(); r++)
					{
						if (getRect().intersects(turn_right[r].rect))
						{
							dy = 0;
							dx = speed;
							state = right;
							sprite.setRotation(-90);
						}
					}
					for (int l = 0; l < turn_left.size(); l++)
					{
						if (getRect().intersects(turn_left[l].rect))
						{
							dy = 0;
							dx = -speed;
							state = left;
							sprite.setRotation(90);
						}
					}
				}
				if (state == right || state == left)
				{
					for (int d = 0; d < turn_down.size(); d++)
					{
						if (getRect().intersects(turn_down[d].rect))
						{
							dy = speed;
							dx = 0;
							state = down;
							sprite.setRotation(0);
						}
					}
					for (int u = 0; u < turn_up.size(); u++)
					{
						if (getRect().intersects(turn_up[u].rect))
						{
							dy = -speed;
							dx = 0;
							state = down;
							sprite.setRotation(180);
						}
					}
				}
			}
		}
	}

	void update(float time)
	{
		//if (name == "enemy1")
		//{
			move(dx, dy);
			x += dx * time;
			y += dy * time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0)
			{ 
				life = false; 
			}
		//}
	}
};

/*class SmallEnemy : public Enemy
{
public:
	SmallEnemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H, int Id) : Enemy(image, Name, lvl, X, Y, W, H, Id)
	{
		health = 100;
	}
};

class BigEnemy : public Enemy
{
public:
	BigEnemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H, int Id) : Enemy(image, Name, lvl, X, Y, W, H, Id)
	{
		health = 200;
	}
};*/

class Tower : public Entity
{
public:
	bool stay; int id, damage;
	CircleShape range;
	Texture place_t; Sprite place_s;
	Tower(Image &image, String Name, Level &lvl, float X, float Y, int W, int H, int Id, int damage_) : Entity(image, Name, X, Y, W, H, Id)
	{
		place_t.loadFromFile("images/textures/tower_place.png"); place_s.setTexture(place_t);
		place_s.setOrigin(32, 32);
		place_s.setPosition(x + 32, y + 32);
		obj = lvl.GetObjects("tower");
		x = X + 16; y = Y + 24; w = W; h = H; id = Id; damage = damage_;
		stay = true;

		range.setRadius(140); range.setFillColor(sf::Color(255, 255, 255, 6));
		range.setOutlineThickness(3); range.setOutlineColor(sf::Color(153, 255, 153, 40));
		range.setPosition(x - 121, y - 126);

	}
	void update(float time)
	{
		//if (name == "tower1")
		//{
			sprite.setPosition(x + w / 2, y + h / 2 - 8);
		//}
	}
};

class Explosion : public Entity
{
public:
	Explosion(Image &image, String Name, Level &lvl, float X, float Y, int W, int H, int Id) : Entity(image, Name, X, Y, W, H, Id)
	{
		x = X; y = Y; w = W; h = H; name = Name; id = Id;
	}

	void update(float time)
	{
		if (name == "explosion")
		{
			sprite.setPosition(x + w / 2, y + h / 2);
		}
	}
};