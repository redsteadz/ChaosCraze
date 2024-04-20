#include <iostream>
#include "math.h"
#include <string>
#include <vector>
#include <raylib.h>
using namespace std;
const int width = 800;
const int height = 600;
enum STATE{
    attack,
    walk,
    idle,
    hurt
};
class NPC_Walk{
    Vector2 position;
    public:
    NPC_Walk(){}
    void Random_walk(Vector2& npcPos)
    {
        position=npcPos;
       int direction=GetRandomValue(0,3);

       switch(direction){
        case 0:
          position.y-=1;//Upward
          break;
        case 1:
          position.y+=1;//Downward
          break;
        case 2:
          position.x-=1;//Leftward
          break;
        case 3:
          position.x+=1;
          break;
        default:
          break;        
       }
    }
    Vector2 getPosition(){
        return position;
    }
};
class NPC{
    NPC_Walk npc_walk;
    STATE state;
    int sentiment;
    int age;
    string occupation;
    string texturePath;
    string name;
  Texture2D npcTexture;
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
  NPC(string texPath, string _name,Rectangle rec, Vector2 pos, float spd,STATE state,int sentiment,int age,string occupation) {
    npcRectangle = rec;
    npcPosition = pos;
    npcSpeed = spd;
    texturePath = texPath;
    name=_name;
    npcTexture = LoadTexture((texturePath + "/" + name + ".png").c_str());
    this->state=state;
    this->sentiment=sentiment;
    this->age=age;
    this->occupation=occupation;
  }
  
  void setState(STATE s)
  {
     state=s;
     UnloadTexture(npcTexture);
     switch (s)
     {
     case attack:
        npcTexture = LoadTexture((texturePath + "/" + name + "_attack.png").c_str());
        Animate(s);
        break;
     case walk:
        npcTexture = LoadTexture((texturePath + "/" + name + "_walk.png").c_str());
        npc_walk.Random_walk(npcPosition); // Update position
        npcPosition = npc_walk.getPosition(); 
        Animate(s);
        break;
     case idle:
        npcTexture = LoadTexture((texturePath + "/" + name + ".png").c_str());
        Animate(s);
        break;
     case hurt:
        npcTexture = LoadTexture((texturePath + "/" + name + "_hurt.png").c_str());  
        Animate(s);    
        break;
     default:
        break;
     }
     
  }
  void Draw()
  {
    if (flip) {
      Rectangle npcFlipped = npcRectangle;
      npcFlipped.x =
          npcRectangle.x + npcRectangle.width; // Set x-coordinate to right edge
      npcFlipped.x -= 20;
      npcFlipped.width =
          -npcRectangle.width; // Invert width to flip horizontally
      DrawTextureRec(npcTexture, npcFlipped, npcPosition, WHITE);
    } else {
      DrawTextureRec(npcTexture, npcRectangle, npcPosition, WHITE);
    }
  }
void Animate(STATE s)
{
    static float timer = 0.0f; 
    static int frame = 0;      
    float frameWidth = 0;
    int count=0;
    int numTextures=0;
    if (npcTexture.width > 0 && npcTexture.height > 0)
    {
        numTextures = npcTexture.width / npcTexture.height;
        if (numTextures > 0)
        {
            frameWidth = static_cast<float>(npcTexture.width / numTextures);
        }
    }
    int maxFrames = static_cast<int>(npcTexture.width / frameWidth);
   if(s==walk){
        timer += GetFrameTime();
    if (timer >= 0.3f)
    {
        timer = 0.0f;
        frame += 1;
    }
    frame = frame % maxFrames;
    DrawTextureRec(
        npcTexture,
        Rectangle{frameWidth * frame, 0, frameWidth, static_cast<float>(npcTexture.height)},
        Vector2{20, 20},
        RAYWHITE
    );

   }
   else{
     timer += GetFrameTime();
    if (timer >= 0.3f  && frame<=numTextures-2)
    {
        timer = 0.0f;
        frame += 1;
        count++;
    }
    DrawTextureRec(
        npcTexture,
        Rectangle{frameWidth * frame, 0, frameWidth, static_cast<float>(npcTexture.height)},
        Vector2{20, 20},
        RAYWHITE
    );

   }
   
}


  

};

/*class NPC{
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
    float frame_width=(float)npcTexture.width/4;
    float timer=0.0f;
    int frame=0;
    int maxframe=(npcTexture.width/frame_width);
    timer+=GetFrameTime();
    if(timer>=0.2f)
    {
        timer=0.0f;
        frame+=1;
    }
    frame=frame%maxframe;
     DrawTextureRec(npcTexture,Rectangle{frame_width*frame,0,frame_width,(float)npcTexture.height},Vector2{20,20},RAYWHITE);
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
    
};*/
class Game{
    vector<NPC> npcs;
   public:
    void Create()
    {
       //Adding values in npc obj;
       
       STATE state=idle;
   // npcs.push_back(NPC("assets/Villagers/3_Man", "Man", {0, 0, 48, 48}, {40, 40}, 2.5,state,-1,17,"Man"));
    
    //till n no of npcs
    }
    void Update()
    {
         for(int i=0;i<npcs.size();i++)
         {
          STATE state=attack;
           npcs[i].setState(state);
         }
    }
    void draw()
    {
        for(int i=0;i<npcs.size();i++)
        {
            STATE state=attack;
            npcs[i].Draw();
            npcs[i].setState(state);
        }
    }
   void ProcessNPCs(vector<NPC> &npcs){
       for(int i=0;i<npcs.size();i++)
       {
      //  npcs[i].Update();
       }
   }
};
int main()
{
    InitWindow(width, height, "My first RAYLIB program!");
    SetTargetFPS(60);
    Game g;
    //g.Create();
    //g.draw();
    STATE state=idle;
    NPC n("assets/Villagers/3_Man", "Man", {0, 0, 48, 48}, {40, 40}, 2.5,state,-1,17,"Man");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
       // g.Update();
       STATE state=walk;
       n.setState(state);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}