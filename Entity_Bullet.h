#pragma once
#include <list>
#include "Entity.h"
class Bullet : public Entity
{
public:
	int damage;
	float direction, bulletSpeed;
	bool life;
	Enemy* enemy_entity;
	Bullet(Image &image, String Name, Level &lev, float X, float Y, int W, int H, float dir, int Id, Enemy* enemy_entity_, int damage_) : Entity(image, Name, X, Y, W, H, Id)
	{
		obj = lev.GetObjects("border");
		x = X; y = Y; direction = dir; id = Id; bulletSpeed = 0.2; w = h = 16; damage = damage_;
		life = true;
		enemy_entity = enemy_entity_;
	}
	void update(float time)
	{
		if (name == "bullet")
		{

			dx = bulletSpeed * cos(direction*(3.14159265 / 180));
			dy = bulletSpeed * sin(direction*(3.14159265 / 180));

			x += dx * time;
			y += dy * time;



			//if (x <= 0) { x = 1; }
			//if (y <= 0) { y = 1; }

			if (getRect().intersects(enemy_entity->sprite.getGlobalBounds()) && life == true)
			{
				enemy_entity->health -= damage;
				std::cout << "Bullet #" << id << " -> Enemy #" << enemy_entity->id << " (HP: " << enemy_entity->health << ")" << std::endl;
				life = false;
			}

			for (int i = 0; i < obj.size(); i++) {
				if (getRect().intersects(obj[i].rect))
				{
					life = false;
				}
			}

			sprite.setPosition(x + w / 2, y + h / 2);
		}
	}
};
