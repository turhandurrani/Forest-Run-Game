#pragma once
#include "Player.hpp"
#include "ObstacleSpawner.hpp"
#include "Coin.hpp"
#include <vector>

enum class GameState {
    PLAYING,
    DEAD,
    PAUSED
};

class GameManager {
    private:
    Player player;
    ObstacleSpawner spawner;
    std::vector<Obstacle*> obstacles;
    std::vector<Coin*> coins;

    GameState state;
    float score;
    float gameSpeed;
    float screenWidth;
    float screenHeight;
    int coinCount;
    float coinSpawnTimer;
    float coinSpawnInterval;

    static constexpr float FLOOR_OFFSET = 100.0f;  
    static constexpr float SPEED_INCREASE = 20.0f;   
    static constexpr float MAX_SPEED = 700.0f;

    float lastSpeedScore;

    void checkCollisions();
    void cleanupObstacles();
    void spawnCoins(float deltaTime);
    void cleanupCoins();

    public:
    GameManager(float screenWidth, float screenHeight);
    ~GameManager();

    void update(float deltaTime);
    void reset();

    GameState getState() const;
    float getScore() const;
    int getCoinCount() const;
    Player& getPlayer();
    const std::vector<Obstacle*>& getObstacle() const;
    const std::vector<Coin*>& getCoins() const;
};
