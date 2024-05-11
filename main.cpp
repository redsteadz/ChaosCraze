#include "headers/UI.h"
#include "headers/collisions.h"
#include "headers/ds.h"
#define GUI_STATUSBAR_IMPLEMENTATION
#include "headers/gui_statusBar.h"
#include "math.h"
// Semtiment -ve -> MORE
// Age Game SMALLE -> MORE
// OCC 1 -> MORE
#include <cstring>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#define RAYLIB_TILESON_IMPLEMENTATION
#include "raylib-tileson.h"
#include <chrono>
#include <random>
#include <unistd.h> // For getpid()
#include <vector>

using namespace std;
const int width = 800;
const int height = 800;
enum STATE { attack, walk, idle, hurt, death };

int CollisionMapper::collisionMap[25][25] = {0};

class CameraController {
  // Make a camera Object
  Camera2D camera;

public:
  Camera2D *getCamera() { return &this->camera; }
  CameraController() {
    camera.target = GetMousePosition();
    camera.offset = (Vector2){0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
  }

  void BeginCamera() { BeginMode2D(camera); }
  void EndCamera() { EndMode2D(); }

  void MoveCamera() {
    // Right click drag to move the camera
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      Vector2 mouseDelta = GetMouseDelta();
      camera.offset.x += mouseDelta.x;
      camera.offset.y += mouseDelta.y;
    }
  }

  void ZoomControl() {
    // Zoom based on mouse wheel
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

      // Top left Corner of the screen

      // Set the offset to where the mouse is
      camera.offset = GetMousePosition();
      // Set the target to match, so that the camera maps the world space point
      // under the cursor to the screen space point under the cursor at any zoom
      camera.target = mouseWorldPos;

      // Zoom increment
      const float zoomIncrement = 0.25f;

      camera.zoom += (wheel * zoomIncrement);
      if (camera.zoom < zoomIncrement)
        camera.zoom = zoomIncrement;
      if (camera.zoom < 1)
        camera.zoom = 1;
    }
  }

  void Control() {
    ZoomControl();
    MoveCamera();
    CheckBounds();
  }

  void CheckBounds() {
    Vector2 TopLeft = GetScreenToWorld2D(Vector2{0, 0}, camera);
    Vector2 BottomRight = GetScreenToWorld2D(Vector2{800, 800}, camera);

    // Check if the camera goes out of bounds
    // cout << TopLeft.x << " " << TopLeft.y << " " << BottomRight.x << " " <<
    // BottomRight.y << endl;
    if (TopLeft.x < 0) {
      camera.offset.x += TopLeft.x;
    }
    if (TopLeft.y < 0) {
      camera.offset.y += TopLeft.y;
    }
    if (BottomRight.x > 800) {
      camera.offset.x -= 800 - BottomRight.x;
    }
    if (BottomRight.y > 800) {
      camera.offset.y -= 800 - BottomRight.y;
    }

    // If the camera is out of bounds, adjust its position
    // if (outOfBounds) {
    //   camera.target = GetScreenToWorld2D(
    //       Vector2{800 / 2, 800 / 2}, camera);
    // }

    this->camera = camera;
  }

  // Control that Camera Object the map
  // Right click drag changes the Camera
};

class NPC_Characteristics {
public:
  STATE state;
  float sentiment;
  int age;
  string occupation;
  string name;
  int health;

  NPC_Characteristics(STATE state, float sentiment, int age, string occupation,
                      string name) {
    this->state = idle;
    this->sentiment = sentiment;
    this->age = age;
    this->occupation = occupation;
    this->health = 100;
    this->name = name;
  }
  friend class NPC;
  virtual void setState(STATE s) { this->state = s; }
  float getSentiment() { return sentiment; }
  int getAge() { return age; }
  string getOccupation() { return occupation; }
  string getName() { return name; }
  STATE getState() { return state; }
  void setName(string name) { this->name = name; }
  void setAge(int age) { this->age = age; }
  void setOccupation(string occupation) { this->occupation = occupation; }
  void setSentiment(float sentiment) { this->sentiment = sentiment; }
  void setHealth(int health) {
    if (health < this->health)
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
    if (CollisionMapper::collisionMap[x][y] == 1 ||
        CollisionMapper::collisionMap[x][y + 1] == 1 ||
        CollisionMapper::collisionMap[x + 1][y] == 1 ||
        CollisionMapper::collisionMap[x + 1][y + 1] == 1) {
      // DrawRectangleLinesEx(Rectangle{npcPosition.x, npcPosition.y, 32, 64},
      // 2,
      //                      RED);
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

  GuiStatusBarState StatusState = InitGuiStatusBar();

public:
  NPC(string _name, Vector2 pos, float spd, STATE state, float sentiment,
      int age, string occupation)
      : NPC_Characteristics(state, sentiment, age, occupation, _name) {
    npcSpeed = spd;
    texturePath = "../assets/" + occupation + "/" + _name + "/" + _name;
    StatusState.name = new char[_name.length() + 1];
    strcpy(StatusState.name, _name.c_str());
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
    DrawOnHover();
  }
  void DrawOnHover() {
    Vector2 mousePos = {(float)GetMouseX(), (float)GetMouseY()};
    if (CheckCollisionPointRec(mousePos, Rectangle{npcPosition.x, npcPosition.y,
                                                   npcRectangle.width,
                                                   npcRectangle.height})) {

      StatusState.anchor01 = npcPosition;
      StatusState.layoutRecs[0] =
          (Rectangle){npcPosition.x + -64, npcPosition.y + -24, 128, 24};
      StatusState.layoutRecs[1] =
          (Rectangle){npcPosition.x + -48, npcPosition.y + -16, 104, 12};
      StatusState.ProgressBar001Value = health / 100.0;
      // cout << " HELLO " << endl;
      GUIStatusBar(&StatusState);
    }
  }
  void Update() {
    Animate();
    if (isMoving)
      Random_walk();
  }
  void Animate() {
    int count = 0;
    int numTextures = 0;
    if (npcTexture.width > 0 && npcTexture.height > 0) {
      numTextures = npcTexture.width / npcTexture.height;
      if (numTextures > 0) {
        frameWidth = static_cast<float>((float)npcTexture.width / numTextures);
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
      // cout << name << " " << frame << endl;
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
    npcFlipped.x -= (state == death) ? 0 : 12;
    npcFlipped.width = -1 * npcRectangle.width;
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
      Circle c = {npc->GetPos(), 20};
      c.center.x += npc->GetRect().width / 2 - 10;
      c.center.y += npc->GetRect().height / 2;
      // DrawCircleLinesV(c.center, c.radius, RED);
      vector<Point<NPC>> collided = Q->query(c);
      // cout << collided.size() << endl;
      // cout << npc->GetName() << " " << collided.size() << endl;
      for (Point<NPC> other : collided) {
        if (other.data != npc) {
          // cout << npc->getName() << " COLLIDING " << other.data->getName() <<
          // endl;
          collisions.push_back({npc, other.data});
          // flag = true;
          // break;
        }
      }
    }
    return collisions;
  }

  vector<Point<NPC>> QueryRec(Rectangle r) { return Q->query(r); }

  double calculateProbability(double s1, double s2, double g, double o) {
    double y = (1.0 - (3.0 * (s1 + s2) / 2.0)) * (0.25 - (g / 48.0)) + o / 2.0 +
               1.0 / (1.0 + g);
    return y;
  }
  unsigned long long generateSeed() {
    // Get the current time since epoch
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    unsigned long long seed =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    // Add process ID to the seed
    seed += static_cast<unsigned long long>(getpid());

    return seed;
  }

  bool probabilityBasedFunction(double probability) {
    // Generate a seed
    unsigned long long seed = generateSeed();

    // Initialize a random number generator with the seed
    std::mt19937 gen(seed);
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // Generate a random number between 0 and 1
    double randomValue = dis(gen);

    // Check if the random number is less than the probability
    return randomValue < probability;
  }

  bool ActionProbablity(NPC *npc1, NPC *npc2) {
    int g = abs(npc1->age - npc2->age);
    int o;
    if (npc1->occupation != npc2->occupation) {
      o = 1;
    } else {
      o = 0;
    }

    float y =
        calculateProbability(npc1->sentiment, npc2->sentiment, (g + 3), o) /
        30.0;
    bool val = probabilityBasedFunction(y);
    // cout << y << " " << val << endl;
    if (val) {
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
        // cout << npc->getName() << ": " << npc->getSentiment() << endl;
        npc->Update();
      }
    }
    delete Q;
  }
};

class Game : public NPC_Interactions, public UI {
  static int deathCount;

public:
  Game() {}
  void DrawUI(){
    UI::Draw();
  }
  void DrawNPC(){
    NPC_Interactions::Draw();
  }
  void Draw() {
    DrawNPC();
    DrawUI();
  }
  void Random_interactions() {
    vector<pair<NPC *, NPC *>> pairList = CheckCollisions();
    // cout << pairList.size() << endl;
    for (auto o : pairList) {
      bool stuff = ActionProbablity(o.first, o.second);
      // cout << stuff << " " << o.first->getName() << " " <<
      // o.second->getName()
      // << endl;
      if (stuff && o.first->getState() != death &&
          o.first->getState() != attack && o.second->getState() != death &&
          o.second->getState() != attack) {
        if ((o.first->getSentiment() >= -1 && o.first->getSentiment() <= 1) ||
            (o.second->getSentiment() >= -1 && o.second->getSentiment() <= 1)) {
          if (o.first->getSentiment() > o.second->getSentiment() ||
              o.first->getSentiment() == o.second->getSentiment()) {
            o.second->setState(attack);
            o.first->setState(hurt);
            if (o.first->getSentiment() > 0)
              o.first->setHealth(15);
            else if (o.first->getSentiment() < 0)
              o.first->setHealth(25);
            else
              o.first->setHealth(10);
          } else if (o.first->getSentiment() < o.second->getSentiment()) {
            o.first->setState(attack);
            o.second->setState(hurt);
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
  void Update() {
    if (UI::getPostedState()) {
      // Grab the second Noun
      // Grab the connect
      string target = DropdownNoun2[ActiveDropdownNoun2()];
      string adj = DropdownConnector[ActiveDropdownConnector()];
      if (neg.count(adj) > 0) {
        // Decrease the sentiment
        // Calculate the sentiment value for the targets
        ChangeSentimentVal(target, -1);
      } else {
        ChangeSentimentVal(target, 1);
      }
      UI::setPostedState(0);
    }
    if (capture == 2) {
      // It has captured some shit, Grab the npcs from rectangle
      Rectangle captured = rect;
      // cout << captured.x << " " << captured.y << " " << captured.width << " "
      // << captured.height << endl;
      vector<Point<NPC>> collided = QueryRec(captured);
      vector<string> names;
      for (Point<NPC> npc : collided) {
        // Update the options in the caption
        if (npc.data->state == attack || npc.data->state == hurt)
          names.push_back(npc.data->getName());
        // cout << npc.data->getName() << " ";
      }
      vector<string> defaultCaption = {"Take", "Caption"};
      if (!names.empty())
        setCaption(names);
      else
        setCaption(defaultCaption);

      UI::capture = 0;
    }
    Random_interactions();
    NPC_Interactions::Update();
  }
};

int Game::deathCount = 0;

int main() {
  InitWindow(width, height, "My first RAYLIB program!");
  SetTargetFPS(60);
  // Seed for random number

  srand(time(NULL));
  //  NPC(string _name, Vector2 pos, float spd, STATE state, int sentiment, int
  //  age, string occupation)
  // NPC_Interactions Game;
  Game Game;

  CameraController c;
  CollisionMapper::LoadCollisionMap();

  NPC Boy("Boy", {100, 200}, 2, STATE::walk, -0.8, 0, "Villagers");
  NPC Girl("Girl", {100, 200}, 2, STATE::walk, 0, 0, "Villagers");
  NPC Old_Man("Old_man", {100, 200}, 2, STATE::walk, -0.8, 0, "Villagers");
  NPC Man("Man", {100, 200}, 2, STATE::walk, 0, 0, "Villagers");
  NPC Woman("Woman", {100, 200}, 2, STATE::walk, 0, 0, "Villagers");

  Game.AddNPC(&Boy);
  Game.AddNPC(&Girl);
  Game.AddNPC(&Old_Man);
  Game.AddNPC(&Man);
  Game.AddNPC(&Woman);

  Map map = LoadTiled("../assets/TileMap/Final.json");
  // int stateC = 4;
  // STATE state_list[] = {idle, walk, attack, hurt, death};
  while (!WindowShouldClose()) {
    c.Control();
    BeginDrawing();
    ClearBackground(WHITE);
    c.BeginCamera();
    DrawTiled(map, 0, 0, WHITE);
    Game.DrawNPC();
    // CollisionMapper::DrawCollisionMap();
    c.EndCamera();
    Game.DrawUI();
    EndDrawing();
    Game.HandleCapture();
    Game.Update();
  }
  UnloadMap(map);
  CloseWindow();
  return 0;
}
