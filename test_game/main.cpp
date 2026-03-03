#include<iostream>
#include "GameLogic.h"
#include <SFML/Graphics.hpp>
#include "GameBoard.h"
#include "BoardRenderer.h"
using std::cout;
using std::cin;

int main()
{
	setlocale(LC_ALL, "RUS");
	sf::ContextSettings settings;
	settings.antiAliasingLevel = 8;
	auto window = sf::RenderWindow(sf::VideoMode({ 1200, 800 }), "Window", sf::State::Windowed, settings);
	GameBoard board;
	BoardRenderer renderer;
	renderer.init(board, 70.f);
	while (window.isOpen()) 
	{
		while (const std::optional event = window.pollEvent()) 
		{
			if (event->is<sf::Event::Closed>()) 
				window.close();
			
		}
		window.clear(sf::Color(100, 149, 237));
		renderer.draw(window);
		window.display();
	}
	GameLogic round;
	cout << "Введите индексы двух точек для соединения (от 0 до 36)\n";
	cout << "Для выхода введите -1\n";
	int p1, p2;
	bool toggle = true;
	cout << "Ход игрока: введите две точки (p1 p2): \n";
	while (true)
	{
		if (toggle)
			cout << "Игрок 1: ";
		else 
			cout << "Игрок 2: ";
		cin >> p1;
		cin >> p2;
		if (p1 == -1 || p2 == -1)
			break;
		round.game(p1, p2, toggle);

		toggle = !toggle;
	}
	

	return 0;
}
