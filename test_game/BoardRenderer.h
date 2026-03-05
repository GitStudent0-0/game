#pragma once
#include "GameBoard.h"
#include <SFML/Graphics.hpp>
using namespace sf;
class BoardRenderer 
{
    CircleShape visualPoints[MAX_POINTS];
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
            visualPoints[i].setFillColor(Color::White);
        }
    }

    void draw(RenderWindow& window)
    {
        for (auto& shape : visualPoints) 
            window.draw(shape);  
    }

    int getPointByPosition(Vector2f mousePos)
    {
        for (int i = 0; i < MAX_POINTS; i++)
        {
            Vector2f pos = visualPoints[i].getPosition();
            float radius = visualPoints[i].getRadius();
            float dx = mousePos.x - pos.x;
            float dy = mousePos.y - pos.y;
            if ((dx * dx + dy * dy) <= (radius * radius))
                return i;
        }
        return -1;
    }
    void colorChange(int index)
    {
        visualPoints[index].setFillColor(Color::Magenta);
    }
};