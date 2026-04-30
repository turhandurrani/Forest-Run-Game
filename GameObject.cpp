#include "GameObject.hpp"

GameObject::GameObject(float x, float y, float w, float h) : x(x), y(y), width(w), height(h), velX(0), velY(0) {}

bool GameObject::collidesWith(const GameObject& other) const {
    return (x < other.x + other.width) && (x + width > other.x) &&
    (y<other.y + other.height) && (y + height > other.y);
}

float GameObject::getX() const {
    return x;
}
float GameObject::getY() const {
    return y;
}
float GameObject::getWidth() const {
    return width;
}
float GameObject::getHeight() const {
    return height;
}


