#pragma once
#include "Player.hpp"
#include "ObstacleSpawner.hpp"
#include "Coin.hpp"
#include "ScoreManager.hpp"
#include "AudioManager.hpp"
#include <vector>
#include <fstream> 
enum class GameState {
    PLAYING,
    DEAD,
    PAUSED
};

struct SaveData {
    float highScore;
    int   totalCoinsEver;
    int   gamesPlayed;
};

class GameManager {
    private:
    Player player;
    ObstacleSpawner spawner;
    ScoreManager scoreManager;
    AudioManager audioManager;
    std::vector<Obstacle*> obstacles;
    std::vector<Coin*> coins;
    GameState state;
    float score;
    float gameSpeed;
    float screenWidth;
    float screenHeight;
    int coinCount;
    float coinSpawnTimer;
    float coinSpawnInterval;
    int lives;
    float invincibilityTimer;
    bool deathScoreChecked;

    static constexpr float FLOOR_OFFSET          = 72.0f;
    static constexpr float SPEED_INCREASE        = 20.0f;
    static constexpr float MAX_SPEED             = 700.0f;
    static constexpr float INVINCIBILITY_DURATION = 2.0f;

    static int totalGamesPlayed;

    float lastSpeedScore;

    void checkCollisions();
    void cleanupObstacles();
    void spawnCoins(float deltaTime);
    void cleanupCoins();

    public:
    GameManager(float screenWidth, float screenHeight);
    ~GameManager();
    void update(float deltaTime);
    void reset();

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    static int getTotalGamesPlayed();

    inline GameState getState()    const { return state; }
    inline float     getScore()    const { return score; }
    inline int       getCoinCount() const { return coinCount; }
    inline int       getLives()    const { return lives; }

    float  getHighScore() const;
    Player& getPlayer();
    const std::vector<Obstacle*>& getObstacle() const;
    const std::vector<Coin*>&     getCoins()    const;
};
