#pragma once
#include "GameObject.hpp"

class Coin : public GameObject {
private:
    float speed;
    bool collected;

public:
    Coin(float x, float y, float speed);
    void update(float deltaTime) override;
    bool isOffScreen() const;
    bool isCollected() const;
    void collect();
};