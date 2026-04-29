#pragma once 
#include "GameObject.hpp"

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

    static constexpr float JUMP_FORCE = -640.0f;
    static constexpr float GRAVITY = 800.0f;
    static constexpr float HOVER_GRAVITY = 150.0f;
    static constexpr float MAX_HOVER_TIME = 0.6f;
    static constexpr float DUCK_HEIGHT = 30.0f;
    static constexpr float STAND_HEIGHT = 60.0f;

    public: 
    Player(float startX, float floorY);

    void handleInput();
    void update(float deltaTime) override;
    void flipGravity(float newFloorY); 
    void reset();

    PlayerState getState() const;
    bool getIsHovering() const;
    float getHoverTime() const;
    bool getOnGround() const;

};
