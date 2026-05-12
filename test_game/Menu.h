#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "GameLogic.h"

using namespace sf;

enum class PlayerType { Human, Bot };

class Menu
{
  RectangleShape background;

  ConvexShape playButton;
  ConvexShape rulesButton;

  Text titleText;
  Text rulesBtnText;
  Text startBtnText;
  Text p2Label;
  Text endModeLabel;
  CircleShape p2HumanRadio;
  CircleShape p2BotRadio;
  CircleShape endTokenRadio;
  CircleShape endMovesRadio;
  CircleShape p2HumanInner;
  CircleShape p2BotInner;
  CircleShape endTokenInner;
  CircleShape endMovesInner;

  Text p2HumanText;
  Text p2BotText;
  Text endTokenText;
  Text endMovesText;

  Text firstMoveLabel;
  Text firstPlayer1Text;
  Text firstPlayer2Text;

  CircleShape firstPlayer1Radio;
  CircleShape firstPlayer2Radio;
  CircleShape firstPlayer1Inner;
  CircleShape firstPlayer2Inner;
  bool firstPlayer = true;

  Text gameVariantLabel;
  Text classicVariantText;
  Text modifiedVariantText;

  CircleShape classicVariantRadio;
  CircleShape modifiedVariantRadio;
  CircleShape classicVariantInner;
  CircleShape modifiedVariantInner;

  bool isRulesState = false;
  RectangleShape rulesBg;
  Text rulesText;
  ConvexShape backButton;
  Text backBtnText;

  PlayerType p2Type = PlayerType::Bot;
  GameEndMode endMode = GameEndMode::NoMovesLeft;
  GameOption version = GameOption::Classical;

  void makeRounded(ConvexShape& shape, Vector2f size, float radius)
  {
    size_t pointsCount = 40;
    shape.setPointCount(pointsCount);

    float x = size.x, y = size.y;
    for (size_t i = 0; i < pointsCount / 4; i++)
    {
      float angle = i * 2 * 3.14159f / pointsCount;
      shape.setPoint(i, { x - radius + cos(angle) * radius, radius - sin(angle) * radius });
      shape.setPoint(i + pointsCount / 4, { radius - sin(angle) * radius, radius - cos(angle) * radius });
      shape.setPoint(i + pointsCount / 2, { radius - cos(angle) * radius, y - radius + sin(angle) * radius });
      shape.setPoint(i + 3 * pointsCount / 4, { x - radius + sin(angle) * radius, y - radius + cos(angle) * radius });
    }
  }

  void centerTextOrigin(Text& t)
  {
    auto bounds = t.getLocalBounds();
    float centerX = std::floor(bounds.position.x + bounds.size.x / 2.0f);
    float centerY = std::floor(bounds.position.y + bounds.size.y / 2.0f);
    t.setOrigin({ centerX, centerY });
  }

  void setupRadio(CircleShape& outer, CircleShape& inner, Vector2f pos)
  {
    outer.setRadius(12.f);
    outer.setOutlineThickness(3.f);
    outer.setOutlineColor(Color::Black);
    outer.setFillColor(Color::White);
    outer.setOrigin({ 12.f, 12.f });
    outer.setPosition(pos);

    inner.setRadius(6.f);
    inner.setFillColor(Color::Black);
    inner.setOrigin({ 6.f, 6.f });
    inner.setPosition(pos);
  }

public:
  
  Menu(Font& font) :
    titleText(font, L"Выбор режима", 45),
    rulesBtnText(font, L"Правила", 28),
    startBtnText(font, L"Начать игру", 30),
    p2Label(font, L"Второй игрок", 30),
    endModeLabel(font, L"Конец игры", 30),
    p2HumanText(font, L"человек", 25),
    p2BotText(font, L"бот", 25),
    endTokenText(font, L"кто первый расставит 10 фишек", 25),
    endMovesText(font, L"закончились ходы", 25),
    rulesText(font,
      L"Правила игры для классической версии:\n\n"
      L"1. Игроки по очереди соединяют 4 точки линией, параллельной граням поля.\n\n"
      L"2. Если образуется треугольник, в него ставится фишка.\n\n"
      L"3. Игра заканчивается по достижении условия конца игры.\n\n"
      L"Правила игры для модифицированной версии:\n\n"
      L"1. Игроки по очереди соединяют до 4 точек линией, параллельной граням поля.\n\n"
      L"2. Если образуется треугольник, в него ставится фишка.\n\n"
      L"3. Если точка используется 4 раза, она бледнеет.\n\n"
      L"4. Игра заканчивается по достижении условия конца игры.",
      30),
    firstMoveLabel(font, L"Первым ходит", 30),
    firstPlayer1Text(font, L"Игрок 1", 25),
    firstPlayer2Text(font, L"Игрок 2", 25),
    backBtnText(font, L"Назад", 30),
    gameVariantLabel(font, L"Вариант игры", 30),
    classicVariantText(font, L"классический вариант", 25),
    modifiedVariantText(font, L"модифицированный вариант", 25)
  {
    float leftX = 370.f;
    float rightX = 930.f;
    background.setSize({ 1400.f, 980.f });
    background.setFillColor(Color(238, 243, 247));

    makeRounded(rulesButton, { 180.f, 50.f }, 15.f);
    rulesButton.setFillColor(Color(145, 178, 197));
    rulesButton.setOutlineThickness(3.f);
    rulesButton.setOutlineColor(Color::Black);
    rulesButton.setOrigin({ 90.f, 25.f });
    rulesButton.setPosition({ 1200.f, 80.f });
    rulesBtnText.setFillColor(Color::Black);
    rulesBtnText.setPosition({ 1200.f, 80.f });
    centerTextOrigin(rulesBtnText);

    titleText.setFillColor(Color::Black);
    titleText.setPosition({ leftX + 330.f, 150.f });
    centerTextOrigin(titleText);

    p2Label.setFillColor(Color::Black);
    p2Label.setPosition({ leftX, 260.f });
    centerTextOrigin(p2Label);

    setupRadio(p2HumanRadio, p2HumanInner, { leftX - 100.f, 330.f });
    p2HumanText.setFillColor(Color::Black);
    p2HumanText.setPosition({ leftX - 70.f, 315.f });

    setupRadio(p2BotRadio, p2BotInner, { leftX - 100.f, 390.f });
    p2BotText.setFillColor(Color::Black);
    p2BotText.setPosition({ leftX - 70.f, 375.f });

    endModeLabel.setFillColor(Color::Black);
    endModeLabel.setPosition({ leftX, 500.f });
    centerTextOrigin(endModeLabel);

    setupRadio(endTokenRadio, endTokenInner, { leftX - 100.f, 570.f });
    endTokenText.setFillColor(Color::Black);
    endTokenText.setPosition({ leftX - 70.f, 555.f });

    setupRadio(endMovesRadio, endMovesInner, { leftX - 100.f, 630.f });
    endMovesText.setFillColor(Color::Black);
    endMovesText.setPosition({ leftX - 70.f, 615.f });

    firstMoveLabel.setFillColor(Color::Black);
    firstMoveLabel.setPosition({ rightX, 260.f });
    centerTextOrigin(firstMoveLabel);

    setupRadio(firstPlayer1Radio, firstPlayer1Inner, { rightX - 100.f, 330.f });
    firstPlayer1Text.setFillColor(Color::Black);
    firstPlayer1Text.setPosition({ rightX - 70.f, 315.f });

    setupRadio(firstPlayer2Radio, firstPlayer2Inner, { rightX - 100.f, 390.f });
    firstPlayer2Text.setFillColor(Color::Black);
    firstPlayer2Text.setPosition({ rightX - 70.f, 375.f });

    gameVariantLabel.setFillColor(Color::Black);
    gameVariantLabel.setPosition({ rightX, 500.f });
    centerTextOrigin(gameVariantLabel);

    setupRadio(classicVariantRadio, classicVariantInner, { rightX - 100.f, 570.f });
    classicVariantText.setFillColor(Color::Black);
    classicVariantText.setPosition({ rightX - 70.f, 555.f });

    setupRadio(modifiedVariantRadio, modifiedVariantInner, { rightX - 100.f, 630.f });
    modifiedVariantText.setFillColor(Color::Black);
    modifiedVariantText.setPosition({ rightX - 70.f, 615.f });

    makeRounded(playButton, { 300.f, 70.f }, 25.f);
    playButton.setFillColor(Color(145, 178, 197));
    playButton.setOutlineThickness(4.f);
    playButton.setOutlineColor(Color::Black);
    playButton.setOrigin({ 150.f, 35.f });
    playButton.setPosition({ 700.f, 780.f });
    startBtnText.setFillColor(Color::Black);
    startBtnText.setPosition({ 700.f, 780.f });
    centerTextOrigin(startBtnText);

    rulesBg.setSize({ 1400.f, 980.f });
    rulesBg.setFillColor(Color(238, 243, 247));
    rulesText.setFillColor(Color::Black);
    rulesText.setPosition({ 200.f, 100.f });

    makeRounded(backButton, { 250.f, 60.f }, 20.f);
    backButton.setFillColor(Color(145, 178, 197));
    backButton.setOutlineThickness(3.f);
    backButton.setOutlineColor(Color::Black);
    backButton.setOrigin({ 125.f, 30.f });
    backButton.setPosition({ 700.f, 780.f });
    backBtnText.setFillColor(Color::Black);
    backBtnText.setPosition({ 700.f, 780.f });
    centerTextOrigin(backBtnText);
  }

  void draw(RenderWindow& window)
  {
    if (isRulesState)
    {
      window.draw(rulesBg);
      window.draw(rulesText);
      window.draw(backButton);
      window.draw(backBtnText);
    }
    else
    {
      window.draw(background);

      window.draw(rulesButton);
      window.draw(rulesBtnText);
      window.draw(titleText);

      window.draw(p2Label);
      window.draw(p2HumanRadio);
      window.draw(p2HumanText);
      if (p2Type == PlayerType::Human)
        window.draw(p2HumanInner);

      window.draw(p2BotRadio);
      window.draw(p2BotText);
      if (p2Type == PlayerType::Bot)
        window.draw(p2BotInner);

      window.draw(endModeLabel);
      window.draw(endTokenRadio);
      window.draw(endTokenText);
      if (endMode == GameEndMode::TokenLimit)
        window.draw(endTokenInner);

      window.draw(endMovesRadio);
      window.draw(endMovesText);
      if (endMode == GameEndMode::NoMovesLeft)
        window.draw(endMovesInner);

      window.draw(playButton);
      window.draw(startBtnText);

      window.draw(firstMoveLabel);
      window.draw(firstPlayer1Radio);
      window.draw(firstPlayer1Text);
      if (firstPlayer)
        window.draw(firstPlayer1Inner);

      window.draw(firstPlayer2Radio);
      window.draw(firstPlayer2Text);
      if (!firstPlayer)
        window.draw(firstPlayer2Inner);

      window.draw(gameVariantLabel);
      window.draw(classicVariantRadio);
      window.draw(classicVariantText);
      if (version == GameOption::Classical)
        window.draw(classicVariantInner);

      window.draw(modifiedVariantRadio);
      window.draw(modifiedVariantText);
      if (version == GameOption::Modified)
        window.draw(modifiedVariantInner);
    }
  }

  bool isClicked(Vector2f mousePos)
  {
    if (isRulesState)
    {
      if (backButton.getGlobalBounds().contains(mousePos))
        isRulesState = false;
      return false;
    }

    if (rulesButton.getGlobalBounds().contains(mousePos))
    {
      isRulesState = true;
      return false;
    }

    if (p2HumanRadio.getGlobalBounds().contains(mousePos) || p2HumanText.getGlobalBounds().contains(mousePos))
      p2Type = PlayerType::Human;
    if (p2BotRadio.getGlobalBounds().contains(mousePos) || p2BotText.getGlobalBounds().contains(mousePos))
      p2Type = PlayerType::Bot;

    if (endTokenRadio.getGlobalBounds().contains(mousePos) || endTokenText.getGlobalBounds().contains(mousePos))
      endMode = GameEndMode::TokenLimit;
    if (endMovesRadio.getGlobalBounds().contains(mousePos) || endMovesText.getGlobalBounds().contains(mousePos))
      endMode = GameEndMode::NoMovesLeft;
    if (classicVariantRadio.getGlobalBounds().contains(mousePos) || classicVariantText.getGlobalBounds().contains(mousePos))
      version = GameOption::Classical;
    if (modifiedVariantRadio.getGlobalBounds().contains(mousePos) || modifiedVariantText.getGlobalBounds().contains(mousePos))
      version = GameOption::Modified;
    if (firstPlayer1Radio.getGlobalBounds().contains(mousePos) || firstPlayer1Text.getGlobalBounds().contains(mousePos))
      firstPlayer = true;
    if (firstPlayer2Radio.getGlobalBounds().contains(mousePos) || firstPlayer2Text.getGlobalBounds().contains(mousePos))
      firstPlayer = false;
    if (playButton.getGlobalBounds().contains(mousePos))
      return true;

    return false;
  }

  PlayerType getPlayer2Type() const 
  { 
    return p2Type; 
  }
  GameEndMode getEndMode() const 
  { 
    return endMode; 
  }
  bool isFirstPlayer() const 
  { 
    return firstPlayer; 
  }
  GameOption getGameVersion() const
  {
    return version;
  }
};