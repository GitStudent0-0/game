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
	auto window = RenderWindow(VideoMode({ 1400, 900 }), "Window", State::Windowed, settings);
	window.setFramerateLimit(30);
	Font font;
	font.openFromFile("C:/Windows/Fonts/Arial.ttf");

	GameBoard board;
	GameLogic round(board);
	BoardRenderer renderer;
	renderer.init(board, 90.f);

	bool toggle = true;
	int firstPointIdx = -1;

	std::vector<RectangleShape> linesToDraw;
	std::vector<CircleShape> tokensToDraw;

	struct FlashEffect 
	{
		int p1 = -1, p2 = -1;
		Clock timer;
		bool active = false;
	} errorFlash;

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
					if (toggle)
						cout << "Игрок 1: ";
					else
						cout << "Игрок 2: ";
					if (clickPointIdx != -1)
					{
						cout << "Выбрана точка: " << clickPointIdx << '\n';
						if (firstPointIdx == -1)
							firstPointIdx = clickPointIdx;
						else if (firstPointIdx == clickPointIdx)
							firstPointIdx = -1;
						else
						{
							int tokensBefore = round.getTotalTokens();
							if (round.game(firstPointIdx, clickPointIdx, toggle))
							{
								linesToDraw.push_back(renderer.drawLine(firstPointIdx, clickPointIdx));
								int tokensAfter = round.getTotalTokens();
								for (int i = tokensBefore; i < tokensAfter; i++)
									tokensToDraw.push_back(renderer.drawTokens(round, i, toggle, 90.f));		
								toggle = !toggle;
							}
							else
							{
								cout << "Неверный ход \n";
								errorFlash.p1 = firstPointIdx;
								errorFlash.p2 = clickPointIdx;
								errorFlash.active = true;

								renderer.colorChange(errorFlash.p1, Color(219, 31, 31));
								renderer.colorChange(errorFlash.p2, Color(219, 31, 31));

								errorFlash.timer.restart();
							}
							firstPointIdx = -1;
						}
					}					
					else
						cout << "мимо \n";
				}
			}
		}
		if (errorFlash.active && errorFlash.timer.getElapsedTime().asSeconds() > 0.3f) 
		{
			renderer.colorChange(errorFlash.p1, Color(13, 89, 26));
			renderer.colorChange(errorFlash.p2, Color(13, 89, 26));
			errorFlash.active = false;
		}
		window.clear(Color(215, 241, 247));
		for (const auto& line : linesToDraw)
			window.draw(line);
		for (const auto& token : tokensToDraw)
			window.draw(token);
		renderer.drawPoint(window);
		renderer.text(window, round.getTokenCount1(), round.getTokenCount2(), font);
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
