# ChaosCraze

ChaosCraze is an experimental C++17 and raylib social-simulation game prototype. It models NPC traits, movement, collisions, sentiment, and interactions inside a small animated world.

## What is implemented

- NPC traits for name, occupation, age, sentiment, health, and animation state
- Idle, walking, attack, hurt, and death animations
- Random movement constrained by a collision map
- Quadtree-backed spatial queries for nearby NPCs
- Probabilistic interactions influenced by NPC characteristics
- UI actions that can change the sentiment of named or occupational groups
- Camera panning and zoom, menus, pause state, sound, and effects
- A capture tool that builds simple captions from NPCs inside a selected area

## Controls

| Input | Action |
| --- | --- |
| Right mouse drag | Pan the camera |
| Mouse wheel | Zoom |
| On-screen controls | Navigate menus, pause, post statements, and use capture tools |

## Build and run

The supplied Makefile targets a Linux-style desktop raylib installation and links against OpenGL and X11 libraries.

~~~bash
git clone https://github.com/redsteadz/ChaosCraze.git
cd ChaosCraze
make
cd build
../chaos_craze
~~~

The executable is launched from `build/` because the current asset paths are relative to that working directory.

## Status

ChaosCraze is a prototype for experimenting with simulation systems and game UI. The current entry point adds one NPC to the live scene while the remaining example characters are disabled in `main.cpp`; enable additional NPCs to exercise the interaction system.
