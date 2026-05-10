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
    bool justJumped;

    static constexpr float JUMP_FORCE = -640.0f;
    static constexpr float GRAVITY = 800.0f;
    static constexpr float STAND_HEIGHT = 72.0f; 
    static constexpr float DUCK_HEIGHT  = 52.0f; 

    public: 
    Player(float startX, float floorY);

    void handleInput();
    void update(float deltaTime) override;
    void reset();
    bool getJustJumped() const;
    void clearJustJumped();

    PlayerState getState() const;
    bool getOnGround() const;

};
