#include "math.h"
#include <iostream>
#include <raylib.h>
#include <string>
#include <vector>
using namespace std;
const int width = 800;
const int height = 600;
enum STATE { attack, walk, idle, hurt };

class NPC_Characteristics {
  STATE state;
  int sentiment;
  int age;
  string occupation;
  string name;

public:
NPC_Characteristics(STATE state,int sentiment,int age,string occupation,string name){
  this->state;
  this->sentiment;
  this->age=age;
  this->occupation=occupation;
  this->name=name;
}
  friend class NPC;
  virtual void setState(STATE s){
    this->state=s;
  }
  int getSentiment(){
    return sentiment;
  }
  int getAge(){
    return age;
  }
  string getOccupation(){
    return occupation;
  }
  string getName(){
       return name;
  }
  STATE getState(){
    return state;
  }
  void setName(string name){
    this->name=name;
  }
  void setAge(int age){
    this->age=age;
  }
  void setOccupation(string occupation){
     this->occupation=occupation;
  }
  void setSentiment(int sentiment){
    this->sentiment=sentiment;
  }
  
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

  bool isMoving = false;

  string texturePath;
  Texture2D npcTexture;
  int frameSpeed = 6;
  int frameCount = 0;
  int currentFrame = 0;

public:
  NPC(string folderPath, string _name, Vector2 pos, float spd, STATE state,
      int sentiment, int age, string occupation):NPC_Characteristics(state,sentiment,age,occupation,_name){
    npcSpeed = spd;
    texturePath = folderPath + "/" + _name;
    setState(state);
    npcPosition=pos;
    npcRectangle={0, 0, (float)npcTexture.height, (float)npcTexture.height};
  }

  void setState(STATE s){
    state = s;
    UnloadTexture(npcTexture);
    switch (s) {
    case attack:
      npcTexture = LoadTexture((texturePath + "_attack.png").c_str());
      break;
    case walk:
      npcTexture = LoadTexture((texturePath + "_walk.png").c_str());
      break;
    case idle:
      npcTexture = LoadTexture((texturePath + "_idle.png").c_str());
      break;
    case hurt:
      npcTexture = LoadTexture((texturePath + "_hurt.png").c_str());
      break;
    default:
      break;
    }
    if (state == walk)
      isMoving = true;
    else
      isMoving = false;
    npcRectangle = {0, 0, (float)npcTexture.height, (float)npcTexture.height};
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

    static float timer = 0.0f;
    static int frame = 0;
    float frameWidth = 0;
    if (npcTexture.width > 0 && npcTexture.height > 0) {
      int numTextures = npcTexture.width / npcTexture.height;
      if (numTextures > 0) {
        frameWidth = static_cast<float>(npcTexture.width / numTextures);
      }
    }
    int maxFrames = static_cast<int>(npcTexture.width / frameWidth);

    timer += GetFrameTime();
    if (timer >= 0.2f) {
      timer = 0.0f;
      frame += 1;
    }
    frame = frame % maxFrames;
    npcRectangle = {frame * frameWidth, 0, frameWidth,
                    (float)npcTexture.height};
  }

private:
  Rectangle flippedTexture() {
    Rectangle npcFlipped = npcRectangle;
    npcFlipped.x =
        npcRectangle.x + npcRectangle.width; // Set x-coordinate to right edge
    npcFlipped.x -= 24;
    npcFlipped.width = -npcRectangle.width; // Invert width to flip horizontally
    return npcFlipped;
  }
};

class Game {
  vector<NPC> npcs;
  
public:
  void Create() {
    // Adding values in npc obj;

    STATE state = idle;
    // npcs.push_back(NPC("assets/Villagers/3_Man", "Man", {0, 0, 48, 48}, {40,
    // 40}, 2.5,state,-1,17,"Man"));

    // till n no of npcs
  }
  void Update() {
    for (int i = 0; i < npcs.size(); i++) {
      STATE state = attack;
      npcs[i].setState(state);
    }
  }
  void Draw() {
    for (int i = 0; i < npcs.size(); i++) {
      STATE state = attack;
      npcs[i].Draw();
      npcs[i].setState(state);
    }
  }
  void ProcessNPCs(vector<NPC> &npcs) {
    for (int i = 0; i < npcs.size(); i++) {
      //  npcs[i].Update();
    }
  }
};
int main() {
  InitWindow(width, height, "My first RAYLIB program!");
  SetTargetFPS(60);
  Game g;
  // g.Create();
  // g.Draw();
  NPC n("../assets/Villagers/3_Man", "Man", {40, 40}, 2.5, STATE::idle, -1, 17,
        "Man");
  // List of states
  int stateC = 0;
  STATE state_list[] = {idle, walk, attack, hurt};
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    // g.Update();
    if (IsKeyPressed(KEY_SPACE)) {
      n.setState(state_list[(++stateC) % 4]);
    }
    n.Update();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
