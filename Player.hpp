#pragma once 
#include "GameObject.hpp"
#include <cmath>

enum class PlayerState {
    RUNNING,
    JUMPING,
    HOVERING,
    DUCKING
};

class Player : public GameObject {
    private:
    PlayerState state;
    float gravity;
    bool onGround;
    float floorY;

    bool isHovering;
    float hoverTimer;

    static constexpr float JUMP_FORCE = 300;
    static constexpr float GRAVITY = 800.0f;
    static constexpr float HOVER_GRAVITY = 80.0f;
    static constexpr float MAX_HOVER_TIME = 1.2f;
    static constexpr float DUCK_HEIGHT = 30.0f;
    static constexpr float STAND_HEIGHT = 60.0f;

    public: 
    Player(float startX, float floorY);

    void handleInput();
    void update(float deltaTime) override;
    void reset();

    PlayerState getState() const;
    bool getIsHovering() const;
    float getHoverTime() const;
    bool getOnGround() const;

};
