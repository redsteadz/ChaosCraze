#include "math.h"
#include "headers/ds.h"
#include <bits/stdc++.h>
#include "headers/UI.h"
#include <iostream>
#include <raylib.h>
#include <string>
#include <vector>
using namespace std;
const int width = 800;
const int height = 600;
enum STATE { attack, walk, idle, hurt, death };

class NPC_Characteristics {
  STATE state;
  int sentiment;
  int age;
  string occupation;
  string name;
  int health;

public:
  NPC_Characteristics(STATE state, int sentiment, int age, string occupation,
                      string name) {
    this->state = idle;
    this->sentiment = 0;
    this->age = age;
    this->occupation = occupation;
    this->name = name;
    this->health=100;
  }
  friend class NPC;
  virtual void setState(STATE s) { this->state = s; }
  int getSentiment() { return sentiment; }
  int getAge() { return age; }
  string getOccupation() { return occupation; }
  string getName() { return name; }
  STATE getState() { return state; }
  int getHealth(){ return health; }
  void setHealth(int health){ this->health=health; }
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
    if (movement.x < 0.0f) {
      flip = 1;
    } else {
      flip = 0;
    }
    // Ensure NPC stays within the screen boundaries
    npcPosition.x = fmax(fmin(npcPosition.x, width - npcRectangle.width), 0);
    npcPosition.y = fmax(fmin(npcPosition.y, height - npcRectangle.height), 0);
  }
  friend class NPC;
};

class NPC : public NPC_Physics, NPC_Characteristics {

  float timer = 0.0f;
  int frame = 0;
  float frameWidth = 0;

  bool isMoving = false;

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
        if (frame >= maxFrames)
          frame = maxFrames - 1;
      }
    }

    cout << frame << endl;
    npcRectangle = {frameWidth * frame, 0, frameWidth,
                    static_cast<float>(npcTexture.height)};
  }
  Rectangle GetRect() { return npcRectangle; }
  Vector2 GetPos() { return npcPosition; }
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
  NPC_Interactions(){
    int attackers_num=10;
    int healers_num=30;
    int villagers_num=70;
    for(int i=0;i<10;i++){
      npcList.push_back(new NPC("Snuffy", {40, 40}, 2.5, STATE::idle, -1, 17, "Greenzone"));
    }
    for(int i=0;i<10;i++){
      npcList.push_back(new NPC("Huffy", {40, 40}, 2.5, STATE::idle, -1, 17, "Greenzone"));
    }
    for(int i=0;i<10;i++){
      npcList.push_back(new NPC("Bob", {40, 40}, 2.5, STATE::idle, -1, 17, "Greenzone"));
    }
    for(int i=0;i<3;i++){
      npcList.push_back(new NPC("Woodcutter", {40, 40}, 2.5, STATE::idle, -1, 17, "MainCharacters"));
    }
    for(int i=0;i<3;i++){
      npcList.push_back(new NPC("SteamMan", {40, 40}, 2.5, STATE::idle, -1, 17, "MainCharacters"));
    }
    for(int i=0;i<4;i++){
      npcList.push_back(new NPC("GraveRobber", {40, 40}, 2.5, STATE::idle, -1, 17, "MainCharacters"));
    }
    for(int i=0;i<13;i++){
      npcList.push_back(new NPC("Man", {40, 40}, 2.5, STATE::idle, -1, 17, "Villagers"));
    }
    for(int i=0;i<13;i++){
      npcList.push_back(new NPC("Woman", {40, 40}, 2.5, STATE::idle, -1, 17, "Villagers"));
    }
    for(int i=0;i<10;i++){
      npcList.push_back(new NPC("Old_man", {40, 40}, 2.5, STATE::idle, -1, 17, "Villagers"));
    }
    for(int i=0;i<10;i++){
      npcList.push_back(new NPC("Old_woman", {40, 40}, 2.5, STATE::idle, -1, 17, "Villagers"));
    }
    for(int i=0;i<12;i++){
      npcList.push_back(new NPC("Girl", {40, 40}, 2.5, STATE::idle, -1, 17, "Villagers"));
    }
    for(int i=0;i<12;i++){
      npcList.push_back(new NPC("Boy", {40, 40}, 2.5, STATE::idle, -1, 17, "Villagers"));
    }
  }
  bool run = true;
  void AddNPC(NPC *npc) { npcList.push_back(npc); }
 bool ActionProbablity(NPC *npc1,NPC *npc2){
  int g=abs(npc1->age - npc2->age);
  if(npc1->occupation != npc2->occupation){
    int o=1;
  }
  else{
    o=0;
  }
     int y=(-(4*npc1->sentiment)-(4*npc2->sentiment)-(g/3)+o+14)/22;
     if(y>=0.5){
        return true;
     }
     else{
      return false;
     }
 }
  void CheckCollisions() {
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
          flag = true;
          // npc->setColliding(true);
          // other.data->setColliding(true);
        }
      }

      if (!flag) {
        // npc->setColliding(false);
      }
    }
    delete Q;
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
  }
};

int main() {
  InitWindow(width, height, "My first RAYLIB program!");
  SetTargetFPS(60);
  // g.Create();
  // g.Draw();
  // Villager, Name,

  NPC n("Woodcutter", {40, 40}, 2.5, STATE::idle, -1, 17, "MainCharacters");
  // List of states
  int stateC = 4;
  STATE state_list[] = {idle, walk, attack, hurt, death};
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    // g.Update();
    if (IsKeyPressed(KEY_SPACE)) {
      n.setState(state_list[(++stateC) % 5]);
    }
    n.Update();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
