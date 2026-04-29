#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
using namespace sf;

Player::Player(float starX, float startY): GameObject(starX, floorY - 60, 40, 60){
    gravity = GRAVITY;
    this->floorY = floorY - 60;
    onGround = true;
    isHovering = false;
    hoverTimer = 0.0f;
    state = PlayerState::RUNNING;
}

void Player::handleInput(){
    if(Keyboard::isKeyPressed(Keyboard::Key::Space) && onGround){
        velY = JUMP_FORCE;
        onGround = false;
        state = PlayerState::JUMPING;
    }
    
    if(Keyboard::isKeyPressed(Keyboard::Key::Space) && !onGround){
        if(hoverTimer < MAX_HOVER_TIME){
            isHovering = true;
            hoverTimer += 0.016f;
            state = PlayerState::HOVERING;
        }
        else{
            isHovering = false;
        }
    }
    else if (onGround){
        isHovering = false;
        hoverTimer = 0.0f;
    }

    if(Keyboard::isKeyPressed(Keyboard::Key::Down)){
        PlayerState::DUCKING;
        height = DUCK_HEIGHT;
    }
    else if (onGround && state != PlayerState::JUMPING){
        height = STAND_HEIGHT;
        if(state != PlayerState::HOVERING){
            state = PlayerState::RUNNING;
        }
    }
}

void Player::update(float deltaTime){
    float currentGravity = isHovering ? HOVER_GRAVITY : gravity;
    velY += currentGravity * deltaTime;
    y += velY * deltaTime;

    if (y >= floorY){
        y = floorY;
        velY = 0;
        onGround = true;
        isHovering = false;
        hoverTimer = 0.0f;
        if(state != PlayerState::DUCKING){
            state = PlayerState::DUCKING;
        }
    }
}

void Player::reset(){
    y = 0;
    velY = 0;
    velX = 0;
    gravity = GRAVITY;
    onGround = true;
    isHovering = false;
    hoverTimer = 0.0f;
    state = PlayerState::RUNNING;
    height = STAND_HEIGHT;
}

PlayerState Player::getState() const {return state;}
bool Player::getIsHovering() const {return isHovering;}
bool Player::getOnGround() const {return getOnGround;}
float Player::getHoverTime() const {return hoverTimer;}