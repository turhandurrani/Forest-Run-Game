#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>

Player::Player(float startX, float startFloorY)
    : GameObject(startX, startFloorY - STAND_HEIGHT, 54.0f, STAND_HEIGHT)
{
    this->floorY = startFloorY;
    gravity        = GRAVITY;
    this->floorY   = startFloorY;
    onGround       = true;
    state          = PlayerState::RUNNING;
    justJumped     = false;
}

void Player::handleInput() {
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) && onGround) {
        velY     = JUMP_FORCE;
        onGround = false;
        state    = PlayerState::JUMPING;
        justJumped = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) or sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        state  = PlayerState::DUCKING;
        height = DUCK_HEIGHT;
        velY = -JUMP_FORCE;
    } else if (onGround && state != PlayerState::JUMPING
                        && state != PlayerState::HOVERING) {
        height = STAND_HEIGHT;
        state  = PlayerState::RUNNING;
        y = floorY - STAND_HEIGHT;
    }
}

void Player::update(float deltaTime) {

    velY += gravity * deltaTime;
    y    += velY * deltaTime;

    if (y + height >= floorY) {
        y          = floorY - height;
        velY       = 0;
        onGround   = true;
        if (state != PlayerState::DUCKING)
            state = PlayerState::RUNNING;
    }
    
}


void Player::reset() {
    gravity    = GRAVITY;
    y          = floorY - STAND_HEIGHT;
    velY       = 0.0f;
    velX       = 0.0f;
    onGround   = true;
    height     = STAND_HEIGHT;
    state      = PlayerState::RUNNING;
}

PlayerState Player::getState()      const { return state; }
bool        Player::getOnGround()   const { return onGround; }
bool Player::getJustJumped()        const { return justJumped; }
void Player::clearJustJumped()      { justJumped = false; }
