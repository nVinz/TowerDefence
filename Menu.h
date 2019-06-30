#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

void menu(RenderWindow & window) {
	Texture newgame_t, about_t, exit_t, about_bg_t, bg_t;
	newgame_t.loadFromFile("images/buttons/menu_newgame.png"); about_t.loadFromFile("images/buttons/menu_about.png"); exit_t.loadFromFile("images/buttons/menu_exit.png"); 
	bg_t.loadFromFile("images/background_menu.jpg");
	Sprite newgame_s(newgame_t), about_s(about_t), exit_s(exit_t), bg_s(bg_t);
	bool isMenu = true, isAboutMenu = false;
	int menuNum = 0;
	newgame_s.setPosition(150, 80); about_s.setPosition(128, 140); exit_s.setPosition(240, 200); 
	bg_s.setPosition(0, 0);  bg_s.setScale(720 / bg_s.getLocalBounds().width, 720 / bg_s.getLocalBounds().height);

	//  MENU  //
	while (isMenu)
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		newgame_s.setColor(Color::White); about_s.setColor(Color::White); exit_s.setColor(Color::White);
		menuNum = 0;

		if (IntRect(150, 80, 440, 56).contains(Mouse::getPosition(window))) { newgame_s.setColor(Color(180, 180, 180)); menuNum = 1; }
		if (IntRect(128, 140, 488, 56).contains(Mouse::getPosition(window))) { about_s.setColor(Color(180, 180, 180)); menuNum = 2; }
		if (IntRect(240, 200, 240, 64).contains(Mouse::getPosition(window))) { exit_s.setColor(Color(255, 180, 180)); menuNum = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;
			if (menuNum == 2) 
			{ 
				//window.draw(about_s); 
				//window.display(); 
				isAboutMenu = true; isMenu = false; break;
				//while (!Keyboard::isKeyPressed(Keyboard::Escape)); 
			}
			if (menuNum == 3) { window.close(); isMenu = false; }
		}

		window.draw(bg_s);
		window.draw(newgame_s);
		window.draw(about_s);
		window.draw(exit_s);

		window.display();
	}

	while (isAboutMenu)
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			isAboutMenu = false; isMenu = true;
		}
		window.draw(about_s); 
		window.display(); 
	}
	//  //  //
}