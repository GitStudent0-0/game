#include<iostream>
#include "GameLogic.h"
using std::cout;
using std::cin;

int main()
{
	setlocale(LC_ALL, "RUS");
	GameLogic round;
	cout << "Введите индексы двух точек для соединения (от 0 до 36)\n";
	cout << "Для выхода введите -1\n";
	int p1, p2;
	while (true)
	{
		cout << "Ход игрока: введите две точки (p1 p2): ";
		cin >> p1;
		if (p1 == -1) 
			break;
		cin >> p2;

		round.game(p1, p2);
	}

	return 0;
}
