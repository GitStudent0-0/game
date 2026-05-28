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
    Vector2f boardCenter = { 700.f, 520.f };

    ConvexShape backButton;
    ConvexShape restartButton;
    void makeRounded(ConvexShape& shape, Vector2f size, float radius)
    {
      if (radius * 2 > size.x) 
        radius = size.x / 2;
      if (radius * 2 > size.y) 
        radius = size.y / 2;
      size_t points = 10;
      size_t pointsCount = points * 4;
      shape.setPointCount(pointsCount);

      float x = size.x, y = size.y;
      float step = (3.14159f / 2.0f) / (points - 1);
      for (size_t i = 0; i < points; i++)
      {
        float angle = i * step;
        shape.setPoint(i, { x - radius + cos(angle) * radius, radius - sin(angle) * radius });
        shape.setPoint(i + points, { radius - sin(angle) * radius, radius - cos(angle) * radius });
        shape.setPoint(i + points * 2, { radius - cos(angle) * radius, y - radius + sin(angle) * radius });
        shape.setPoint(i + points * 3, { x - radius + sin(angle) * radius, y - radius + cos(angle) * radius });
      }
    }
    void centerTextOrigin(Text& t)
    {
      auto bounds = t.getLocalBounds();
      float centerX = std::floor(bounds.position.x + bounds.size.x / 2.0f);
      float centerY = std::floor(bounds.position.y + bounds.size.y / 2.0f);
      t.setOrigin({ centerX, centerY });
    }

    void setupButton(ConvexShape& button, Vector2f size, Vector2f position)
    {
      makeRounded(button, size, 20.f);

      button.setFillColor(Color(145, 178, 197));
      button.setOutlineThickness(3.f);
      button.setOutlineColor(Color::Black);

      button.setOrigin({ size.x / 2.f, size.y / 2.f });
      button.setPosition(position);
    }

    void drawButton(RenderWindow& window, ConvexShape& button, Font& font, const String& text)
    {
      window.draw(button);

      Text buttonText(font, text, 30);
      buttonText.setFillColor(Color::Black);
      buttonText.setPosition(button.getPosition());

      centerTextOrigin(buttonText);

      window.draw(buttonText);
    }

public:
    void init(const GameBoard& board, float radius)
    {
      for (int i = 0; i < board.MAX_POINTS; i++)
      {
          visualPoints[i].setRadius(radius * 0.4f);
          visualPoints[i].setOrigin({ radius * 0.4f, radius * 0.4f });

          float x = radius * sqrt(3.f) * (board.getPoint(i).q + board.getPoint(i).r / 2.f);
          float y = radius * 1.5f * board.getPoint(i).r;
          visualPoints[i].setPosition({ boardCenter.x + x, boardCenter.y + y });
          visualPoints[i].setFillColor(Color(47, 62, 70));
      }
      setupButton(restartButton, { 220.f, 60.f }, { 150.f, 100.f });
      setupButton(backButton, { 220.f, 60.f }, { 150.f, 200.f });
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
      line.setFillColor(Color(107, 143, 163));
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
      token.setPosition({ boardCenter.x + x, boardCenter.y + y });
      token.setOutlineThickness(-5.f);
      token.setOutlineColor(Color(47, 62, 70));

      if (toggle)
          token.setFillColor(Color(245, 64, 127));
      else
          token.setFillColor(Color(0, 168, 107));
        
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
      float centerX = 700.f;
      float playersY = 25.f;
      float scoreY = 70.f;
      float gap = 20.f;

      Text player1(font, L"Игрок 1", 28);
      Text versus(font, L"vs", 28);
      Text player2(font, L"Игрок 2", 28);

      player1.setFillColor(Color(245, 64, 127));   
      versus.setFillColor(Color::Black);     
      player2.setFillColor(Color(0, 168, 107));  

      auto b1 = player1.getLocalBounds();
      auto bVs = versus.getLocalBounds();
      auto b2 = player2.getLocalBounds();

      float w1 = b1.size.x;
      float wVs = bVs.size.x;
      float w2 = b2.size.x;

      float totalWidth = w1 + gap + wVs + gap + w2;
      float startX = centerX - totalWidth / 2.f;

      player1.setOrigin({ b1.position.x, b1.position.y + b1.size.y / 2.f });
      versus.setOrigin({ bVs.position.x, bVs.position.y + bVs.size.y / 2.f });
      player2.setOrigin({ b2.position.x, b2.position.y + b2.size.y / 2.f });

      player1.setPosition({ startX, playersY });
      versus.setPosition({ startX + w1 + gap, playersY });
      player2.setPosition({ startX + w1 + gap + wVs + gap, playersY });

      window.draw(player1);
      window.draw(versus);
      window.draw(player2);

      String scoreStr = std::to_wstring(tokenCount1) + L" : " + std::to_wstring(tokenCount2);

      Text score(font, scoreStr, 35);
      score.setFillColor(Color::Black);

      auto scoreBounds = score.getLocalBounds();
      score.setOrigin({
          scoreBounds.position.x + scoreBounds.size.x / 2.f,
          scoreBounds.position.y + scoreBounds.size.y / 2.f
      });

      score.setPosition({ centerX, scoreY });
      window.draw(score);
    }

    void drawCurrentPlayerText(RenderWindow& window, bool toggle, Font& font)
    {
      String curPlayerText;
      Color curPlayerColor;

      if (toggle)
      {
        curPlayerText = L"Ходит игрок 1";
        curPlayerColor = Color(245, 64, 127);
      }
      else
      {
        curPlayerText = L"Ходит игрок 2";
        curPlayerColor = Color(0, 168, 107);
      }

      Text currentPlayer(font, curPlayerText, 35);
      currentPlayer.setFillColor(curPlayerColor);

      auto bounds = currentPlayer.getLocalBounds();
      float originX = std::floor(bounds.position.x + bounds.size.x / 2.f);
      float originY = std::floor(bounds.position.y + bounds.size.y / 2.f);
      currentPlayer.setOrigin({ originX, originY });

      currentPlayer.setPosition({ 1150.f, 100.f });
      window.draw(currentPlayer);
    }

    void drawGameButtons(RenderWindow& window, Font& font)
    {
      drawButton(window, restartButton, font, L"Заново");
      drawButton(window, backButton, font, L"Назад");
    }

    bool isBackButtonClicked(Vector2f mousePos)
    {
      return backButton.getGlobalBounds().contains(mousePos);
    }

    bool isRestartButtonClicked(Vector2f mousePos)
    {
      return restartButton.getGlobalBounds().contains(mousePos);
    }
};