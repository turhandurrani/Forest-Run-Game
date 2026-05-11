#pragma once
#include <SFML/Graphics.hpp>

class Animator {
private:
    sf::Texture  texture;
    int          frameWidth;
    int          frameHeight;
    int          frameCount;
    int          currentFrame;
    float        frameTimer;
    float        frameInterval;

public:
    Animator() = default;
    Animator(const std::string& filePath, int frameW, int frameH, int frames, float fps);

    void update(float deltaTime);
    sf::IntRect getCurrentRect() const;
    const sf::Texture& getTexture() const;
    void reset();
};