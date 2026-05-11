#include "ScoreManager.hpp"
#include <iostream>

ScoreManager::ScoreManager(const std::string& path) : filePath(path), highScore(0.0f) {
    try {
        std::ifstream file(filePath);
        if (!file.is_open())
            throw ScoreFileException("Could not open highscore file: " + filePath);
        
        if (!(file >> highScore))
            throw ScoreFileException("Highscore file is empty or corrupted: " + filePath);
        
        file.close();
    }
    catch (const ScoreFileException& e) {
        std::cout << "[ScoreManager] " << e.what() << " — starting from 0\n";
        highScore = 0.0f;
    }
    catch (const std::exception& e) {
        std::cout << "[ScoreManager] Unexpected error reading score: " << e.what() << "\n";
        highScore = 0.0f;
    }
}

void ScoreManager::tryUpdateHighScore(float score) {
    if (score <= highScore) return;

    try {
        std::ofstream file(filePath);
        if (!file.is_open())
            throw ScoreFileException("Could not write to highscore file: " + filePath);
        
        file << score;
        
        if (file.fail())
            throw ScoreFileException("Failed to write score to file: " + filePath);
        
        file.close();
        highScore = score;
        std::cout << "[ScoreManager] New high score saved: " << score << "\n";
    }
    catch (const ScoreFileException& e) {
        std::cout << "[ScoreManager] " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "[ScoreManager] Unexpected error saving score: " << e.what() << "\n";
    }
}

float ScoreManager::getHighScore() const { return highScore; }