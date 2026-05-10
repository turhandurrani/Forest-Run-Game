#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "GameManager.hpp"
#include "Animator.hpp"

class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    GameManager gameManager;

    sf::RectangleShape skyLayer;
    sf::RectangleShape groundLayer;
    sf::RectangleShape playerBody;
    sf::CircleShape    playerGlow;
    sf::Texture bgTextures[3];
    std::optional<sf::Sprite> bgSprites[3];

    sf::Font font;
    sf::Text scoreText;
    sf::Text coinText;
    sf::Text gravityText;
    sf::Text deathText;
    sf::Text restartText;
    sf::Text livesText;
    sf::Text highScoreText;

    // Player animators
    Animator animRun;
    Animator animJump;
    Animator animDuck;

    // Obstacle animators
    Animator animOwl;
    Animator animMushroom;
    Animator animCoin;

    // Static obstacle sprites
    sf::Texture texThornbush;
    sf::Texture texTree;
    std::optional<sf::Sprite> spriteThornbush;
    std::optional<sf::Sprite> spriteTree;

    float timeElapsed;

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