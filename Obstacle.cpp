#include "Obstacle.hpp"

Obstacle::Obstacle(float startX, float startY, ObstacleType type, float speed) : GameObject(startX, startY, 0, 0), type(type), speed(speed) {
    if(type == ObstacleType::MUSHROOM) {
        width = 20;
        height = 30;
    }
    else if (type == ObstacleType::THORNBUSH) {
        width = 50; 
        height = 30;
    }
    else if (type == ObstacleType::OWL) {
        width = 35;
        height = 25;
    }
    else if (type == ObstacleType::TREE) {
        width = 40;
        height = 100;
    }
}

void Obstacle::update(float deltaTime) {
    x-= speed * deltaTime;
}

bool Obstacle::isOffScreen() const {
    return (x+width)<0;
}

ObstacleType Obstacle::getType() const {
    return type;
}

int Obstacle::getHeight() const{
    return height;
}

void Obstacle::setSpeed(float newSpeed) {
    speed = newSpeed;
}
