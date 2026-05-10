#include "Animator.hpp"

Animator::Animator(const std::string& filePath, int frameW, int frameH, int frames, float fps)
    : frameWidth(frameW), frameHeight(frameH),
      frameCount(frames), currentFrame(0),
      frameTimer(0.0f), frameInterval(1.0f / fps)
{
    texture.loadFromFile(filePath);
}

void Animator::update(float deltaTime) {
    frameTimer += deltaTime;
    if (frameTimer >= frameInterval) {
        frameTimer = 0.0f;
        currentFrame = (currentFrame + 1) % frameCount;
    }
}

sf::IntRect Animator::getCurrentRect() const {
    return sf::IntRect(
        sf::Vector2i(currentFrame * frameWidth, 0),
        sf::Vector2i(frameWidth, frameHeight)
    );
}

const sf::Texture& Animator::getTexture() const {
    return texture;
}

void Animator::reset() {
    currentFrame = 0;
    frameTimer   = 0.0f;
}