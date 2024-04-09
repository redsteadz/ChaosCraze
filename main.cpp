#include <iostream>
#include "math.h"
#include <string>
#include <vector>
#include <raylib.h>
using namespace std;
const int width = 800;
const int height = 600;

class Occupation{
    int id;
    string name;
    public:
    Occupation(int i,string n):id(i),name(n){}
    int getid()
    {
        return id;
    }
    string getName()
    {
        return name;
    }
    void setId(int i){
        id=i;
    }
    void setName(string n){
        name=n;
    }
};
enum Actions{
    WALK=1,
    RUN=2,
    SLAP=3,
    ATTACK=4,
    DIE=5
};
class NPC{
    Actions action;
    string name;
    int age;
    int sentiment_val;
    int gender;//1=male 0=female
    Occupation occp;
    string texturePath;
    Texture2D npcTexture;
    Rectangle npcRectangle;
    Vector2 npcPosition;
    float npcSpeed;
    float direction = 0.0f;
  bool isMoving = false;
  int flip = 0;
  int frameSpeed = 6;
  int frameCount = 0;
  int currentFrame = 0;
    public:
    NPC(string texPath, Rectangle rec, Vector2 pos, float spd,string name,int age,int sent_val,int gender,Occupation &o,Actions act):name(name),age(age),sentiment_val(sent_val),gender(gender),occp(o),action(act){
    npcRectangle = rec;
    npcPosition = pos;
    npcSpeed = spd;
    npcTexture = LoadTexture(texPath.c_str());
    texturePath = texPath;
  }
  //draw
  void Draw() {
     DrawTextureV(npcTexture, npcPosition, WHITE);
   }  
   //random pos update
   void update_pos()
   {
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
   //actions
   void random_move(){
      update_pos();
   } 
   void animate_walk()
   {
      UnloadTexture(npcTexture);
      npcTexture=LoadTexture("assets/Villagers/3_Man/Man_walk.png");
   }
   void animate_slap()
   {
      UnloadTexture(npcTexture);
      npcTexture=LoadTexture("");
   }
   void animate_run(){
      UnloadTexture(npcTexture);
      npcTexture=LoadTexture("");
   }
   void animate_attack()
   {
       UnloadTexture(npcTexture);
       npcTexture=LoadTexture("");
   }
   void animate_die()
   {
       UnloadTexture(npcTexture);
       npcTexture=LoadTexture("");
   }
   //.....furtheractions
   //getter setter
   void setAction(Actions act) {
        action = act;
    }

    Actions getAction() {
        return action;
    }
   //update texture
   void Update() {
        switch(action) {
            case WALK:
                animate_walk();
                random_move();
                break;
            case RUN:
                animate_run();
                random_move();
                break;
            case SLAP:
                animate_slap();  
                break;
             case ATTACK:
                animate_attack();
                break;
            case DIE:
                animate_die();
                break;    
            default:
                random_move();
                break;
        }
    }
    //after actions like slap or attack 
    //will activate for the other npc
    void change_color()
    {
        Color tint={255, 150, 150, 255};
        DrawTextureV(npcTexture, npcPosition,tint);
    }
    
};
class Game{
    vector<NPC> npcs;
   public:
    void Create()
    {
       //Adding values in npc obj;
       Occupation o1(1,"Musketeer");
       Actions act=WALK;
    npcs.push_back(NPC("assets/Villagers/3_Man/Man.png",  {0, 0, 48, 48}, {0, 0}, 2.5,"Hamees",20,-1,1,o1,act));
    
    //till n no of npcs
    }
    void Update()
    {
         for(int i=0;i<npcs.size();i++)
         {
            npcs[i].Update();
         }
    }
    void Draw()
    {
        for(int i=0;i<npcs.size();i++)
        {
            npcs[i].Draw();
        }
    }
   void ProcessNPCs(vector<NPC> &npcs){
       for(int i=0;i<npcs.size();i++)
       {
        npcs[i].Update();
       }
   }
};
int main()
{
    InitWindow(width, height, "My first RAYLIB program!");
    SetTargetFPS(60);
    Game g;
    g.Create();
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        g.Draw();
        g.Update();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}