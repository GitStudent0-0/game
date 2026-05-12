#include<iostream>
#include "GameLogic.h"
#include <SFML/Graphics.hpp>
#include "GameBoard.h"
#include "BoardRenderer.h"
#include "Menu.h"
#include "MinimaxBot.h"

using std::cout;
using std::cin;
enum class AppState { Menu, Game };

const float VIRTUAL_WIDTH = 1400.f;
const float VIRTUAL_HEIGHT = 980.f;
const float BOARD_RADIUS = 82.f;

View getVirtualBox(View view, Vector2u windowSize)
{
  float windowRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
  float viewRatio = view.getSize().x / view.getSize().y;

  float sizeX = 1.f;
  float sizeY = 1.f;
  float posX = 0.f;
  float posY = 0.f;

  bool horizontalSpacing = windowRatio > viewRatio;

  if (horizontalSpacing)
  {
    sizeX = viewRatio / windowRatio;
    posX = (1.f - sizeX) / 2.f;
  }
  else
  {
    sizeY = windowRatio / viewRatio;
    posY = (1.f - sizeY) / 2.f;
  }

  view.setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));
  return view;
}

void updatePointColors(GameLogic& round, BoardRenderer& renderer, GameOption currentVersion)
{
	for (int i = 0; i < GameBoard::MAX_POINTS; i++)
	{
		if (currentVersion == GameOption::Modified && round.isPointOverflow(i))
			renderer.colorChange(i, Color(206, 216, 224));
		else
			renderer.colorChange(i, Color(47, 62, 70));
	}
}

bool applyMove(GameLogic& round,BoardRenderer& renderer, vector<RectangleShape>& linesToDraw, vector<CircleShape>& tokensToDraw, Move move, bool currentPlayer, GameOption currentVersion)
{
	int tokensBefore = round.getTotalTokens();
	if (round.game(move.a, move.b, currentPlayer))
	{
		updatePointColors(round, renderer, currentVersion);
		linesToDraw.push_back(renderer.drawLine(move.a, move.b));
		int tokensAfter = round.getTotalTokens();
		for (int i = tokensBefore; i < tokensAfter; i++)
			tokensToDraw.push_back(renderer.drawTokens(round, i, currentPlayer, BOARD_RADIUS));
		return true;
	}
	return false;
}

PlayerType getCurrentPlayerType(bool toggle, PlayerType player1Type, PlayerType player2Type)
{
	if (toggle)
		return player1Type;
	else
		return player2Type;
}

void restartGame(
  GameBoard& board,
  BoardRenderer& renderer,
  std::unique_ptr<GameLogic>& round,
  std::unique_ptr<Bot>& bot,
  GameEndMode currentMode,
  GameOption currentVersion,
  bool firstPlayer,
  bool& toggle,
  int& firstPointIdx,
  vector<RectangleShape>& linesToDraw,
  vector<CircleShape>& tokensToDraw,
  bool& botDelayStarted
)
{
  bot.reset();

  round = std::make_unique<GameLogic>(board, currentMode, currentVersion);
  if (currentVersion == GameOption::Classical)
    bot = std::make_unique<Bot>(*round, 3, 3);
  else
    bot = std::make_unique<Bot>(*round, 2, 1);

  renderer.init(board, BOARD_RADIUS);

  toggle = firstPlayer;
  firstPointIdx = -1;

  linesToDraw.clear();
  tokensToDraw.clear();

  botDelayStarted = false;
}

String getEndGameText(GameResult result)
{
  String str = L"";
  if (result == GameResult::Player1Win)
    str = L"Победил игрок 1";
  else if (result == GameResult::Player2Win)
    str = L"Победил игрок 2";
  else if (result == GameResult::Draw)
    str = L"Ничья";
  return str;
}

void drawEndGameBanner(RenderWindow& window, Font& font, const String& message)
{
  RectangleShape panel({ 600.f, 300.f });
  panel.setFillColor(Color(218, 230, 240));
  panel.setOutlineThickness(4.f);
  panel.setOutlineColor(Color::Black);
  panel.setOrigin({ 300.f, 150.f });
  panel.setPosition({ 700.f, 490.f });

  Text text(font, message, 45);
  text.setFillColor(Color::Black);

  auto bounds = text.getLocalBounds();
  text.setOrigin({
    bounds.position.x + bounds.size.x / 2.f,
    bounds.position.y + bounds.size.y / 2.f
  });

  text.setPosition(panel.getPosition());

  window.draw(panel);
  window.draw(text);
}

int main()
{
  setlocale(LC_ALL, "RUS");
  ContextSettings settings;
  settings.antiAliasingLevel = 8;
  auto window = RenderWindow(VideoMode({ 1400, 980 }), "Window", State::Windowed, settings);
  View gameView(FloatRect({ 0.f, 0.f }, { VIRTUAL_WIDTH, VIRTUAL_HEIGHT }));
  window.setView(gameView);
  window.setFramerateLimit(30);
  AppState curScreen = AppState::Menu;
  Font font;
  font.openFromFile("C:/Windows/Fonts/Arial.ttf");
  Menu menu(font);

  GameBoard board;
  BoardRenderer renderer;
  renderer.init(board, BOARD_RADIUS);

  std::unique_ptr<GameLogic> round;
  std::unique_ptr<Bot> bot;

  PlayerType player1Type = PlayerType::Human;
  PlayerType player2Type = PlayerType::Bot;
  GameEndMode currentMode = GameEndMode::NoMovesLeft;
  GameOption currentVersion = GameOption::Classical;

  bool toggle = true;
  int firstPointIdx = -1;
  bool firstPlayer = true;

  vector<RectangleShape> linesToDraw;
  vector<CircleShape> tokensToDraw;

  struct FlashEffect
  {
    int p1 = -1, p2 = -1;
    Clock timer;
    bool active = false;
  } errorFlash;

  struct HighlightEffect
  {
    int p1 = -1, p2 = -1;
    Move move;
    Clock timer;
    bool active = false;
  } highlightEffect;

  struct EndBanner
  {
    String message;
    Clock timer;
    bool active = false;
    bool shown = false;
  } endBanner;

  Clock botDelayClock;
  bool botDelayStarted = false;

  while (window.isOpen())
  {
    while (const std::optional event = window.pollEvent())
    {
      if (event->is<Event::Closed>())
        window.close();
      if (const auto* resized = event->getIf<Event::Resized>())
      {
        gameView = getVirtualBox(gameView, resized->size);
        window.setView(gameView);
      }
      if (const auto* mouseClick = event->getIf<Event::MouseButtonPressed>())
      {
        if (mouseClick->button == Mouse::Button::Left)
        {
          Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), gameView);
          if (curScreen == AppState::Menu)
          {
            if (menu.isClicked(mousePos))
            {
              player2Type = menu.getPlayer2Type();
              GameEndMode selectedMode = menu.getEndMode();
              currentMode = selectedMode;

              firstPlayer = menu.isFirstPlayer();
              toggle = firstPlayer;

              currentVersion = menu.getGameVersion();
              round = std::make_unique<GameLogic>(board, selectedMode, currentVersion);
              if (currentVersion == GameOption::Classical)
                bot = std::make_unique<Bot>(*round, 3, 3);
              else
                bot = std::make_unique<Bot>(*round, 2, 1);
              endBanner.active = false;
              endBanner.shown = false;
              endBanner.message = L"";

              curScreen = AppState::Game;
            }
          }
          else if (curScreen == AppState::Game)
          {
            if (renderer.isBackButtonClicked(mousePos))
            {
              curScreen = AppState::Menu;

              bot.reset();
              round.reset();

              linesToDraw.clear();
              tokensToDraw.clear();

              toggle = true;
              firstPointIdx = -1;

              errorFlash.active = false;
              errorFlash.p1 = -1;
              errorFlash.p2 = -1;

              botDelayStarted = false;

              renderer.init(board, BOARD_RADIUS);
            }
            else if (renderer.isRestartButtonClicked(mousePos))
            {
              restartGame(
                board,
                renderer,
                round,
                bot,
                currentMode,
                currentVersion,
                firstPlayer,
                toggle,
                firstPointIdx,
                linesToDraw,
                tokensToDraw,
                botDelayStarted
              );

              errorFlash.active = false;
              errorFlash.p1 = -1;
              errorFlash.p2 = -1;

              highlightEffect.active = false;
              highlightEffect.p1 = -1;
              highlightEffect.p2 = -1;

              endBanner.active = false;
              endBanner.shown = false;
              endBanner.message = L"";
            }
            else
            {
              PlayerType currentPlayerType = getCurrentPlayerType(toggle, player1Type, player2Type);
              if (currentPlayerType == PlayerType::Human)
              {
                int clickPointIdx = renderer.getPointByPosition(mousePos);
                if (toggle)
                  cout << "Игрок 1: ";
                else
                  cout << "Игрок 2: ";

                if (clickPointIdx != -1)
                {
                  cout << "Выбрана точка: " << clickPointIdx << '\n';

                  if (round->isGameOver())
                  {
                    firstPointIdx = -1;
                    errorFlash.p1 = clickPointIdx;
                    errorFlash.p2 = clickPointIdx;
                    errorFlash.active = true;
                    renderer.colorChange(clickPointIdx, Color(219, 31, 31));
                    errorFlash.timer.restart();
                  }
                  else if (firstPointIdx == -1)
                  {
                    firstPointIdx = clickPointIdx;
                    renderer.colorChange(firstPointIdx, Color(46, 191, 44));
                  }
                  else if (firstPointIdx == clickPointIdx)
                  {
                    firstPointIdx = -1;
                    updatePointColors(*round, renderer, currentVersion);
                  }
                  else
                  {

                    Move humanMove;
                    humanMove.a = firstPointIdx;
                    humanMove.b = clickPointIdx;

                    if (round->canMakeMove(humanMove.a, humanMove.b))
                    {
                      renderer.colorChange(clickPointIdx, Color(46, 191, 44));

                      highlightEffect.p1 = firstPointIdx;
                      highlightEffect.p2 = clickPointIdx;
                      highlightEffect.move = humanMove;
                      highlightEffect.active = true;
                      highlightEffect.timer.restart();
                    }
                    else
                    {
                      cout << "Неверный ход!\n";
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
        }
      }
    }

    PlayerType currentPlayerType = getCurrentPlayerType(toggle, player1Type, player2Type);
    if (highlightEffect.active && highlightEffect.timer.getElapsedTime().asSeconds() > 0.1f)
    {
      if (applyMove(*round, renderer, linesToDraw, tokensToDraw, highlightEffect.move, toggle, currentVersion))
      {
        toggle = !toggle;

        if (round->isGameOver() && !endBanner.shown)
        {
          endBanner.message = getEndGameText(round->getResult());
          endBanner.active = true;
          endBanner.shown = true;
          endBanner.timer.restart();
        }
      }

      highlightEffect.active = false;
      highlightEffect.p1 = -1;
      highlightEffect.p2 = -1;
    }
    if (curScreen == AppState::Game && currentPlayerType == PlayerType::Bot && !round->isGameOver())
    {
      if (!botDelayStarted)
      {
        botDelayClock.restart();
        botDelayStarted = true;
      }
      if (botDelayClock.getElapsedTime().asSeconds() >= 0.1f)
      {
        Move botMove = bot->choiceMove(); 
        if (botMove.a != -1 && botMove.b != -1)
        {
          cout << "Бот выбрал ход: " << botMove.a << " " << botMove.b << '\n';
          if (applyMove(*round, renderer, linesToDraw, tokensToDraw, botMove, toggle, currentVersion))
          {
            toggle = !toggle;

            if (round->isGameOver() && !endBanner.shown)
            {
              endBanner.message = getEndGameText(round->getResult());
              endBanner.active = true;
              endBanner.shown = true;
              endBanner.timer.restart();
            }
          }
        }
        botDelayStarted = false;
      }
    }
    else
      botDelayStarted = false;

    if (errorFlash.active && errorFlash.timer.getElapsedTime().asSeconds() > 0.3f)
    {
      for (int i = 0; i < GameBoard::MAX_POINTS; i++)
      {
        if (currentVersion == GameOption::Modified && round->isPointOverflow(i))
          renderer.colorChange(i, Color(206, 216, 224));
        else
          renderer.colorChange(i, Color(47, 62, 70));
      }
      errorFlash.active = false;
    }

    window.clear(Color(238, 243, 247));
    if (endBanner.active && endBanner.timer.getElapsedTime().asSeconds() > 3.0f)
      endBanner.active = false;

    if (curScreen == AppState::Menu)
      menu.draw(window);
    else if (curScreen == AppState::Game)
    {
      for (const auto& line : linesToDraw)
        window.draw(line);
      for (const auto& token : tokensToDraw)
        window.draw(token);
      renderer.drawPoint(window);
      renderer.text(window, round->getTokenCount1(), round->getTokenCount2(), font);
      renderer.drawCurrentPlayerText(window, toggle, font);
      renderer.drawGameButtons(window, font);
      if (endBanner.active)
        drawEndGameBanner(window, font, endBanner.message);
    }
    window.display();
  }

  return 0;
}