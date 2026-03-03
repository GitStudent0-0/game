#pragma once
#include "GameBoard.h"
#include <SFML/Graphics.hpp>
class BoardRenderer 
{
public:
    void init(const GameBoard& board, float radius) 
    {
        for (int i = 0; i < board.MAX_POINTS; i++)
        {
            visualPoints[i].setRadius(radius * 0.4f);
            visualPoints[i].setOrigin({ radius * 0.4f, radius * 0.4f });

            float x = radius * sqrt(3.f) * (board.getPoint(i).q + board.getPoint(i).r / 2.f);
            float y = radius * 1.5f * board.getPoint(i).r;
            visualPoints[i].setPosition({ 600.f + x, 400.f + y });
            visualPoints[i].setFillColor(sf::Color::White);
        }
    }

    void draw(sf::RenderWindow& window)
    {
        for (auto& shape : visualPoints) 
            window.draw(shape);  
    }

private:
    sf::CircleShape visualPoints[MAX_POINTS];
    int activePointsCount = 0;
};