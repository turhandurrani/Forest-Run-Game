#pragma once
#include <SFML/Graphics.hpp>
#include "GameManager.hpp"

class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    GameManager gameManager;

    // Background layers (parallax)
    sf::RectangleShape skyLayer;
    sf::RectangleShape groundLayer;

    // Shapes for player
    sf::RectangleShape playerBody;
    sf::CircleShape    playerGlow;

    // HUD
    sf::Font font;
    sf::Text scoreText;
    sf::Text coinText;
    sf::Text gravityText;
    sf::Text deathText;
    sf::Text restartText;

    // Sky color transition (dusk -> night)
    float timeElapsed;

    // Screen dimensions
    static constexpr float SCREEN_W = 1280.0f;
    static constexpr float SCREEN_H = 720.0f;

public:
    Game();
    void run();

private:
    void handleEvents();
    void update(float deltaTime);
    void render();

    void drawPlayer();
    void drawObstacles();
    void drawCoins();
    void drawBackground();
    void drawHUD();

    sf::Color getSkyColor() const;
    sf::Color getObstacleColor(ObstacleType type) const;
};
