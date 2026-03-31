#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
using namespace sf;

class Menu 
{
	RectangleShape background;
	ConvexShape playButton;
	std::unique_ptr<Text> buttonText;

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
public:
	Menu(Font& font)
	{
		background.setSize({ 1400.f, 900.f });
		background.setFillColor(sf::Color(229, 177, 242));

		float radius = 30.f; 
		makeRounded(playButton, { 300.f, 80.f }, radius);
		playButton.setFillColor(Color(189, 76, 217)); 
		playButton.setOrigin({ 150.f, 40.f });
		playButton.setPosition({ 700.f, 450.f });

		buttonText = std::make_unique<Text>(font, L"Начать игру", 40);
		buttonText->setFillColor(Color::White);
		buttonText->setPosition({ 700.f, 450.f });
		auto centerText = [](sf::Text& t)
		{
			auto bounds = t.getLocalBounds();
			float centerX = bounds.position.x + bounds.size.x / 2.0f;
			float centerY = bounds.position.y + bounds.size.y / 2.0f;
			t.setOrigin({ centerX, centerY });
		};
		centerText(*buttonText);
	}

	void draw(RenderWindow& window)
	{
		window.draw(background);
		window.draw(playButton);
		window.draw(*buttonText);
	}

	bool isClicked(Vector2f mousePos)
	{
		return playButton.getGlobalBounds().contains(mousePos);
	}
};