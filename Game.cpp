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
      coinText(font),
      livesText(font),
      highScoreText(font)
{
    window.setFramerateLimit(60);
    timeElapsed = 0.0f;

    float groundH = 100.0f;
    groundLayer.setSize(sf::Vector2f(SCREEN_W, groundH));
    groundLayer.setPosition({0, SCREEN_H - groundH});
    groundLayer.setFillColor(sf::Color(34, 85, 34));

    if (!font.openFromFile("arial.ttf")) {}

    scoreText.setCharacterSize(28);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({20, 20});

    gravityText.setCharacterSize(22);
    gravityText.setFillColor(sf::Color(200, 255, 200));
    gravityText.setPosition({20, 60});

    deathText.setCharacterSize(52);
    deathText.setFillColor(sf::Color(255, 80, 80));
    deathText.setString("YOU DIED");
    deathText.setPosition({SCREEN_W / 2 - 130, SCREEN_H / 2 - 60});

    restartText.setCharacterSize(28);
    restartText.setFillColor(sf::Color::White);
    restartText.setString("Press R to restart");
    restartText.setPosition({SCREEN_W / 2 - 110, SCREEN_H / 2 + 10});

    livesText.setCharacterSize(28);
    livesText.setFillColor(sf::Color(255, 100, 100));
    livesText.setPosition({20, 90});

    highScoreText.setCharacterSize(28);
    highScoreText.setFillColor(sf::Color(255,215,0));
    highScoreText.setPosition({20,125});

    coinText.setCharacterSize(28);
    coinText.setFillColor(sf::Color(255, 215, 0));
    coinText.setPosition({20, 55});

    bgTextures[0].loadFromFile("assets/bg1.png");
    bgTextures[1].loadFromFile("assets/bg2.png");
    bgTextures[2].loadFromFile("assets/bg3.png");
    
    for (int i = 0; i < 3; i++) {
        bgSprites[i] = sf::Sprite(bgTextures[i], sf::IntRect({0, 0}, {1920, 1080}));
    }

    animJump = Animator("assets/playerJump1.png", 25, 36,  1,  8.0f);
    animDuck = Animator("assets/playerDuck1.png", 27, 26,  1, 10.0f);
    
    animMushroom = Animator("assets/mushroom.png", 30, 31, 1, 1.0f);
    animCoin     = Animator("assets/coin.png",     16, 16, 15, 12.0f);
    
    texTree.loadFromFile("assets/tree.png");
    
    if (!texThornbush.loadFromFile("assets/thronbush.png")) {}
    if (!texTree.loadFromFile("assets/tree.png")) {}

    spriteThornbush = sf::Sprite(texThornbush, sf::IntRect({0, 0}, {65, 32}));
    spriteTree      = sf::Sprite(texTree,      sf::IntRect({0, 0}, {80, 112}));
    
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
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

    PlayerState ps = gameManager.getPlayer().getState();
    if      (ps == PlayerState::DUCKING)                              animDuck.update(deltaTime);
    else if (ps == PlayerState::JUMPING || ps == PlayerState::HOVERING) animJump.update(deltaTime);
    else                                                               animRun.update(deltaTime);

    animOwl.update(deltaTime);
    animMushroom.update(deltaTime);
    animCoin.update(deltaTime);
}

void Game::render() {
    window.clear();

    drawBackground();
    drawCoins();
    drawObstacles();
    drawPlayer();
    drawHUD();

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
    int bgIndex = (int)(timeElapsed / 40.0f) % 3;
    int nextIndex = (bgIndex + 1) % 3;
    
    float intervalTime = std::fmod(timeElapsed, 40.0f);
    float alpha = std::min(std::max((intervalTime - 35.0f) / 5.0f, 0.0f), 1.0f);

    sf::Sprite& cur = *bgSprites[bgIndex];
    cur.setColor(sf::Color(255, 255, 255, 255));
    cur.setScale({SCREEN_W / 1920.0f, SCREEN_H / 1080.0f});
    cur.setPosition({0, 0});
    window.draw(cur);

    groundLayer.setPosition({0, SCREEN_H - 100.0f});
    window.draw(groundLayer);

    float pulse = (std::sin(timeElapsed * 3.0f) + 1.0f) / 2.0f; 
    sf::CircleShape firefly(3.0f);
    firefly.setFillColor(sf::Color(255, 255, 150, (uint8_t)(100 + 100 * pulse)));

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
    PlayerState state = player.getState();

    Animator* anim = &animRun;
    if (state == PlayerState::JUMPING || state == PlayerState::HOVERING)
        anim = &animJump;
    else if (state == PlayerState::DUCKING)
        anim = &animDuck;

    sf::Sprite spr(anim->getTexture(), anim->getCurrentRect());
    spr.setScale({2.0f, 2.0f});
    spr.setPosition({px, py});
    window.draw(spr);
}

void Game::drawObstacles() {
    for (Obstacle* obs : gameManager.getObstacle()) {
        float ox = obs->getX();
        float oy = obs->getY();
        float ow = obs->getWidth();
        float oh = obs->getHeight();

        if (obs->getType() == ObstacleType::OWL) {
        sf::Sprite spr(animOwl.getTexture(), animOwl.getCurrentRect());
        spr.setScale({1.0f, 1.0f});
        spr.setPosition({ox, oy});
        window.draw(spr);

    } else if (obs->getType() == ObstacleType::MUSHROOM) {
        sf::Sprite spr(animMushroom.getTexture(), animMushroom.getCurrentRect());
        spr.setScale({1.0f, 1.0f});
        spr.setPosition({ox, oy});
        window.draw(spr);

    } else if (obs->getType() == ObstacleType::THORNBUSH) {
        sf::Sprite& thornSpr = *spriteThornbush;
        thornSpr.setScale({1.0f, 1.0f});
        thornSpr.setPosition({ox, oy});
        window.draw(thornSpr);

    } else if (obs->getType() == ObstacleType::TREE) {
        sf::Sprite& treeSpr = *spriteTree;
        treeSpr.setScale({1.0f, 1.0f});
        treeSpr.setPosition({ox, oy});
        window.draw(treeSpr);
    }
    }
}

void Game::drawCoins() {
    for (Coin* c : gameManager.getCoins()) {
        if (!c->isCollected()) {
            sf::Sprite spr(animCoin.getTexture());
            spr.setTextureRect(animCoin.getCurrentRect());
            spr.setScale({20.0f / 16.0f, 20.0f / 16.0f});
            spr.setPosition({c->getX(), c->getY()});
            window.draw(spr);
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

    std::ostringstream ls;
    ls << "Lives: " << gameManager.getLives();
    livesText.setString(ls.str());
    window.draw(livesText);

    std::ostringstream hs;
    hs << "High Score: "<<gameManager.getHighScore();
    highScoreText.setString(hs.str());
    window.draw(highScoreText);
}

sf::Color Game::getSkyColor() const {
    float t = std::min(timeElapsed / 120.0f, 1.0f);
    uint8_t r = (uint8_t)(255 * (1 - t) + 10 * t);
    uint8_t g = (uint8_t)(160 * (1 - t) + 10 * t);
    uint8_t b = (uint8_t)(80  * (1 - t) + 40 * t);
    return sf::Color(r, g, b);
}

sf::Color Game::getObstacleColor(ObstacleType type) const {
    switch (type) {
        case ObstacleType::MUSHROOM:   return sf::Color(220, 60,  60);
        case ObstacleType::THORNBUSH:  return sf::Color(30,  120, 30);   
        case ObstacleType::OWL:        return sf::Color(139, 90,  43);  
        case ObstacleType::TREE:       return sf::Color(20,  80,  20);   
        default:                       return sf::Color::White;
    }
}
