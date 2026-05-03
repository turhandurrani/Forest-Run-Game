#pragma once 
#include "GameObject.hpp"

enum class ObstacleType {
    MUSHROOM, 
    THORNBUSH, 
    OWL, 
    TREE 
};

class Obstacle : public GameObject {
    private: 
    ObstacleType type;
    float speed;

    public:
    Obstacle(float startX, float startY, ObstacleType type, float speed);

    void update(float deltaTime) override;
    bool isOffScreen() const;

    ObstacleType getType() const;
    void setSpeed(float newSpeed);

    int getHeight()const;
    
};
