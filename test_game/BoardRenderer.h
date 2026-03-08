#pragma once
#include "GameBoard.h"
#include <SFML/Graphics.hpp>
#include <cmath>
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
            visualPoints[i].setFillColor(Color::Color(13, 89, 26));
        }
    }

    void drawPoint(RenderWindow& window)
    {
        for (auto& shape : visualPoints) 
            window.draw(shape);  
    }

    RectangleShape drawLine(RenderWindow& window, int idx1, int idx2, float thickness, Color color)
    {
        Vector2f p1 = visualPoints[idx1].getPosition();
        Vector2f p2 = visualPoints[idx2].getPosition();
        Vector2f direction = p2 - p1;
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        float angleRad = atan2(direction.y, direction.x);
        RectangleShape line(sf::Vector2f(length, thickness));
        line.setFillColor(color);
        line.setPosition(p1);
        line.setRotation(radians(angleRad));
        return line;
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