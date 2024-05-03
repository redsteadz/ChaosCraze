#ifndef COLLISION_MAPPER_H
#define COLLISION_MAPPER_H

#include <iostream>
#include <fstream>
#include "raylib.h"

class CollisionMapper {
public:
    static void LoadCollisionMap() {
        std::ifstream file("../assets/TileMap/Collision_map.bin", std::ios::binary);
        file.read((char *)collisionMap, sizeof(collisionMap));
        file.close();
    }

    static void AddCollision(int x, int y, int val) {
        collisionMap[y][x] = val;
    }

    static void DrawCollisionMap() {
        for (int i = 0; i < 25; i++) {
            for (int j = 0; j < 25; j++) {
                if (collisionMap[i][j] == 1) {
                    DrawRectangle(i * 32, j * 32, 32, 32, BLUE);
                }
            }
        }
    }

    static void SaveCollisionMap() {
        std::ofstream file("../assets/TileMap/Collision_map.bin", std::ios::binary);
        file.write((char *)collisionMap, sizeof(collisionMap));
        file.close();
    }

    static void Mapper() {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int i = GetMouseX() / 32;
            int j = GetMouseY() / 32;
            std::cout << i << " " << j << std::endl;
            CollisionMapper::AddCollision(j, i, 1);
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            int i = GetMouseX() / 32;
            int j = GetMouseY() / 32;
            CollisionMapper::AddCollision(j, i, 0);
        }
        if (IsKeyDown(KEY_SPACE)) {
            CollisionMapper::SaveCollisionMap();
        }
    }
    static int collisionMap[25][25];
};

#endif // COLLISION_MAPPER_H
