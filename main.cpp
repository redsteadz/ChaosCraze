#include "headers/collisions.h"
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

class NPC_Characteristics {
public:
  STATE state;
  int sentiment;
  int age;
  string occupation;
  string name;
  int health;

  NPC_Characteristics(STATE state, int sentiment, int age, string occupation,
                      string name) {
    this->state = idle;
    this->sentiment = 0;
    this->age = age;
    this->occupation = occupation;
    this->health = 100;
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
  void setHealth(int health) {
    if (health >= this->health)
      this->health -= health;
    else
      this->health = 0;
  }
  int getHealth() { return this->health; }
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
    if (collisionMap[x][y] == 1 || collisionMap[x][y + 1] == 1 ||
        collisionMap[x + 1][y] == 1 || collisionMap[x + 1][y + 1] == 1) {
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
    npcFlipped.x -= 20;
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

  bool ActionProbablity(NPC *npc1, NPC *npc2) {
    int g = abs(npc1->age - npc2->age);
     int o;
    if (npc1->occupation != npc2->occupation) {
      int o = 1;
    } else {
      o = 0;
    }
    int y =
        (-(4 * npc1->sentiment) - (4 * npc2->sentiment) - (g / 3) + o + 14) /
        22;
    if (y >= 0.5) {
      return true;
    } else {
      return false;
    }
  }
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
class Game: public NPC_Interactions {
  static int deathCount;

public:
  void Random_interactions() {
    vector<pair<NPC *, NPC *>> pairList = CheckCollisions();
    for (auto o : pairList) {
      if (ActionProbablity(o.first, o.second) &&
          o.first->getState() != death && o.first->getState() != attack &&
          o.second->getState() != death && o.second->getState() != attack) {
        if (o.first->getSentiment() >= -1 && o.first->getSentiment() <= 1 &&
            o.second->getSentiment() >= -1 && o.second->getSentiment() <= 1) {
          if (o.first->getSentiment() > o.second->getSentiment() ||
              o.first->getSentiment() == o.second->getSentiment()) {
            o.second->setState(attack);
            if (o.first->getSentiment() > 0)
              o.first->setHealth(15);
            else if (o.first->getSentiment() < 0)
              o.first->setHealth(25);
            else
              o.first->setHealth(10);
          } else if (o.first->getSentiment() < o.second->getSentiment()) {
            o.first->setState(attack);
            if (o.second->getSentiment() > 0)

              o.second->setHealth(15);

            else if (o.second->getSentiment() < 0)

              o.second->setHealth(25);

            else

              o.second->setHealth(10);
          }
        }
        if (o.first->getHealth() == 0) {
          o.first->setState(death);
          deathCount++;
        } else if (o.second->getHealth() == 0) {
          o.second->setState(death);
          deathCount++;
        }
      }
    }
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
