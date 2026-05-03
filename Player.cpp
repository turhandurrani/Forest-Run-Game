#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>

Player::Player(float startX, float startFloorY)
    : GameObject(startX, startFloorY - 60.0f, 40.0f, 60.0f)
{
    gravity        = GRAVITY;
    this->floorY   = startFloorY;
    onGround       = true;
    isHovering     = false;
    hoverTimer     = 0.0f;
    state          = PlayerState::RUNNING;
}

void Player::handleInput() {
    // --- JUMP ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && onGround) {
        velY     = -sqrt(2.0f * GRAVITY * 20);
        onGround = false;
        state    = PlayerState::JUMPING;
        
    }

    // --- HOVER (hold space while in air) ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !onGround) {
        if (hoverTimer < MAX_HOVER_TIME) {
            isHovering = true;
            state      = PlayerState::HOVERING;
        } else {
            isHovering = false;
        }
    } else if (onGround) {
        isHovering = false;
        hoverTimer = 0.0f;
    }

    // --- DUCK ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) or sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        state  = PlayerState::DUCKING;
        height = DUCK_HEIGHT;
        y = floorY + - DUCK_HEIGHT;
    } else if (onGround && state != PlayerState::JUMPING
                        && state != PlayerState::HOVERING) {
        height = STAND_HEIGHT;
        state  = PlayerState::RUNNING;
        y = floorY - STAND_HEIGHT;
    }
}

void Player::update(float deltaTime) {
    // Hover timer ticks up while hovering
    if (isHovering && !onGround) {
        hoverTimer += deltaTime;
    }

    // Apply gravity — reduced when hovering
    float currentGravity = isHovering ? HOVER_GRAVITY : gravity;
    velY += currentGravity * deltaTime;
    y    += velY * deltaTime;

    if (y + height >= floorY) {
        y          = floorY - height;
        velY       = 0;
        onGround   = true;
        isHovering = false;
        hoverTimer = 0.0f;
        if (state != PlayerState::DUCKING)
            state = PlayerState::RUNNING;
    }
    
}


void Player::reset() {
    // Reset to bottom floor position
    gravity    = GRAVITY;
    y          = floorY - STAND_HEIGHT;
    velY       = 0.0f;
    velX       = 0.0f;
    onGround   = true;
    isHovering = false;
    hoverTimer = 0.0f;
    height     = STAND_HEIGHT;
    state      = PlayerState::RUNNING;
}

PlayerState Player::getState()      const { return state; }
bool        Player::getIsHovering() const { return isHovering; }
float       Player::getHoverTime() const { return hoverTimer; }
bool        Player::getOnGround()   const { return onGround; }
