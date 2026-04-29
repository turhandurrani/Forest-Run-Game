#include "ObstacleSpawner.hpp"

ObstacleSpawner::ObstacleSpawner(float screenWidth, float floorY, float midY) : screenWidth(screenWidth), floorY(floorY), midY(midY) {
    currentSpeed = INITIAL_SPEED;
    spawnTimer = 0.0f;
    spawnInterval = randomInterval();
    srand((unsigned int)time(nullptr));   
}

Obstacle* ObstacleSpawner::update(float deltaTime) {
    spawnTimer += deltaTime;

    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0f;
        spawnInterval = randomInterval();
        ObstacleType type = randomType();
        float spawnY = (type==ObstacleType::OWL) ? midY : floorY;

        return new Obstacle(screenWidth, spawnY, type, currentSpeed);
    }
    return nullptr;
}

void ObstacleSpawner::increaseSpeed(float newSpeed) {
    currentSpeed = newSpeed;
}

void ObstacleSpawner::reset() {
    spawnTimer = 0.0f;
    spawnInterval = randomInterval();
    currentSpeed = INITIAL_SPEED;
}

ObstacleType ObstacleSpawner::randomType() {
    int r = rand() % 10; 

    if (r<=3) {
        return ObstacleType::MUSHROOM;
    }
    else if (r<=6) {
        return ObstacleType::THORNBUSH;
    }
    else if (r<=8) {
        return ObstacleType::OWL;
    }
    else {
        return ObstacleType::TREE;
    }
}

float ObstacleSpawner::randomInterval() {
    return MIN_INTERVAL + (float)rand() / RAND_MAX * (MAX_INTERVAL - MIN_INTERVAL);
}
