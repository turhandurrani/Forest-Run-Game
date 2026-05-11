#include "AudioManager.hpp"
#include <iostream>

AudioManager::AudioManager()
    : jumpSound(jumpBuffer),
      coinSound(coinBuffer),
      damageSound(damageBuffer),
      extraLifeSound(extraLifeBuffer),
      gameOverSound(gameOverBuffer)
{
    // Load background music (streamed)
    if (!backgroundMusic.openFromFile("assets/backgroundMusic.ogg"))
        std::cout << "[AudioManager] Failed to load background music\n";
    else {
        backgroundMusic.setLooping(true);
        backgroundMusic.setVolume(50.0f);
        backgroundMusic.play();
    }

    // Load sound effects
    if (!jumpBuffer.loadFromFile("assets/jump.ogg"))
        std::cout << "[AudioManager] Failed to load jump sound\n";

    if (!coinBuffer.loadFromFile("assets/coin.ogg"))
        std::cout << "[AudioManager] Failed to load coin sound\n";

    if (!damageBuffer.loadFromFile("assets/damage.ogg"))
        std::cout << "[AudioManager] Failed to load damage sound\n";

    if (!extraLifeBuffer.loadFromFile("assets/extraLife.ogg"))
        std::cout << "[AudioManager] Failed to load extra life sound\n";

    if (!gameOverBuffer.loadFromFile("assets/gameOver.ogg"))
        std::cout << "[AudioManager] Failed to load game over sound\n";

    // Set default volumes
    jumpSound.setVolume(80.0f);
    coinSound.setVolume(70.0f);
    damageSound.setVolume(90.0f);
    extraLifeSound.setVolume(90.0f);
    gameOverSound.setVolume(100.0f);
}

void AudioManager::playJump()      { jumpSound.play(); }
void AudioManager::playCoin()      { coinSound.play(); }
void AudioManager::playDamage()    { damageSound.play(); }
void AudioManager::playExtraLife() { extraLifeSound.play(); }
void AudioManager::playGameOver()  { gameOverSound.play(); }

void AudioManager::setMusicVolume(float volume) { backgroundMusic.setVolume(volume); }
void AudioManager::setSFXVolume(float volume) {
    jumpSound.setVolume(volume);
    coinSound.setVolume(volume);
    damageSound.setVolume(volume);
    extraLifeSound.setVolume(volume);
    gameOverSound.setVolume(volume);
}

void AudioManager::stopMusic()   { backgroundMusic.stop(); }
void AudioManager::resumeMusic() { backgroundMusic.play(); }