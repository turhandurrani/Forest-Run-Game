#pragma once

class GameObject {
protected:
    float x, y;
    float width, height;
    float velX, velY;

public:
    GameObject(float x, float y, float w, float h);

    virtual void update(float deltaTime) = 0;
    bool collidesWith(const GameObject& other) const;

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;

    virtual ~GameObject() {}
};