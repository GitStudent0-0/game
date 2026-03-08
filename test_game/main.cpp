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
	auto window = RenderWindow(VideoMode({ 1200, 800 }), "Window", State::Windowed, settings);
	window.setFramerateLimit(60);
	GameBoard board;
	GameLogic round(board);
	BoardRenderer renderer;
	renderer.init(board, 80.f);
	bool toggle = true;
	int firstPointIdx = -1;
	std::vector<RectangleShape> linesToDraw;
	while (window.isOpen()) 
	{
		while (const std::optional event = window.pollEvent()) 
		{
			if (event->is<Event::Closed>()) 
				window.close();
			if (const auto* mouseClick = event->getIf<Event::MouseButtonPressed>())
			{
				if (mouseClick->button == Mouse::Button::Left)
				{
					Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
					int  clickPointIdx= renderer.getPointByPosition(mousePos);

					if (clickPointIdx != -1)
					{
						if (firstPointIdx == -1)
							firstPointIdx = clickPointIdx;
						else if (firstPointIdx == clickPointIdx)
							firstPointIdx = -1;
						else
						{
							if (round.game(firstPointIdx, clickPointIdx, toggle))
							{
								linesToDraw.push_back(renderer.drawLine(window, firstPointIdx, clickPointIdx, 10.f, Color::Color(135, 163, 98)));
								toggle = !toggle;
							}
							else
								cout << "Неверный ход \n";
							firstPointIdx = -1;
						}
					}					
					else
						cout << "мимо \n";
				}
			}
		}
		window.clear(Color(215, 241, 247));
		for (const auto& line : linesToDraw)
			window.draw(line);
		renderer.drawPoint(window);
		window.display();
	}
	
	/*cout << "Введите индексы двух точек для соединения (от 0 до 36)\n";
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
	}*/
	

	return 0;
}
