#include "Game.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <SFML/System.hpp>
using namespace sf;

Game::Game()
    : window(sf::VideoMode(sf::Vector2u((unsigned int)SCREEN_W, (unsigned int)SCREEN_H)), "Forest Run",
             sf::Style::Default),
      gameManager(SCREEN_W, SCREEN_H),
      scoreText(font),
      gravityText(font),
      deathText(font),
      restartText(font),
      coinText(font)
{
    window.setFramerateLimit(60);
    timeElapsed = 0.0f;

    // --- Sky layer ---
    skyLayer.setSize(sf::Vector2f(SCREEN_W, SCREEN_H));
    skyLayer.setPosition({0, 0});

    // --- Ground layer ---
    float groundH = 100.0f;
    groundLayer.setSize(sf::Vector2f(SCREEN_W, groundH));
    groundLayer.setPosition({0, SCREEN_H - groundH});
    groundLayer.setFillColor(sf::Color(34, 85, 34));   // dark green

    // --- Player shape ---
    playerBody.setSize(sf::Vector2f(100.0f, 120.0f));
    playerBody.setFillColor(sf::Color(180, 100, 220)); // purple fairy

    // --- Glow effect (soft circle behind player) ---
    playerGlow.setRadius(38.0f);
    playerGlow.setFillColor(sf::Color(180, 100, 220, 60)); // transparent purple

    // --- Font + Text ---
    // Using default system font as fallback — Turhan can swap for a custom .ttf
    if (!font.openFromFile("arial.ttf")) {
        // fallback: SFML will render without font, no crash
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(28);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({20, 20});

    gravityText.setFont(font);
    gravityText.setCharacterSize(22);
    gravityText.setFillColor(sf::Color(200, 255, 200));
    gravityText.setPosition({20, 60});

    deathText.setFont(font);
    deathText.setCharacterSize(52);
    deathText.setFillColor(sf::Color(255, 80, 80));
    deathText.setString("YOU DIED");
    deathText.setPosition({SCREEN_W / 2 - 130, SCREEN_H / 2 - 60});

    restartText.setFont(font);
    restartText.setCharacterSize(28);
    restartText.setFillColor(sf::Color::White);
    restartText.setString("Press R to restart");
    restartText.setPosition({SCREEN_W / 2 - 110, SCREEN_H / 2 + 10});

    coinText = sf::Text(font);
    coinText.setCharacterSize(28);
    coinText.setFillColor(sf::Color(255, 215, 0));
    coinText.setPosition({20, 55});
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        // cap deltaTime so a lag spike doesn't launch the fairy into space
        if (deltaTime > 0.05f) deltaTime = 0.05f;

        timeElapsed += deltaTime;

        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::handleEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::R &&
                gameManager.getState() == GameState::DEAD) {
                gameManager.reset();
                timeElapsed = 0.0f;
            }
            if (keyPressed->code == sf::Keyboard::Key::Escape)
                window.close();
        }
    }
}

void Game::update(float deltaTime) {
    gameManager.update(deltaTime);
}

void Game::render() {
    window.clear();

    drawBackground();
    drawCoins();
    drawObstacles();
    drawPlayer();
    drawHUD();

    // Death screen overlay
    if (gameManager.getState() == GameState::DEAD) {
        sf::RectangleShape overlay(sf::Vector2f(SCREEN_W, SCREEN_H));
        overlay.setFillColor(sf::Color(0, 0, 0, 140));
        window.draw(overlay);
        window.draw(deathText);
        window.draw(restartText);
    }

    window.display();
}

void Game::drawBackground() {
    // Sky color shifts from dusk orange -> midnight blue over time
    skyLayer.setFillColor(getSkyColor());
    window.draw(skyLayer);

    // Ground — flips position when gravity flips
    groundLayer.setPosition({0, SCREEN_H - 100.0f});
    window.draw(groundLayer);

    // Firefly particles — simple pulsing dots scattered across screen
    float pulse = (std::sin(timeElapsed * 3.0f) + 1.0f) / 2.0f; // 0.0 - 1.0
    sf::CircleShape firefly(3.0f);
    firefly.setFillColor(sf::Color(255, 255, 150, (uint8_t)(100 + 100 * pulse)));

    // Fixed positions for fireflies (consistent per run)
    float fireflyPositions[][2] = {
        {200, 200}, {450, 350}, {700, 150}, {900, 400},
        {1100, 250},{300, 500}, {600, 300}, {1050, 150}
    };
    for (auto& pos : fireflyPositions) {
        firefly.setPosition({pos[0], pos[1]});
        window.draw(firefly);
    }
}

void Game::drawPlayer() {
    Player& player = gameManager.getPlayer();
    float px = player.getX();
    float py = player.getY();

    // Draw glow behind player
    playerGlow.setPosition({px - 18, py - 10});
    window.draw(playerGlow);

    // Draw player body — color changes by state
    PlayerState state = player.getState();
    if (state == PlayerState::HOVERING) {
        playerBody.setFillColor(sf::Color(220, 150, 255)); // lighter when hovering
        playerBody.setSize(sf::Vector2f(40.0f, 60.0f));
    } else if (state == PlayerState::DUCKING) {
        playerBody.setFillColor(sf::Color(140, 70, 180));
        playerBody.setSize(sf::Vector2f(50.0f, 30.0f));  // wider, shorter when ducking
    } else {
        playerBody.setFillColor(sf::Color(180, 100, 220));
        playerBody.setSize(sf::Vector2f(40.0f, 60.0f));
    }

    playerBody.setPosition({px, py});
    window.draw(playerBody);

    // Wing flap — two small rectangles on sides, offset by time
    float wingOffset = std::sin(timeElapsed * 15.0f) * 5.0f;
    sf::RectangleShape wingL(sf::Vector2f(15.0f, 20.0f));
    sf::RectangleShape wingR(sf::Vector2f(15.0f, 20.0f));
    wingL.setFillColor(sf::Color(220, 180, 255, 180));
    wingR.setFillColor(sf::Color(220, 180, 255, 180));
    wingL.setPosition({px - 15, py + 10 + wingOffset});
    wingR.setPosition({px + 40, py + 10 - wingOffset});
    window.draw(wingL);
    window.draw(wingR);

    // Sparkle trail when jumping or hovering
    if (state == PlayerState::JUMPING || state == PlayerState::HOVERING) {
        sf::CircleShape sparkle(4.0f);
        sparkle.setFillColor(sf::Color(255, 255, 100,
                             (uint8_t)(150 + 50 * std::sin(timeElapsed * 20.0f))));
        sparkle.setPosition({px - 10, py + 20});
        window.draw(sparkle);
        sparkle.setPosition({px - 5,  py + 35});
        window.draw(sparkle);
        sparkle.setPosition({px - 15, py + 50});
        window.draw(sparkle);
    }
}

void Game::drawObstacles() {
    sf::RectangleShape shape;

    for (Obstacle* obs : gameManager.getObstacle()) {
        shape.setSize(sf::Vector2f(obs->getWidth(), obs->getHeight()));
        shape.setPosition({obs->getX(), obs->getY()});
        shape.setFillColor(getObstacleColor(obs->getType()));

        // Dark outline on all obstacles
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color(0, 0, 0, 120));

        window.draw(shape);

        // Extra detail per type
        if (obs->getType() == ObstacleType::TREE) {
            // Draw a darker trunk at the bottom
            sf::RectangleShape trunk(sf::Vector2f(15.0f, 30.0f));
            trunk.setFillColor(sf::Color(80, 40, 10));
            trunk.setPosition({obs->getX() + 12, obs->getY() + obs->getHeight() - 30});
            window.draw(trunk);
        }

        if (obs->getType() == ObstacleType::OWL) {
            // Draw two small eye dots
            sf::CircleShape eye(4.0f);
            eye.setFillColor(sf::Color::Yellow);
            eye.setPosition({obs->getX() + 8,  obs->getY() + 6});
            window.draw(eye);
            eye.setPosition({obs->getX() + 22, obs->getY() + 6});
            window.draw(eye);
        }
    }
}

void Game::drawCoins(){
    sf::CircleShape coinShape(10.0f);
    coinShape.setFillColor(sf::Color(255, 215, 0));
    coinShape.setOutlineThickness(2);
    coinShape.setOutlineColor(sf::Color(200, 160, 0));

    for (Coin* c : gameManager.getCoins()) {
        if (!c->isCollected()) {
            coinShape.setPosition({c->getX(), c->getY()});
            window.draw(coinShape);
        }
    }
}

void Game::drawHUD() {
    std::ostringstream ss;
    ss << "Score: " << std::fixed << std::setprecision(0) << gameManager.getScore();
    scoreText.setString(ss.str());
    window.draw(scoreText);

    std::ostringstream cs;
    cs << "Coins: " << gameManager.getCoinCount();
    coinText.setString(cs.str());
    window.draw(coinText);
}

sf::Color Game::getSkyColor() const {
    // Lerp from dusk (255, 160, 80) to night (10, 10, 40) over 120 seconds
    float t = std::min(timeElapsed / 120.0f, 1.0f);
    uint8_t r = (uint8_t)(255 * (1 - t) + 10 * t);
    uint8_t g = (uint8_t)(160 * (1 - t) + 10 * t);
    uint8_t b = (uint8_t)(80  * (1 - t) + 40 * t);
    return sf::Color(r, g, b);
}

sf::Color Game::getObstacleColor(ObstacleType type) const {
    switch (type) {
        case ObstacleType::MUSHROOM:   return sf::Color(220, 60,  60);   // red
        case ObstacleType::THORNBUSH:  return sf::Color(30,  120, 30);   // dark green
        case ObstacleType::OWL:        return sf::Color(139, 90,  43);   // brown
        case ObstacleType::TREE:       return sf::Color(20,  80,  20);   // deep green
        default:                       return sf::Color::White;
    }
}
