#pragma once
#include "Player.hpp"
#include "ObstacleSpawner.hpp"
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

    GameState state;
    float score;
    float gameSpeed;
    bool gravityFlipped;
    float screenWidth;
    float screenHeight;

    static constexpr float FLOOR_OFFSET = 100.0f;  
    static constexpr float FLIP_INTERVAL = 200.0f; 
    static constexpr float SPEED_INCREASE = 20.0f;   
    static constexpr float MAX_SPEED = 700.0f;

    float lastFlipScore;
    float lastSpeedScore;

    void checkCollisions();
    void cleanupObstacles();
    void triggerGravityFlip();

    public:
    GameManager(float screenWidth, float screenHeight);
    ~GameManager();

    void update(float deltaTime);
    void reset();

    GameState getState() const;
    float getScore() const;
    bool isGravityFlipped() const;
    Player& getPlayer();
    const std::vector<Obstacle*>& getObstacle() const;
};
