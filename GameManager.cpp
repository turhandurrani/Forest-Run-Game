#include "GameManager.hpp"

GameManager::GameManager(float screenWidth, float screenHeight)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      player(100.0f, screenHeight - FLOOR_OFFSET),
      spawner(screenWidth, screenHeight - FLOOR_OFFSET, screenHeight / 2)
{
    state          = GameState::PLAYING;
    score          = 0.0f;
    gameSpeed      = 300.0f;
    gravityFlipped = false;
    lastFlipScore  = 0.0f;
    lastSpeedScore = 0.0f;
}

GameManager::~GameManager() {
    for (Obstacle* obs : obstacles)
        delete obs;
    obstacles.clear();
}

void GameManager::update(float deltaTime) {
    if (state != GameState::PLAYING) return;

    score     += gameSpeed * deltaTime * 0.05f;
    gameSpeed  = std::min(gameSpeed + deltaTime * 5.0f, MAX_SPEED);

    if (score - lastSpeedScore >= 100.0f) {
        lastSpeedScore = score;
        spawner.increaseSpeed(gameSpeed);
    }

    if (score - lastFlipScore >= FLIP_INTERVAL) {
        lastFlipScore = score;
        triggerGravityFlip();
    }

    player.handleInput();
    player.update(deltaTime);

    Obstacle* newObs = spawner.update(deltaTime);
    if (newObs != nullptr)
        obstacles.push_back(newObs);

    for (Obstacle* obs : obstacles)
        obs->update(deltaTime);

    cleanupObstacles();
    checkCollisions();
}

void GameManager::checkCollisions() {
    for (Obstacle* obs : obstacles) {
        if (player.collidesWith(*obs)) {
            state = GameState::DEAD;
            return;
        }
    }
}

void GameManager::cleanupObstacles() {
    for (auto it = obstacles.begin(); it != obstacles.end(); ) {
        if ((*it)->isOffScreen()) {
            delete *it;
            it = obstacles.erase(it);
        } else {
            ++it;
        }
    }
}


void GameManager::reset() {
    for (Obstacle* obs : obstacles)
        delete obs;
    obstacles.clear();

    score          = 0.0f;
    gameSpeed      = 300.0f;
    gravityFlipped = false;
    lastFlipScore  = 0.0f;
    lastSpeedScore = 0.0f;
    state          = GameState::PLAYING;

    spawner.reset();
    player.reset();
}

GameState                      GameManager::getState()        const { return state; }
float                          GameManager::getScore()        const { return score; }
bool                           GameManager::isGravityFlipped() const { return gravityFlipped; }
Player&                        GameManager::getPlayer()             { return player; }
const std::vector<Obstacle*>&  GameManager::getObstacle()    const { return obstacles; }