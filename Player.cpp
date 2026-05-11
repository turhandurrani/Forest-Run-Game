#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>


int Player::totalPlayersCreated = 0;

Player::Player(float startX, float startFloorY)
    : GameObject(startX, startFloorY - STAND_HEIGHT, 54.0f, STAND_HEIGHT)
{
    this->floorY  = startFloorY;
    gravity       = GRAVITY;
    onGround      = true;
    state         = PlayerState::RUNNING;
    justJumped    = false;
    totalPlayersCreated++;
}

Player::Player()
    : GameObject(100.0f, 300.0f, 54.0f, STAND_HEIGHT)
{
    this->floorY  = 300.0f + STAND_HEIGHT;
    gravity       = GRAVITY;
    onGround      = true;
    state         = PlayerState::RUNNING;
    justJumped    = false;
    totalPlayersCreated++;
}

Player::Player(float startX, float startFloorY, int lives)
    : GameObject(startX, startFloorY - STAND_HEIGHT, 54.0f, STAND_HEIGHT),
      stats(lives)
{
    this->floorY  = startFloorY;
    gravity       = GRAVITY;
    onGround      = true;
    state         = PlayerState::RUNNING;
    justJumped    = false;
    totalPlayersCreated++;
}

Player::Player(const Player& other)
    : GameObject(other),
      stats(other.stats)
{
    this->state      = other.state;
    this->gravity    = other.gravity;
    this->onGround   = other.onGround;
    this->floorY     = other.floorY;
    this->justJumped = other.justJumped;
    totalPlayersCreated++;
}

void Player::move(float offsetX) {
    this->x += offsetX;
}

void Player::move(float offsetX, float offsetY) {
    this->x += offsetX;
    this->y += offsetY;
}

Player Player::getSnapshot() const {
    return Player(*this);
}

bool Player::operator==(const Player& other) const {
    return (this->x == other.x && this->y == other.y);
}

std::ostream& operator<<(std::ostream& os, const Player& p) {
    os << "Player @ (" << p.x << ", " << p.y << ") "
       << "| State: " << static_cast<int>(p.state)
       << "| Lives: " << p.stats.lives
       << "| OnGround: " << p.onGround;
    return os;
}

void Player::handleInput() {
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) && onGround) {
        velY       = JUMP_FORCE;
        onGround   = false;
        state      = PlayerState::JUMPING;
        justJumped = true;
        stats.jumpCount++; 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        state        = PlayerState::DUCKING;
        this->height = DUCK_HEIGHT;   
        velY         = -JUMP_FORCE;
    } else if (onGround && state != PlayerState::JUMPING
                        && state != PlayerState::HOVERING) {
        this->height = STAND_HEIGHT;  
        state        = PlayerState::RUNNING;
        this->y      = floorY - STAND_HEIGHT;
    }
}

void Player::update(float deltaTime) {
    velY += gravity * deltaTime;
    y    += velY   * deltaTime;

    stats.distanceTravelled += deltaTime * 200.0f; 

    if (y + height >= floorY) {
        this->y    = floorY - height;  
        velY       = 0;
        onGround   = true;
        if (state != PlayerState::DUCKING)
            state = PlayerState::RUNNING;
    }
}

void Player::reset() {
    this->gravity  = GRAVITY;           
    this->y        = floorY - STAND_HEIGHT;
    this->velY     = 0.0f;
    this->velX     = 0.0f;
    this->onGround = true;
    this->height   = STAND_HEIGHT;
    state          = PlayerState::RUNNING;
    stats          = Stats();           
}

bool        Player::getJustJumped() const { return justJumped; }
void        Player::clearJustJumped()   { justJumped = false; }
