#include "Coin.hpp"

Coin::Coin(float x, float y, float speed)
    : GameObject(x, y, 20.0f, 20.0f), speed(speed), collected(false) {}

void Coin::update(float deltaTime) {
    x -= speed * deltaTime;
}

bool Coin::isOffScreen() const { return (x + width) < 0; }
bool Coin::isCollected()  const { return collected; }
void Coin::collect()            { collected = true; }