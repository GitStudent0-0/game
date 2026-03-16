#pragma once
#include "GameBoard.h"
#include "GameLogic.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <String>

using namespace sf;
class BoardRenderer 
{
    CircleShape visualPoints[GameBoard::MAX_POINTS];
public:
    void init(const GameBoard& board, float radius) 
    {
        for (int i = 0; i < board.MAX_POINTS; i++)
        {
            visualPoints[i].setRadius(radius * 0.4f);
            visualPoints[i].setOrigin({ radius * 0.4f, radius * 0.4f });

            float x = radius * sqrt(3.f) * (board.getPoint(i).q + board.getPoint(i).r / 2.f);
            float y = radius * 1.5f * board.getPoint(i).r;
            visualPoints[i].setPosition({ 700.f + x,450.f + y });
            visualPoints[i].setFillColor(Color(13, 89, 26));
        }
    }

    void drawPoint(RenderWindow& window)
    {
        for (auto& shape : visualPoints) 
            window.draw(shape);  
    }

    RectangleShape drawLine(int idx1, int idx2)
    {
        Vector2f p1 = visualPoints[idx1].getPosition();
        Vector2f p2 = visualPoints[idx2].getPosition();
        Vector2f direction = p2 - p1;
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        float angleRad = atan2(direction.y, direction.x);
        RectangleShape line(sf::Vector2f(length, 20.f));
        line.setOrigin({ 0.f, 10.f });
        line.setFillColor(Color(135, 163, 98));
        line.setPosition(p1);
        line.setRotation(radians(angleRad));
        return line;
    }

    CircleShape drawTokens(const GameLogic& logic, int idxToken, bool toggle, float radius)
    {
        CircleShape token(radius * 0.3f, 4);
        token.setOrigin({ radius * 0.3f, radius * 0.3f });
        

        float x = radius * sqrt(3.f) * (logic.tokens[idxToken].q + logic.tokens[idxToken].r / 2.f);
        float y = radius * 1.5f * logic.tokens[idxToken].r;
        token.setPosition({ 700.f + x, 450.f + y });
        
        if (toggle)
            token.setFillColor(Color(252, 188, 25));
        else
            token.setFillColor(Color(166, 93, 212));
        
        return token;
    }

    int getPointByPosition(Vector2f mousePos)
    {
        for (int i = 0; i < GameBoard::MAX_POINTS; i++)
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

    void colorChange(int index, Color color)
    {
        visualPoints[index].setFillColor(Color(color));
    }

    void text(RenderWindow& window, int& tokenCount1, int& tokenCount2, Font& font)
    {  
        String str1 = L"Фишки: " + std::to_string(tokenCount1);
        String str2 = L"Фишки: " + std::to_string(tokenCount2);

        Text text1(font, str1, 40);
        Text text2(font, str2, 40);
        float sizeBG = 120.f;
        CircleShape background1(sizeBG, 6);
        CircleShape background2(sizeBG, 6);

        background1.setFillColor(Color(135, 163, 98));
        background2.setFillColor(Color(135, 163, 98));

        background1.setOrigin({ sizeBG , sizeBG });
        background2.setOrigin({ sizeBG , sizeBG });

        background1.setPosition({150, 150});
        background2.setPosition({1250, 150});

        auto centerText = [](sf::Text& t) 
        {
            auto bounds = t.getLocalBounds();
            float centerX = bounds.position.x + bounds.size.x / 2.0f;
            float centerY = bounds.position.y + bounds.size.y / 2.0f;
            t.setOrigin({ centerX, centerY });
        };

        centerText(text1);
        centerText(text2);

        text1.setPosition({150, 150});
        text2.setPosition({1250, 150});

        text1.setFillColor(Color(209, 212, 57));
        text2.setFillColor(Color(166, 93, 212));

        window.draw(background1);
        window.draw(background2);
        window.draw(text1);
        window.draw(text2);
    }
};