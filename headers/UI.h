#include "effects.h"
#include "sound.h"
#ifndef UI_H
#define UI_H
#include <iomanip>
#include <iostream>
#include <iterator>
#include <raylib.h>
#include <set>
#include <sstream>
#include <string.h>
#include <string>
#include <vector>
using namespace std;
#define RAYGUI_IMPLEMENTATION
#define GUI_CAMERABUTTON_IMPLEMENTATION
#define GUI_PHONEWINDOW_IMPLEMENTATION
#define GUI_CAMERATOGGLE_IMPLEMENTATION
// #define GUI_MENU_IMPLEMENTATION
#include "gui_cameraToggle.h"
#include "gui_phoneWindow.h"
#include "raygui.h"
class StatusBar {
  Rectangle pauseButton;
  Rectangle resumeButton;
  Rectangle quitButton;
  Texture2D pauseIcon;
  bool pauseButtonpressed;
  bool quitButtonpressed;
  bool resumeButtonpressed;
  Color darkRed;
  Color neonGreen;

public:
  StatusBar() {
    neonGreen = {57, 255, 20, 255};
    darkRed = {150, 0, 0, 255};
    pauseButtonpressed = false;
    quitButtonpressed = false;
    resumeButtonpressed = false;
    pauseButton = {755, 0, 30, 30};
    resumeButton = {420, 580, 190, 60};
    quitButton = {200, 580, 190, 60};
    pauseIcon = LoadTexture("../assets/pauseIcon.png");
  }
  void DrawStatusBar(int alive_count, int health_count, float sentiment_total,
                     int total_count) {
    ostringstream oss;
    oss << fixed << setprecision(2) << sentiment_total;
    string formatted_sentiment_count = oss.str();
    DrawRectangle(43, 13, 140, 30, DARKGREEN);
    DrawRectangleLines(43, 13, 140, 30, BLACK);
    DrawText("Alive ", 63, 19, 18, WHITE);
    DrawText(to_string(alive_count).c_str(), 117, 19, 18, WHITE);
    DrawText("/", 126, 19, 18, WHITE);
    DrawText(to_string(total_count).c_str(), 140, 19, 18, WHITE);
    DrawRectangle(300, 13, 140, 30, DARKGREEN);
    DrawRectangleLines(300, 13, 140, 30, BLACK);
    DrawText("Health ", 325, 19, 18, WHITE);
    DrawText(to_string(health_count).c_str(), 396, 19, 18, WHITE);
    DrawRectangle(573, 13, 140, 30, DARKGREEN);
    DrawRectangleLines(573, 13, 140, 30, BLACK);
    DrawText("Sentiment  ", 586, 19, 18, WHITE);
    DrawText(formatted_sentiment_count.c_str(), 673, 19, 18, WHITE);
    DrawRectangleRec(pauseButton, GREEN);
    DrawTextureEx(pauseIcon, (Vector2){(float)755, (float)0}, 0, 0.09, WHITE);
  }
  bool ispressedPauseIcon() {
    if (CheckCollisionPointRec(GetMousePosition(), pauseButton)) {
      pauseButtonpressed = true;
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return true;
      }
    } else {
      pauseButtonpressed = false;
    }
    return false;
  }
  void ShowStatus(int alive_count, int health_count, float sentiment_total,
                  int total_count) {
    ostringstream oss;
    oss << fixed << setprecision(2) << sentiment_total;
    string formatted_sentiment_count = oss.str();
    DrawRectangle(265, 200, 280, 80, DARKGREEN);
    DrawRectangleLines(265, 200, 280, 80, BLACK);
    DrawText("Alive ", 310, 220, 38, WHITE);
    DrawText(to_string(alive_count).c_str(), 430, 220, 38, GREEN);
    DrawText("/", 460, 220, 38, GREEN);
    DrawText(to_string(total_count).c_str(), 485, 220, 38, GREEN);
    DrawRectangle(265, 330, 280, 80, DARKGREEN);
    DrawRectangleLines(265, 330, 280, 80, BLACK);
    DrawText("Health ", 300, 350, 38, WHITE);
    DrawText(to_string(health_count).c_str(), 450, 350, 38, GREEN);
    DrawRectangle(265, 460, 280, 80, DARKGREEN);
    DrawRectangleLines(265, 460, 280, 80, BLACK);
    DrawText("Sentiment  ", 270, 480, 38, WHITE);
    DrawText(formatted_sentiment_count.c_str(), 460, 480, 38, GREEN);
    DrawRectangleRec(quitButton, quitButtonpressed ? darkRed : RED);
    DrawRectangleLinesEx(quitButton, 3, WHITE);
    DrawText("EXIT", 240, 590, 40, WHITE);
    DrawRectangleRec(resumeButton, resumeButtonpressed ? neonGreen : GREEN);
    DrawRectangleLinesEx(resumeButton, 3, WHITE);
    DrawText("RESUME", 430, 590, 40, WHITE);
  }
  bool isExitPressed() {
    if (CheckCollisionPointRec(GetMousePosition(), quitButton)) {
      quitButtonpressed = true;
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return true;
      }
    } else {
      quitButtonpressed = false;
    }
    return false;
  }

  bool isResumePressed() {
    if (CheckCollisionPointRec(GetMousePosition(), resumeButton)) {
      resumeButtonpressed = true;
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return true;
      }
    } else {
      resumeButtonpressed = false;
    }
    return false;
  }
};
class StartingMenu {
  Rectangle border;
  Rectangle buttonrec1;
  Rectangle buttonrec2;
  Rectangle buttonrec3;
  Color textColor;
  Texture2D texture;
  bool button1pressed;
  bool button2pressed;
  bool button3pressed;

public:
  StartingMenu() {
    textColor = GREEN;
    border = {20, 20, 760, 760};
    buttonrec1 = {330, 470, 150, 30};
    buttonrec2 = {330, 530, 150, 30};
    buttonrec3 = {330, 590, 150, 30};
    button1pressed = false;
    button2pressed = false;
    button3pressed = false;
  }
  void DrawStartingMenu() {
    DrawRectangleLinesEx(border, 7, DARKGREEN);
    DrawRectangleRec(buttonrec1, button1pressed ? DARKBLUE : DARKGREEN);
    DrawRectangleLinesEx(buttonrec1, 2, GREEN);
    DrawText("PLAY GAME", 350, 475, 20, textColor);
    DrawRectangleRec(buttonrec2, button2pressed ? DARKBLUE : DARKGREEN);
    DrawRectangleLinesEx(buttonrec2, 2, GREEN);
    DrawText("OPTIONS", 360, 535, 20, textColor);
    DrawRectangleRec(buttonrec3, button3pressed ? DARKBLUE : DARKGREEN);
    DrawRectangleLinesEx(buttonrec3, 2, GREEN);
    DrawText("QUIT", 380, 595, 20, textColor);
  }
  void undoPressed() { button1pressed = false; }
  bool isPressed1() {
    if (CheckCollisionPointRec(GetMousePosition(), buttonrec1)) {
      button1pressed = true;
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return true;
      }
    } else {
      button1pressed = false;
    }
    return false;
  }
    void DrawAudioBox(){
    Color lightBlue={55, 136, 255, 255};
    Color darkRed={139, 0, 0, 255};
    //DrawRectangle(240,270,350,170,SKYBLUE);
   // DrawRectangleLines(240,270,350,170,WHITE);
   DrawRectangleRec(OKButton,OKButtonpressed?DARKBLUE:SKYBLUE);
    DrawRectangleLines(352,410,90,37,WHITE);
   // DrawText("Audio Settings",250,272,20,WHITE);
   // DrawRectangleRec(crossButton,crossButtonpressed?darkRed:RED)
    DrawTextEx(music,"OK",{(float)372,(float)410},40,0,WHITE);
  }

  bool isPressed2() {
    if (CheckCollisionPointRec(GetMousePosition(), buttonrec2)) {
      button2pressed = true;
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return true;
      }
    } else {
      button2pressed = false;
    }
    return false;
  }
  bool isPressed3() {
    if (CheckCollisionPointRec(GetMousePosition(), buttonrec3)) {
      button3pressed = true;
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return true;
      }
    } else {
      button3pressed = false;
    }
    return false;
  }
  void transition() {
    isPressed1();
    isPressed2();
    isPressed3();
  }
};
class UI : public StatusBar, public StartingMenu {
  // Constructor makes all the necessary UI elements
  GuiPhoneWindowState phoneWindowState;
  GuiCameraToggleState cameraToggleState;
  Image screenShot;
  Texture2D screenShotTexture;
  Texture2D captureTexture;
  Rectangle captureRect;
  int currentFrame;
  int FrameCount;
  int FrameSpeed;
  Vector2 mousePos;
  int maxFrame;
  Camera2D *camera;
  Rectangle crossButton;
  bool crossButtonpressed;

public:
  vector<string> DropdownNoun1;
  vector<string> DropdownNoun2;
  const vector<string> DropdownConnector = {"attack", "slap", "galnotch",
                                            "hugs", "treat"};
  const set<string> neg = {"attack", "slap", "galnotch"};
  const set<string> pos = {"hugs", "treat"};

  Rectangle rect;
  int capture = 0;
  UI() {
    crossButton = {560, 270, 30, 30};
    crossButtonpressed = false;
    phoneWindowState = InitGuiPhoneWindow();
    cameraToggleState = InitGuiCameraToggle();
    rect = {0, 0, 200, 120};
    currentFrame = 0;
    FrameCount = 0;
    FrameSpeed = 5;
    captureRect = {0, 0, 200, 120};
    captureTexture =
        LoadTexture("../assets/effects/Spritesheet_UI_Flat_Select_01.png");
    maxFrame = 4;
    ResetCapture();
  }
  void setCamera(Camera2D *c) { camera = c; }
  int getPostedState() { return phoneWindowState.posted; };
  void setPostedState(int i) { phoneWindowState.posted = i; };
  int ActiveDropdownNoun2() { return phoneWindowState.DropdownNoun2Active; };
  int ActiveDropdownConnector() {
    return phoneWindowState.DropdownConnectorActive;
  }
  void Animate() {
    if (FrameCount >= (60 / FrameSpeed)) {
      currentFrame = (currentFrame + 1) % maxFrame;
      captureRect.x = currentFrame * 200;
      FrameCount = 0;
    }
    FrameCount++;
  }
  void Draw(int alive_count, int health_count, float sentiment_total,
            int total_count) {
    if (capture == 1) {
      DrawTexturePro(captureTexture, captureRect, rect, {0, 0}, 0, WHITE);
    } else if (capture == 2) {
      Vector2 *pos = new Vector2{mousePos.x, mousePos.y};
    }
    Animate();
    GuiCameraToggle(&cameraToggleState);
    phoneWindowState.PhoneBox001Active = cameraToggleState.Toggle000Active;
    // phoneWindowState.anchor02 = GetScreenToWorld2D(phoneWindowState.anchor02
    // , *camera); camera->target = save;
    GuiPhoneWindow(&phoneWindowState);
    cameraToggleState.Toggle000Active = phoneWindowState.PhoneBox001Active;
    DrawStatusBar(alive_count, health_count, sentiment_total, total_count);
    // DrawTexture(screenShotTexture, 0, 0, WHITE);
  }
  void HandleCapture() {
    // Make a Rectangle around the cursor
    // Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), *camera);
    mousePos = GetMousePosition();
    // cout << mousePos.x << " " << mousePos.y << endl;
    capture = 0;
    // Check if in bounds of toggle button OR it is toggled on AND in Phone
    // Bound
    if (CheckCollisionPointRec(mousePos,
                               GetGuiCameraToggleBounds(&cameraToggleState)))
      return;
    if (cameraToggleState.Toggle000Active &&
        CheckCollisionPointRec(mousePos, GuiGetPhoneBox001()))
      return;
    float rectHeight = 120;
    float rectWidth = 160;
    rect = {mousePos.x - rectWidth / 2, mousePos.y - rectHeight / 2, rectWidth,
            rectHeight};

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      // Update the Texture2D
      screenShot = LoadImageFromScreen();
      if (IsImageReady(screenShot)) {
        if (IsTextureReady(screenShotTexture))
          UnloadTexture(screenShotTexture);
        UnloadTexture(phoneWindowState.Image001);
        ImageCrop(&screenShot, rect);
        ImageResize(&screenShot, 250, 180);
        screenShotTexture = LoadTextureFromImage(screenShot);
        phoneWindowState.Image001 = screenShotTexture;
        UnloadImage(screenShot);
      }
      // DrawRectangleRec(rect, RED);
      capture = 2;
      EffectManager::addEffectUI(Capture, &mousePos, 10, true);
      PlaySound(SoundMap::soundMap[Confirm]);
      return;
    }
    // DrawRectangleLinesEx(rect, 3, BLACK);
    capture = 1;
  }

  void setCaption(unordered_set<string> names) {
    string s = "";
    int size = names.size();
    int i = 0;
    for (string x : names) {
      DropdownNoun1.push_back(x);
      DropdownNoun2.push_back(x);
      s += x;
      i++;
      if (i != size)
        s += ";";
    }
    phoneWindowState.DropdownNoun1 = new char[s.size() + 1];
    phoneWindowState.DropdownNoun2 = new char[s.size() + 1];
    strcpy(phoneWindowState.DropdownNoun1, s.c_str());
    strcpy(phoneWindowState.DropdownNoun2, s.c_str());
  }

private:
  void ResetCapture() {
    if (IsTextureReady(screenShotTexture))
      UnloadTexture(screenShotTexture);
    screenShot = LoadImage("../assets/DefaultCapture.png");
    ImageResize(&screenShot, 250, 180);
    screenShotTexture = LoadTextureFromImage(screenShot);
    phoneWindowState.Image001 = screenShotTexture;
    UnloadImage(screenShot);
  }
};

#endif
