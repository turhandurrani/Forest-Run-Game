# Forest Run

A 2D endless runner game built with C++ and SFML 3, developed as a semester project for CS112 - Object Oriented Programming and Design.

---

## Gameplay

You play as a knight running through an enchanted forest. Dodge obstacles, collect coins, and survive as long as possible. The game gets faster over time and the environment shifts from dusk to night.

### Controls

| Key | Action |
|-----|--------|
| `Space` / `Up` | Jump |
| `Down` / `S` | Duck |
| `R` | Restart (after death) |
| `Escape` | Quit |

---

## Features

- **Endless runner** with increasing difficulty over time
- **4 obstacle types** — Mushroom, Thornbush, Owl, Tree
- **Coin collection** system with animated spinning coins
- **Extra lives** — earn one every 10 coins collected
- **Invincibility window** after losing a life
- **High score persistence** — saved to and loaded from `highscore.txt`
- **Animated sprites** for player (run, jump, duck) and obstacles
- **3 background environments** with smooth crossfade transitions every 40 seconds
- **Dynamic audio** — background music, jump, coin, damage, extra life, and game over sounds
- **HUD** displaying score, high score, coins, and lives

---

## Project Structure

```
Forest-Run-Game/
│
├── main.cpp
│
├── Game.hpp / Game.cpp           — Window, rendering, event handling
├── GameManager.hpp / GameManager.cpp — Core game loop, state management
├── GameObject.hpp / GameObject.cpp   — Abstract base class for all entities
│
├── Player.hpp / Player.cpp       — Player movement, jumping, ducking
├── Obstacle.hpp / Obstacle.cpp   — Obstacle entity
├── ObstacleSpawner.hpp / .cpp    — Random obstacle generation
├── Coin.hpp / Coin.cpp           — Coin entity
│
├── Animator.hpp / Animator.cpp   — Sprite sheet animation system
├── AudioManager.hpp / .cpp       — Sound effects and background music
├── ScoreManager.hpp / .cpp       — High score file I/O with exception handling
│
└── assets/                       — Sprites, backgrounds, audio files
```

---

## OOP Concepts Demonstrated

| Concept | Where |
|--------|-------|
| Classes & Objects | All files |
| Encapsulation | All classes use private members with public getters |
| Inheritance (Hierarchical) | `Player`, `Obstacle`, `Coin` → `GameObject` |
| Abstract Classes & Pure Virtual Functions | `GameObject::update()` |
| Runtime Polymorphism & Virtual Functions | `update()` called on `GameObject*` pointers |
| Constructors & Initialization Lists | All classes |
| Destructors & Memory Cleanup | `GameManager`, `Obstacle`, `Coin` |
| Dynamic Memory Allocation | `new`/`delete` for obstacles and coins |
| Pointers to Objects | `std::vector<Obstacle*>`, `std::vector<Coin*>` |
| File Handling | `ScoreManager` reads/writes `highscore.txt` |
| Exception Handling | Custom `ScoreFileException` in `ScoreManager` |
| Static Members | `FLOOR_OFFSET`, `MAX_SPEED`, `GRAVITY` constants |

---

## Dependencies

- [SFML 3](https://www.sfml-dev.org/) — Graphics, Audio, Window
- C++17 or later
- MinGW / GCC (Windows) or equivalent

---

## Building

Make sure SFML is installed at `C:/SFML/`. Then compile with:

```bash
g++ main.cpp GameObject.cpp Player.cpp Obstacle.cpp ObstacleSpawner.cpp GameManager.cpp Game.cpp Coin.cpp Animator.cpp ScoreManager.cpp AudioManager.cpp -I"C:/SFML/include" -L"C:/SFML/lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o ForestRun.exe
```

Then run:
```bash
./ForestRun.exe
```

Make sure the `assets/` folder and `arial.ttf` are in the same directory as the executable.

---

## Assets

All sprites are pixel art sourced from free asset libraries. Audio files are in `.ogg` format.

| Asset | File |
|-------|------|
| Background Music | `backgroundMusic.ogg` |
| Jump Sound | `jump.ogg` |
| Coin Sound | `coin.ogg` |
| Damage Sound | `damage.ogg` |
| Extra Life Sound | `extraLife.ogg` |
| Game Over Sound | `gameOver.ogg` |

---

## Authors

Developed by Turhan Durrani and Shiza Tashfin — CS112 Semester Project.
