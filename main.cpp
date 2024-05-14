#include "headers/UI.h"
#include "headers/collisions.h"
#include "headers/ds.h"
#include "headers/effects.h"
#define RAYGUI_IMPLEMENTATION
#include "headers/raygui.h"
#include "headers/sound.h"
#define GUI_VOLUME_BAR_IMPLEMENTATION
#define GUI_STATUSBAR_IMPLEMENTATION
#include "headers/gui_statusBar.h"
#include "headers/gui_volume_bar.h"
#include "math.h"
// Semtiment -ve -> MORE
// Age Game SMALLE -> MORE
// OCC 1 -> MORE
#include <cstring>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
// #define RAYLIB_TILESON_IMPLEMENTATION
// #include "raylib-tileson.h"

#include <chrono>
#include <random>
#include <unistd.h> // For getpid()
#include <vector>

using namespace std;
const int width = 800;
const int height = 800;
enum STATE { attack, walk, idle, hurt, death };
bool lastplayer = 0;

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
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
      camera.offset = GetMousePosition();
      camera.target = mouseWorldPos;
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
  }

  // Control that Camera Object the map
  // Right click drag changes the Camera
};

class NPC_Characteristics {
  STATE state;
  float sentiment;
  int age;
  string occupation;
  string name;
  int health;

public:
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
  map<STATE, Texture2D> textures;

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
    textures[walk] = LoadTexture((texturePath + "_walk.png").c_str());
    textures[idle] = LoadTexture((texturePath + "_idle.png").c_str());
    textures[attack] = LoadTexture((texturePath + "_attack.png").c_str());
    textures[hurt] = LoadTexture((texturePath + "_hurt.png").c_str());
    textures[death] = LoadTexture((texturePath + "_death.png").c_str());
    npcTexture = textures[state];
  }

  void controlbyPlayer() {
    Vector2 mousepos = GetMousePosition();
    Rectangle NPC_Rect = Rectangle{npcPosition.x, npcPosition.y, 48, 48};
    if (CheckCollisionPointRec(mousepos, NPC_Rect)) {
      if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        return;
      npcPosition = mousepos;
      npcPosition.x -= 24;
      npcPosition.y -= 24;
      if (CheckCollisionRecs(NPC_Rect, Rectangle{320, 320, 160, 160})) {
        health = 0;
        setState(death);
        EffectManager::addEffect(Blood, GetPosPointer(), 10);
      }
    }
  }
  void setState(STATE s) {
    state = s;
    npcTexture = textures[state];
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
    if (isMoving && !lastplayer)
      Random_walk();
    else if (lastplayer)
      controlbyPlayer();
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
  Vector2 *GetPosPointer() { return &npcPosition; }
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
  float calculateSentiment() {
    float sentiment_total = 0;
    for (int i = 0; i < npcList.size(); i++) {
      sentiment_total += npcList[i]->getSentiment();
    }
    return sentiment_total;
  }
  int calculateHealth() {
    int health_total = 0;
    for (int i = 0; i < npcList.size(); i++) {
      health_total += npcList[i]->getHealth();
    }
    return health_total;
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
    int g = abs(npc1->getAge() - npc2->getAge());
    int o;
    if (npc1->getOccupation() != npc2->getOccupation()) {
      o = 1;
    } else {
      o = 0;
    }

    float y = calculateProbability(npc1->getSentiment(), npc2->getSentiment(),
                                   (g + 3), o) /
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
        cout << "-----------------" << npc->getName() << endl;
        npc->setSentiment(npc->getSentiment() + val);
        EffectManager::addEffect(Crystal, npc->GetPosPointer(), 10);
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
  int getListsize() { return npcList.size(); }
};

class Game : public NPC_Interactions, public UI {
  static int death_count;

public:
  Game() {}
  static int getDeath() { return death_count; };
  void DrawUI() {
    UI::Draw(getListsize() - death_count, calculateHealth(),
             calculateSentiment(), getListsize());
  }
  void DrawNPC() { NPC_Interactions::Draw(); }
  void Draw() {
    DrawNPC();
    DrawUI();
  }
  void Random_interactions() {
    vector<pair<NPC *, NPC *>> pairList = CheckCollisions();
    // cout << pairList.size() << endl;
    for (auto o : pairList) {
      bool stuff = ActionProbablity(o.first, o.second);
      if (stuff && o.first->getState() != death &&
          o.first->getState() != attack && o.second->getState() != death &&
          o.second->getState() != attack) {
        if ((o.first->getSentiment() >= -1 && o.first->getSentiment() <= 1) ||
            (o.second->getSentiment() >= -1 && o.second->getSentiment() <= 1)) {
          if (o.first->getSentiment() > o.second->getSentiment() ||
              o.first->getSentiment() == o.second->getSentiment()) {
            EffectManager::addEffect(Slash, o.second->GetPosPointer(), 10);
            PlaySound(SoundMap::soundMap[Attack]);
            o.second->setState(attack);
            EffectManager::addEffect(Blood, o.first->GetPosPointer(), 30);
            PlaySound(SoundMap::soundMap[Hit]);
            o.first->setState(hurt);
            if (o.first->getSentiment() > 0)
              o.first->setHealth(35);
            else if (o.first->getSentiment() < 0)
              o.first->setHealth(35);
            else
              o.first->setHealth(30);
          } else if (o.first->getSentiment() < o.second->getSentiment()) {
            EffectManager::addEffect(Slash, o.first->GetPosPointer(), 10);
            PlaySound(SoundMap::soundMap[Attack]);
            o.first->setState(attack);
            EffectManager::addEffect(Blood, o.second->GetPosPointer(), 10);
            PlaySound(SoundMap::soundMap[Hit]);
            o.second->setState(hurt);
            if (o.second->getSentiment() > 0)
              o.second->setHealth(35);
            else if (o.second->getSentiment() < 0)
              o.second->setHealth(25);
            else
              o.second->setHealth(20);
          }
        }
        if (o.first->getHealth() == 0) {
          o.first->setState(death);
          death_count++;
        } else if (o.second->getHealth() == 0) {
          o.second->setState(death);
          death_count++;
        }
      }
    }
  }
  void Update() {
    if (death_count == getListsize() - 1) {
      lastplayer = 1;
    }
    if (UI::getPostedState()) {
      // Grab the second Noun
      // Grab the connect
      string target = DropdownNoun2[ActiveDropdownNoun2()];
      string adj = DropdownConnector[ActiveDropdownConnector()];
      if (neg.count(adj) > 0) {
        ChangeSentimentVal(target, -1);
      } else {
        ChangeSentimentVal(target, 1);
      }
      UI::setPostedState(0);
    }
    if (capture == 2) {
      Rectangle captured = rect;
      vector<Point<NPC>> collided = QueryRec(captured);
      unordered_set<string> names;
      for (Point<NPC> npc : collided) {
        if (npc.data->getState() == attack || npc.data->getState() == hurt) {
          names.insert(npc.data->getName());
          names.insert(npc.data->getOccupation());
        }
      }
      unordered_set<string> defaultCaption = {"Take", "Caption"};
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

int Game::death_count = 0;
list<Effect> EffectManager::effectList;
list<Effect> EffectManager::effectListUI;
map<EffectType, Texture2D> EffectMap::effectMap;
map<SoundType, Sound> SoundMap::soundMap;

enum GameState { mainScreen, GameScreen, optionsMenu, quitGame, PauseWindow };
int main() {
  InitWindow(width, height, "ChaosCraze");
  InitAudioDevice();
  // GuiLoadStyle("../assets/entefe.rgs");
  EffectMap::Init();
  SoundMap::Init();
  SetTargetFPS(60);
  // Seed for random number

  srand(time(NULL));
  //  NPC(string _name, Vector2 pos, float spd, STATE state, int sentiment, int
  //  age, string occupation)
  // NPC_Interactions Game;
  Game Game;
  CameraController c;
  Texture2D map;
  map = LoadTexture("../assets/Map0751.png");
  CollisionMapper::LoadCollisionMap();
  Texture2D logo;
  logo = LoadTexture("../assets/Thing.png");
  Texture2D main;
  main = LoadTexture("../assets/mainscreen.png");
  Texture2D optionwindow = LoadTexture("../assets/MEN_UUUUUU.png");
  Texture2D optionBG = LoadTexture("../assets/untitled.png");
  NPC Boy("Boy", {100, 200}, 2, STATE::walk, -0.8, 0, "Villagers");
  NPC Girl("Girl", {100, 200}, 2, STATE::walk, -0.5, 0, "Villagers");
  NPC Old_Man("Old_man", {100, 200}, 2, STATE::walk, -0.8, 0, "Villagers");
  NPC Man("Man", {100, 200}, 2, STATE::walk, -0.5, 0, "Villagers");
  NPC Woman("Woman", {100, 200}, 2, STATE::walk, -0.5, 0, "Villagers");
  NPC Old_Woman("Old_woman", {100, 200}, 2, STATE::walk, -0.5, 0, "Villagers");
  NPC Bob("Bob", {100, 200}, 2, STATE::walk, -0.5, 0, "Greenzone");
  NPC Huffy("Huffy", {100, 200}, 2, STATE::walk, -0.5, 0, "Greenzone");
  NPC Snuffy("Snuffy", {100, 200}, 2, STATE::walk, -0.5, 0, "Greenzone");
  NPC GraveRobber("GraveRobber", {100, 200}, 2, STATE::walk, -0.5, 0,
                  "MainCharacters");
  NPC SteamMan("SteamMan", {100, 200}, 2, STATE::walk, -0.5, 0,
               "MainCharacters");
  NPC Woodcutter("Woodcutter", {100, 200}, 2, STATE::walk, -0.5, 0,
                 "MainCharacters");
  try {
    Game.AddNPC(&Bob);
    Game.AddNPC(&Huffy);
    Game.AddNPC(&Snuffy);
    Game.AddNPC(&GraveRobber);
    Game.AddNPC(&SteamMan);
    Game.AddNPC(&Woodcutter);
    Game.AddNPC(&Old_Woman);
    Game.AddNPC(&Boy);
    Game.AddNPC(&Girl);
    Game.AddNPC(&Old_Man);
    Game.AddNPC(&Man);
    Game.AddNPC(&Woman);

  } catch (bad_alloc &e) {
    cout << "Bad Allocation" << endl;
  }
  GameState currentState = mainScreen;
  Music music = LoadMusicStream("../assets/sound/CLassic.mp3");
  SetMusicVolume(music, 0.7);
  PlayMusicStream(music);
  GuiVolumeBarState volumebarstate = InitGuiVolumeBar();
  // int stateC = 4;
  // STATE state_list[] = {idle, walk, attack, hurt, death};
  while (!WindowShouldClose()) {
    c.Control();
    UpdateMusicStream(music);
    Game.HandleCapture();
    ClearBackground(BLACK);
    switch (currentState) {
    case mainScreen:
      DrawTexture(main, 0, 0, WHITE);
      Game.DrawStartingMenu();
      DrawTextureEx(logo, {145, 10}, 0.0f, 1, WHITE);
      Game.transition();
      if (Game.isPressed1()) {
        currentState = GameScreen;
      }
      if (Game.isPressed2()) {
        currentState = optionsMenu;
      }
      if (Game.isPressed3()) {
        currentState = quitGame;
      }
      break;
    case optionsMenu:
      ClearBackground(DARKGREEN);
      DrawTexture(optionwindow, 0, 0, WHITE);
      DrawTexture(optionBG, 226, 260, WHITE);
      Game.DrawAudioBox();
      GuiVolumeBar(&volumebarstate);
      if (Game.isOkButtonPressed()) {
        currentState = mainScreen;
      }
      break;
    case GameScreen:
      c.BeginCamera();
      DrawTexture(map, 0, 0, WHITE);
      Game.DrawNPC();
      // TODO: updateEffectUI and World
      EffectManager::updateEffects();
      // CollisionMapper::DrawCollisionMap();
      c.EndCamera();
      Game.DrawUI();
      EffectManager::updateEffectsUI();
      if (Game.ispressedPauseIcon()) {
        currentState = PauseWindow;
      }
      Game.Update();
      break;
    case PauseWindow:
      DrawTexture(main, 0, 0, WHITE);
      Game.ShowStatus(Game.getListsize() - Game.getDeath(),
                      Game.calculateHealth(), Game.calculateSentiment(),
                      Game.getListsize());
      if (Game.isResumePressed()) {
        currentState = GameScreen;
      }
      if (Game.isExitPressed()) {
        currentState = mainScreen;
      }
      break;
    case quitGame:
      CloseWindow();
      break;
    }
    EndDrawing();
    // DrawTiled(map, 0, 0, WHITE);
    // CollisionMapper::DrawCollisionMap();
  }
  UnloadTexture(logo);
  UnloadTexture(map);
  // UnloadMap(map);
  CloseWindow();
  return 0;
}
