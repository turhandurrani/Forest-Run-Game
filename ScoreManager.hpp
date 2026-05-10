#pragma once
#include <string>
#include <fstream>
#include <stdexcept>

class ScoreFileException : public std::runtime_error {
public:
    ScoreFileException(const std::string& message) 
        : std::runtime_error(message) {}
};

class ScoreManager {
private:
    std::string filePath;
    float highScore;

public:
    ScoreManager(const std::string& path);
    float getHighScore() const;
    void tryUpdateHighScore(float score);
};