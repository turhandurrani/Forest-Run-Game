#include "GameManager.hpp"
#include <cstdlib>

int GameManager::totalGamesPlayed = 0;

GameManager::GameManager(float screenWidth, float screenHeight)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      player(100.0f, screenHeight - FLOOR_OFFSET),
      spawner(screenWidth, screenHeight - FLOOR_OFFSET, screenHeight / 1.5),
      scoreManager("highscore.txt")
{
    state              = GameState::PLAYING;
    score              = 0.0f;
    gameSpeed          = 300.0f;
    lastSpeedScore     = 0.0f;
    coinCount          = 0;
    coinSpawnTimer     = 0.0f;
    coinSpawnInterval  = 0.0f;
    lives              = 1;
    invincibilityTimer = 0.0f;
    deathScoreChecked  = false;
    totalGamesPlayed++;
}

GameManager::~GameManager() {
    for (Obstacle* obs : obstacles) delete obs;
    for (Coin* c : coins)          delete c;
    obstacles.clear();
    coins.clear();
}

void GameManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) return;

    SaveData data;
    data.highScore      = scoreManager.getHighScore();
    data.totalCoinsEver = coinCount;
    data.gamesPlayed    = totalGamesPlayed;

    file.write(reinterpret_cast<const char*>(&data), sizeof(SaveData));
}

void GameManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return;

    SaveData data;
    file.read(reinterpret_cast<char*>(&data), sizeof(SaveData));

    if (file) {
        totalGamesPlayed = data.gamesPlayed;

    }
}

int GameManager::getTotalGamesPlayed() {
    return totalGamesPlayed;
}

void GameManager::update(float deltaTime) {
    if (state == GameState::DEAD) {
        if (!deathScoreChecked) {
            scoreManager.tryUpdateHighScore(score);
            saveToFile("savedata.bin");   
            deathScoreChecked = true;
        }
        return;
    }

    if (state != GameState::PLAYING) return;

    score     += gameSpeed * deltaTime * 0.05f;
    gameSpeed  = std::min(gameSpeed + deltaTime * 5.0f, MAX_SPEED);

    if (score - lastSpeedScore >= 100.0f) {
        lastSpeedScore = score;
        spawner.increaseSpeed(gameSpeed);
    }

    player.handleInput();
    if (player.getJustJumped()) {
        audioManager.playJump();
        player.clearJustJumped();
    }
    player.update(deltaTime);

    Obstacle* newObs = spawner.update(deltaTime);
    if (newObs != nullptr)
        obstacles.push_back(newObs);

    for (Obstacle* obs : obstacles) obs->update(deltaTime);

    spawnCoins(deltaTime);
    for (Coin* c : coins) c->update(deltaTime);

    if (invincibilityTimer > 0.0f) invincibilityTimer -= deltaTime;

    cleanupObstacles();
    cleanupCoins();
    checkCollisions();
}

void GameManager::spawnCoins(float deltaTime) {
    coinSpawnTimer += deltaTime;
    if (coinSpawnTimer >= coinSpawnInterval) {
        coinSpawnTimer    = 0.0f;
        coinSpawnInterval = 1.0f + (float)(rand()) / RAND_MAX * 4.0f;

        float baseY = (screenHeight - FLOOR_OFFSET) - 80.0f - (float)(rand() % 60);
        for (int i = 0; i < 3; i++)
            coins.push_back(new Coin(screenWidth + i * 35.0f, baseY, gameSpeed));
    }
}

void GameManager::cleanupCoins() {
    for (auto it = coins.begin(); it != coins.end();) {
        if ((*it)->isOffScreen()) { delete *it; it = coins.erase(it); }
        else                      { ++it; }
    }
}

void GameManager::checkCollisions() {
    for (Obstacle* obs : obstacles) {
        if (player.collidesWith(*obs) && invincibilityTimer <= 0.0f) {
            lives--;
            if (lives == 0) {
                audioManager.playGameOver();
                state = GameState::DEAD;
            } else {
                player.reset();
                invincibilityTimer = INVINCIBILITY_DURATION;
                audioManager.playDamage();
            }
            return;
        }
    }
    for (Coin* c : coins) {
        if (!c->isCollected() && player.collidesWith(*c)) {
            c->collect();
            coinCount++;
            audioManager.playCoin();
            if (coinCount % 10 == 0) {
                lives++;
                audioManager.playExtraLife();
            }
        }
    }
}

void GameManager::cleanupObstacles() {
    for (auto it = obstacles.begin(); it != obstacles.end();) {
        if ((*it)->isOffScreen()) { delete *it; it = obstacles.erase(it); }
        else                      { ++it; }
    }
}

void GameManager::reset() {
    for (Obstacle* obs : obstacles) delete obs;
    obstacles.clear();

    score              = 0.0f;
    gameSpeed          = 300.0f;
    lastSpeedScore     = 0.0f;
    invincibilityTimer = 0.0f;
    state              = GameState::PLAYING;
    lives              = 1;
    coinCount          = 0;
    deathScoreChecked  = false;

    spawner.reset();
    player.reset();
    audioManager.resumeMusic();
}

Player&                       GameManager::getPlayer()      { return player; }
const std::vector<Obstacle*>& GameManager::getObstacle()  const { return obstacles; }
const std::vector<Coin*>&     GameManager::getCoins()     const { return coins; }
float                         GameManager::getHighScore() const { return scoreManager.getHighScore(); }
