#include "GameManager.hpp"
#include <cstdlib>

GameManager::GameManager(float screenWidth, float screenHeight)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      player(100.0f, screenHeight - FLOOR_OFFSET),
      spawner(screenWidth, screenHeight - FLOOR_OFFSET, screenHeight / 2)
{
    state           = GameState::PLAYING;
    score           = 0.0f;
    gameSpeed       = 300.0f;
    lastSpeedScore  = 0.0f;
    coinCount       = 0;
    coinSpawnTimer = 0.0f;
    coinSpawnInterval = 0.0f;
}

GameManager::~GameManager() {
    for (Obstacle* obs : obstacles)
        delete obs;
    for (Coin* c : coins) delete c;
    obstacles.clear();
    coins.clear();
}

void GameManager::update(float deltaTime) {
    if (state != GameState::PLAYING) return;

    score     += gameSpeed * deltaTime * 0.05f;
    gameSpeed  = std::min(gameSpeed + deltaTime * 5.0f, MAX_SPEED);

    if (score - lastSpeedScore >= 100.0f) {
        lastSpeedScore = score;
        spawner.increaseSpeed(gameSpeed);
    }

    player.handleInput();
    player.update(deltaTime);

    Obstacle* newObs = spawner.update(deltaTime);
    if (newObs != nullptr)
        obstacles.push_back(newObs);

    for (Obstacle* obs : obstacles)
        obs->update(deltaTime);
    
    spawnCoins(deltaTime);
    for(Coin* c : coins) c->update(deltaTime);

    cleanupObstacles();
    cleanupCoins();
    checkCollisions();
}

void GameManager::spawnCoins(float deltaTime){
    coinSpawnTimer += deltaTime;
    if(coinSpawnTimer >= coinSpawnInterval){
        coinSpawnTimer = 0.0f;
        coinSpawnInterval = 1.0f * (float)(rand)() / RAND_MAX * 4.0f;

        float baseY = (screenHeight - FLOOR_OFFSET) - 80.0f - (float)(rand() % 120);
        for (int i = 0; i < 3; i++){
            coins.push_back(new Coin(screenWidth+ i * 35.0f, baseY, gameSpeed));
        }
    }
}

void GameManager::cleanupCoins(){
    for (auto it = coins.begin(); it != coins.end();){
        if ((*it)->isOffScreen()){
            delete *it;
            it = coins.erase(it);
        } else{
            ++it;
        }
    }
}

void GameManager::checkCollisions(){
    for (Obstacle* obs : obstacles) {
        if (player.collidesWith(*obs)) {
            state = GameState::DEAD;
            return;
        }
    }
    for (Coin* c : coins) {
        if (!c->isCollected() && player.collidesWith(*c)) {
            c->collect();
            coinCount++;
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
    lastSpeedScore = 0.0f;
    state          = GameState::PLAYING;

    spawner.reset();
    player.reset();
}

GameState                      GameManager::getState()        const { return state; }
float                          GameManager::getScore()        const { return score; }
Player&                        GameManager::getPlayer()             { return player; }
const std::vector<Obstacle*>&  GameManager::getObstacle()    const { return obstacles; }
const std::vector<Coin*>& GameManager::getCoins()     const { return coins; }
int                        GameManager::getCoinCount() const { return coinCount; }