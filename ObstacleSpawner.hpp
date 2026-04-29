#pragma once 
#include "Obstacle.hpp"
#include <cstdlib>
#include <ctime>

class ObstacleSpawner {
    private:
    float spawnTimer;
    float spawnInterval;
    float screenWidth;
    float floorY;
    float midY;
    float currentSpeed;

    ObstacleType randomType();
    float randomInterval();

    static constexpr float MAX_INTERVAL = 2.5f;
    static constexpr float MIN_INTERVAL = 0.8f;
    static constexpr float INITIAL_SPEED = 300.0f;

    public:
    ObstacleSpawner(float screenWidth, float floorY, float midY);

    Obstacle* update(float deltaTime);
    void increaseSpeed(float newSpeed);
    void reset();

};