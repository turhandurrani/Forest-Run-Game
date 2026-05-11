#pragma once
#include <SFML/Audio.hpp>
#include <string>

class AudioManager {
private:
    sf::Music backgroundMusic;

    sf::SoundBuffer jumpBuffer;
    sf::SoundBuffer coinBuffer;
    sf::SoundBuffer damageBuffer;
    sf::SoundBuffer extraLifeBuffer;
    sf::SoundBuffer gameOverBuffer;

    sf::Sound jumpSound;
    sf::Sound coinSound;
    sf::Sound damageSound;
    sf::Sound extraLifeSound;
    sf::Sound gameOverSound;

public:
    AudioManager();

    void playJump();
    void playCoin();
    void playDamage();
    void playExtraLife();
    void playGameOver();

    void setMusicVolume(float volume);
    void setSFXVolume(float volume);
    void stopMusic();
    void resumeMusic();
};