#include <bits/stdc++.h>
#include <cmath>
#include <iostream>
#include <raylib.h>
#include <string>
using namespace std;

const int height = 600;
const int width = 800;

class NPC {
  string texturePath, name;
  Texture2D npcTexture;
  Texture2D npcWalkTex;
  Rectangle npcRectangle;
  Vector2 npcPosition;
  bool colliding = false;
  float direction = 0.0f;
  bool isMoving = false;
  int flip = 0;
  int frameSpeed = 6;
  int frameCount = 0;
  int currentFrame = 0;
  float npcSpeed;

public:
  NPC(string texPath, string _name, Rectangle rec, Vector2 pos, float spd) {
    npcRectangle = rec;
    npcPosition = pos;
    npcSpeed = spd;
    name = _name;
    npcTexture = LoadTexture((texPath + "/" + name + ".png").c_str());
    npcWalkTex = LoadTexture((texPath + "/" + name + "_walk.png").c_str());
    texturePath = texPath;
  }
  ~NPC() {
    UnloadTexture(npcTexture);
    UnloadTexture(npcWalkTex);
  }
  void Draw() {
    // Draw a circle with a low opacity that is blue when not colliding and red
    // when is colliding
    Vector2 center = this->GetPos();
    center.x += this->GetRect().width / 2 - 10;
    center.y += this->GetRect().height / 2;

    DrawCircleLinesV(center, 60, colliding ? RED : BLUE);
    if (flip) {
      Rectangle npcFlipped = npcRectangle;
      npcFlipped.x =
          npcRectangle.x + npcRectangle.width; // Set x-coordinate to right edge
      npcFlipped.x -= 20;
      npcFlipped.width =
          -npcRectangle.width; // Invert width to flip horizontally
      DrawTextureRec(npcWalkTex, npcFlipped, npcPosition, WHITE);
    } else {
      DrawTextureRec(npcWalkTex, npcRectangle, npcPosition, WHITE);
    }
  }
  void Update() {
    // Adjust direction randomly within a small range
    isMoving = GetRandomValue(0, 100) > 25;
    if (isMoving) {
      direction += (rand() % 100 - 50) *
                   0.01f; // Adjust the range and step size as needed

      // Calculate movement vector based on direction
      Vector2 movement = {npcSpeed * cos(direction), npcSpeed * sin(direction)};

      // Update NPC position
      npcPosition.x += movement.x;
      npcPosition.y += movement.y;
      if (movement.x < 0.0f) {
        flip = 1;
      } else {
        flip = 0;
      }
      // Ensure NPC stays within the screen boundaries
      npcPosition.x = fmax(fmin(npcPosition.x, width - npcRectangle.width), 0);
      npcPosition.y =
          fmax(fmin(npcPosition.y, height - npcRectangle.height), 0);
      frameCount++;
      if (frameCount >= (60 / frameSpeed)) {
        frameCount = 0;
        currentFrame++;
        currentFrame = currentFrame % 5;
        npcRectangle.x = currentFrame * 48;
      }
    }
  }
  // Get and set functions
  void setColliding(bool colliding) { this->colliding = colliding; }
  Rectangle GetRect() { return npcRectangle; }
  Vector2 GetPos() { return npcPosition; }
  string GetName() { return name; }
};

class NPC_Interactions {
  vector<NPC *> npcList;

public:
  void AddNPC(NPC *npc) { npcList.push_back(npc); }
  void CheckCollisions() {
    for (int i = 0; i < npcList.size(); i++) {
      bool flag = false;
      for (int j = 0; j < npcList.size(); j++) {
        if (i == j) continue;
        Vector2 npc1 = npcList[i]->GetPos();
        npc1.x += npcList[i]->GetRect().width / 2 - 10;
        npc1.y += npcList[i]->GetRect().height / 2;

        Vector2 npc2 = npcList[j]->GetPos();
        npc2.x += npcList[j]->GetRect().width / 2 - 10;
        npc2.y += npcList[j]->GetRect().height / 2;

        if (CheckCollisionCircles(npc1, 60, npc2, 60)) {
          // cout << "Collision between " << npcList[i]->GetName() << " and "
          //      << npcList[j]->GetName() << endl;
          flag = true;
          npcList[i]->setColliding(true);
          npcList[j]->setColliding(true);
        } 
      }
      if (!flag) npcList[i]->setColliding(false);
    }
  }
  void Draw() {
    for (NPC *npc : npcList) {
      npc->Draw();
    }
  }
  void Update() {
    for (NPC *npc : npcList) {
      npc->Update();
    }
  }
};

int main(int argc, char *argv[]) {
  InitWindow(width, height, "raylib [models] example - npc Test");
  NPC Man =
      NPC("../assets/Villagers/3_Man", "Man", {0, 0, 48, 48}, {0, 0}, 2.5);
  NPC Boy =
      NPC("../assets/Villagers/5_Boy", "Boy", {0, 0, 48, 48}, {10, 20}, 2.5);
  NPC OldMan = NPC("../assets/Villagers/1_Old_man", "Old_man", {0, 0, 48, 48},
                   {0, 0}, 2.5);
  NPC OldWoman = NPC("../assets/Villagers/2_Old_woman", "Old_woman",
                     {0, 0, 48, 48}, {0, 0}, 2.5);
  NPC Woman =
      NPC("../assets/Villagers/4_Woman", "Woman", {0, 0, 48, 48}, {0, 0}, 2.5);
  NPC Girl =
      NPC("../assets/Villagers/6_Girl", "Girl", {0, 0, 48, 48}, {0, 0}, 2.5);

  NPC_Interactions npcList;
  npcList.AddNPC(&Man);
  npcList.AddNPC(&Boy);
  npcList.AddNPC(&OldMan);
  npcList.AddNPC(&OldWoman);
  npcList.AddNPC(&Woman);
  npcList.AddNPC(&Girl);
  SetExitKey(0);
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    // Man.Draw();
    // Boy.Draw();
    npcList.Draw();
    npcList.Update();
    npcList.CheckCollisions();
    ClearBackground(RAYWHITE);
    // Man.Update();
    // Boy.Update();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
