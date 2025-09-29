# Wonky Kong (C++ / CS32)

C++ Donkey Kong–style platformer.  
**Authored core engine** (actor hierarchy, StudentWorld orchestrator, tick-based game loop, STL-based actor management, enemy AI, and safe memory lifecycle).  
level00-sample, framework and assets were provided by UCLA CS32.

---

## Overview
In **Wonky Kong**, the player navigates a 20×20 tile world of platforms and ladders while dodging barrels, fireballs, and koopas.  
The player can climb, jump, and collect goodies to survive and advance through levels.

Gameplay is managed via a **tick system** (~20 updates/second):
- Each actor performs an action (`doSomething()`).
- Dead actors are removed at the end of the tick.
- The display updates with score, lives, and burps remaining.

---

## Authored Files
This repository contains **only the files I implemented**:

- `Actor.cpp` / `Actor.h`  
  - Base `Actor` class and derived classes (Player, Enemies, Kong, Goodies, Floors, Ladders, Bonfires, Burps).  
  - Polymorphic `doSomething()` behaviors for movement, AI, collisions, and interactions.

- `StudentWorld.cpp` / `StudentWorld.h`  
  - Orchestrates the game world: level loading, per-tick scheduling, collision checks, cleanup, HUD updates, and spawning new objects.  
  - Manages all actors via an `std::vector<Actor*>` container with explicit deletion for safe memory lifecycle.

**Note:** All framework and asset files (`GameWorld`, `GraphObject`, `Level`, images, sounds) were provided by the course and are not included here.

---

## Key Features
- **Object-Oriented Design**  
  Inheritance hierarchy with shared base functionality and polymorphic behaviors.  

- **Game Loop Orchestration**  
  Implemented tick-driven simulation, per-actor updates, and cleanup after each tick.  

- **STL Usage**  
  Managed dynamic collections of polymorphic objects using `std::vector`, with safe deletion and iteration.  

- **Enemy AI**  
  - **Fireball:** Wanders, randomly climbs ladders, maintains climb state.  
  - **Koopa:** Freezes player with cooldown logic.  
  - **Barrel:** Rolls, falls, reverses direction when landing.  
  - **Kong:** Throws barrels periodically, flees when player approaches.  

- **Gameplay Logic**  
  Gravity, ladders, jump sequences, burp projectiles, goodies, scoring, lives, and HUD formatting.

- **Robust Memory Management**  
  Explicit cleanup (`deleteDead`, `cleanUp`) ensures no memory leaks or dangling pointers.

---

## Skills Demonstrated
- C++17  
- Object-Oriented Programming (inheritance, polymorphism, virtual methods)  
- STL containers (`std::vector`)  
- Game loop and tick-based scheduling  
- Collision detection and interaction logic  
- Rule-based enemy AI  
- Manual memory management (allocation/deallocation safety)

---

## Notes
This repository is for **portfolio and educational purposes**.  
Provided framework files are excluded to respect course policy — this repository showcases only my authored code.
