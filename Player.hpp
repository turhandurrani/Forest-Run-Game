#pragma once 
#include "GameObject.hpp"
#include <iostream>

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

    class Stats {
        public:
        int jumpCount;
        float distanceTravelled;
        int lives;

        Stats() : jumpCount(0), distanceTravelled(0.0f), lives(3) {}
        Stats(int lives) : jumpCount(0), distanceTravelled(0.0f), lives(lives) {}
    };

    Stats stats;

    static int totalPlayersCreated;

    
    Player(float startX, float floorY);

    Player();
    Player(float startX, float floorY, int lives);

    Player(const Player& other);

    void move(float offsetX);
    void move(float offsetX, float offsetY);

    Player getSnapshot() const;

    void handleInput();
    void update(float deltaTime) override;
    void reset();

    bool getJustJumped() const;
    void clearJustJumped();

    inline PlayerState getState() const { return state; }
    inline bool getOnGround() const { return onGround; }
    inline float getFloorY() const { return floorY; }

    friend class GameManager;

    bool operator==(const Player& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Player& p);
};
