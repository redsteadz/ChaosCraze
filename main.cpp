#include "headers/ds.h"
#include "math.h"
#include <iostream>
#include <raylib.h>
#include <string>
#define RAYLIB_TILESON_IMPLEMENTATION
#include "raylib-tileson.h"
#include <vector>
using namespace std;
const int width = 800;
const int height = 800;
enum STATE { attack, walk, idle, hurt, death };

int collisionMap[25][25] = {0};

class CollisionMapper {

public:
  // Load Collision map from binary file
  static void LoadCollisionMap() {
    ifstream file("../assets/TileMap/Collision_map.bin", ios::binary);
    file.read((char *)collisionMap, sizeof(collisionMap));
    file.close();
  }
  static void AddCollision(int x, int y, int val) { collisionMap[y][x] = val; }
  static void DrawCollisionMap() {
    for (int i = 0; i < 25; i++)
      for (int j = 0; j < 25; j++)
        if (collisionMap[i][j] == 1)
          DrawRectangle(i * 32, j * 32, 32, 32, BLUE);
  }
  // Save collision map to a binary file
  static void SaveCollisionMap() {
    ofstream file("../assets/TileMap/Collision_map.bin", ios::binary);
    file.write((char *)collisionMap, sizeof(collisionMap));
    file.close();
  }
};

class NPC_Characteristics {
  STATE state;
  int sentiment;
  int age;
  string occupation;
  string name;

public:
  NPC_Characteristics(STATE state, int sentiment, int age, string occupation,
                      string name) {
    this->state = idle;
    this->sentiment = 0;
    this->age = age;
    this->occupation = occupation;
    this->name = name;
  }
  friend class NPC;
  virtual void setState(STATE s) { this->state = s; }
  int getSentiment() { return sentiment; }
  int getAge() { return age; }
  string getOccupation() { return occupation; }
  string getName() { return name; }
  STATE getState() { return state; }
  void setName(string name) { this->name = name; }
  void setAge(int age) { this->age = age; }
  void setOccupation(string occupation) { this->occupation = occupation; }
  void setSentiment(int sentiment) { this->sentiment = sentiment; }
};

class NPC_Physics {
  Vector2 npcPosition;
  bool colliding = false;
  float direction = 0.0f;
  Rectangle npcRectangle;
  int flip = 0;
  float npcSpeed;

public:
  void Random_walk() {

    direction +=
        (rand() % 100 - 50) * 0.01f; // Adjust the range and step size as needed

    // Calculate movement vector based on direction
    Vector2 movement = {npcSpeed * cos(direction), npcSpeed * sin(direction)};

    // Update NPC position

    npcPosition.x += movement.x;
    npcPosition.y += movement.y;

    int x = npcPosition.x / 32;
    int y = npcPosition.y / 32;

    // cout << collisionMap[x][y] << endl;
    if (collisionMap[x][y] == 1 || collisionMap[x][y + 1] == 1 || collisionMap[x + 1][y] == 1 || collisionMap[x + 1][y + 1] == 1) {
      DrawRectangleLinesEx(Rectangle{npcPosition.x, npcPosition.y, 32, 64}, 2,
                           RED);
      npcPosition.x -= movement.x;
      npcPosition.y -= movement.y;
    }

    if (movement.x < 0.0f) {
      flip = 1;
    } else {
      flip = 0;
    }

    // Ensure NPC stays within the screen boundaries
    npcPosition.x = fmax(fmin(npcPosition.x, width - npcRectangle.width), 0);
    npcPosition.y = fmax(fmin(npcPosition.y, height - npcRectangle.height), 0);

    // Check if the new position is not hitting any collision
  }
  friend class NPC;
};

class NPC : public NPC_Physics, public NPC_Characteristics {

  float timer = 0.0f;
  int frame = 0;
  float frameWidth = 0;

  bool isMoving = false;

  bool isColliding = false;

  string texturePath;
  Texture2D npcTexture;
  int frameSpeed = 6;
  int frameCount = 0;
  int currentFrame = 0;

public:
  NPC(string _name, Vector2 pos, float spd, STATE state, int sentiment, int age,
      string occupation)
      : NPC_Characteristics(state, sentiment, age, occupation, _name) {
    npcSpeed = spd;
    texturePath = "../assets/" + occupation + "/" + _name + "/" + _name;
    setState(state);
    npcPosition = pos;
    npcRectangle = {0, 0, (float)npcTexture.height, (float)npcTexture.height};
  }

  void setState(STATE s) {
    state = s;
    UnloadTexture(npcTexture);
    switch (s) {
    case walk:
      npcTexture = LoadTexture((texturePath + "_walk.png").c_str());
      break;
    case idle:
      npcTexture = LoadTexture((texturePath + "_idle.png").c_str());
      break;
    // One time
    case attack:
      npcTexture = LoadTexture((texturePath + "_attack.png").c_str());
      break;
    case hurt:
      npcTexture = LoadTexture((texturePath + "_hurt.png").c_str());
      break;
    case death:
      cout << " I CALL ONTO THE DEATH REAPER " << endl;
      npcTexture = LoadTexture((texturePath + "_death.png").c_str());
      break;
    default:
      break;
    }
    if (state == walk)
      isMoving = true;
    else
      isMoving = false;
    npcRectangle = {0, 0, (float)npcTexture.height, (float)npcTexture.height};
    frame = 0;
  }
  void Draw() {
    if (flip) {
      DrawTextureRec(npcTexture, flippedTexture(), npcPosition, WHITE);
    } else {
      DrawTextureRec(npcTexture, npcRectangle, npcPosition, WHITE);
    }
    if (isColliding) {
      DrawRectangleLinesEx(Rectangle{npcPosition.x, npcPosition.y, 32, 32}, 2,
                           GREEN);
    }
    // Ensure NPC stays within the screen boundaries
    npcPosition.x = fmax(fmin(npcPosition.x, width - npcRectangle.width), 0);
    npcPosition.y = fmax(fmin(npcPosition.y, height - npcRectangle.height), 0);
  }
  void Update() {
    Animate();
    if (isMoving)
      Random_walk();
    Draw();
  }
  void Animate() {
    int count = 0;
    int numTextures = 0;
    if (npcTexture.width > 0 && npcTexture.height > 0) {
      numTextures = npcTexture.width / npcTexture.height;
      if (numTextures > 0) {
        frameWidth = static_cast<float>(npcTexture.width / numTextures);
      }
    }
    int maxFrames = static_cast<int>(npcTexture.width / frameWidth);
    timer += GetFrameTime();
    if (timer >= 0.3f) {
      timer = 0.0f;
      frame += 1;
      if (state == walk || state == idle)
        frame = frame % maxFrames;
      else {
        if (frame >= maxFrames) {
          frame = maxFrames - 1;
          if (state == attack || state == hurt)
            setState(walk);
        }
      }
    }

    // cout << frame << endl;
    npcRectangle = {frameWidth * frame, 0, frameWidth,
                    static_cast<float>(npcTexture.height)};
  }
  Rectangle GetRect() { return npcRectangle; }
  Vector2 GetPos() { return npcPosition; }
  void setColliding(bool b) { isColliding = b; }
  bool getColliding() { return isColliding; }

private:
  Rectangle flippedTexture() {
    Rectangle npcFlipped = npcRectangle;
    npcFlipped.x =
        npcRectangle.x + npcRectangle.width; // Set x-coordinate to right edge
    npcFlipped.x -= 14;
    npcFlipped.width = -npcRectangle.width; // Invert width to flip horizontally
    return npcFlipped;
  }
};

class NPC_Interactions {
  vector<NPC *> npcList;
  QuadTree<NPC> *Q;

public:
  bool run = true;
  void AddNPC(NPC *npc) { npcList.push_back(npc); }

  vector<pair<NPC *, NPC *>> CheckCollisions() {
    vector<pair<NPC *, NPC *>> collisions;
    for (NPC *npc : npcList) {
      bool flag = false;
      Circle c = {npc->GetPos(), 60};
      c.center.x += npc->GetRect().width / 2 - 10;
      c.center.y += npc->GetRect().height / 2;
      // DrawCircleLinesV(c.center, c.radius, RED);
      vector<Point<NPC>> collided = Q->query(c);
      // cout << npc->GetName() << " " << collided.size() << endl;
      for (Point other : collided) {
        if (other.data != npc) {
          // cout << npc->getName() << " COLLIDING " << other.data->getName() <<
          // endl;
          collisions.push_back({npc, other.data});
          // flag = true;
          // break;
        }
      }
      // if (flag) {
      //   npc->setColliding(true);
      // } else {
      //   npc->setColliding(false);
      // }
    }
    return collisions;
  }

  vector<Point<NPC>> QueryRec(Rectangle r) { return Q->query(r); }

  void ChangeSentimentVal(string target, int val) {
    for (NPC *npc : npcList) {
      if (npc->getName() == target || npc->getOccupation() == target) {
        npc->setSentiment(val);
      }
    }
  }

  void Draw() {
    Q = new QuadTree<NPC>(Rectangle{0, 0, width, height}, 4);
    for (NPC *npc : npcList) {
      npc->Draw();
      Point<NPC> p = {npc->GetPos(), npc};
      Q->insert(p);
      // cout << npc->GetName() << " " << p.v.x << " " << p.v.y << endl;
    }
    // Q->Draw();
  }
  void Update() {
    if (run) {
      for (NPC *npc : npcList) {
        npc->Update();
      }
    }
    delete Q;
  }
};

int main() {
  InitWindow(width, height, "My first RAYLIB program!");
  SetTargetFPS(60);
  // Seed for random number
  srand(time(NULL));
  //  NPC(string _name, Vector2 pos, float spd, STATE state, int sentiment, int
  //  age, string occupation)
  NPC_Interactions Game;

  CollisionMapper::LoadCollisionMap();

  NPC Boy("Boy", {100, 200}, 2, STATE::walk, 0, 0, "Villagers");
  NPC Girl("Girl", {100, 200}, 2, STATE::walk, 0, 0, "Villagers");
  NPC Old_Man("Old_man", {100, 200}, 2, STATE::walk, 0, 0, "Villagers");
  NPC Man("Man", {100, 200}, 2, STATE::walk, 0, 0, "Villagers");
  NPC Woman("Woman", {100, 200}, 2, STATE::walk, 0, 0, "Villagers");

  Game.AddNPC(&Boy);
  Game.AddNPC(&Girl);
  Game.AddNPC(&Old_Man);
  Game.AddNPC(&Man);
  Game.AddNPC(&Woman);

  Map map = LoadTiled("../assets/TileMap/Final.json");

  int stateC = 4;

  STATE state_list[] = {idle, walk, attack, hurt, death};
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTiled(map, 0, 0, WHITE);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      // Find the i j according to a grid of 32 x 32
      int i = GetMouseX() / 32;
      int j = GetMouseY() / 32;
      cout << i << " " << j << endl;
      CollisionMapper::AddCollision(j, i, 1);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      int i = GetMouseX() / 32;
      int j = GetMouseY() / 32;
      CollisionMapper::AddCollision(j, i, 0);
    }
    // Draw the CollisionMap over it as blue
    // CollisionMapper::DrawCollisionMap();
    // On Space Save the collision map
    if (IsKeyDown(KEY_SPACE)) {
      CollisionMapper::SaveCollisionMap();
    }
    // CollisionMapper::DrawCollisionMap();
    Game.Draw();
    Game.CheckCollisions();
    Game.Update();
    EndDrawing();
  }
  UnloadMap(map);
  CloseWindow();
  return 0;
}
