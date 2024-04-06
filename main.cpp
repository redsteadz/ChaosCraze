#include <iostream>
#include <raylib.h>
using namespace std;

const int height = 600;
const int width = 800;

class NPC {
  string texturePath;
  Texture2D npcTexture;
  Rectangle npcRectangle;
  Vector2 npcPosition;
  float npcSpeed;

public:
  NPC(string texPath, Rectangle rec, Vector2 pos, float spd) {
    npcRectangle = rec;
    npcPosition = pos;
    npcSpeed = spd;
    npcTexture = LoadTexture(texPath.c_str());
    texturePath = texPath;
  }
  void Draw() {
     DrawTextureV(npcTexture, npcPosition, WHITE);
   }
   void Animate(){
      
   }
};

int main(int argc, char *argv[]) {
  InitWindow(width, height, "raylib [models] example - npc Test");
  NPC Man = NPC("../assets/Idle.png", {0, 0, 32, 32}, {0, 0}, 2.0);
  SetExitKey(0);
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    Man.Draw();
    ClearBackground(BLACK);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
