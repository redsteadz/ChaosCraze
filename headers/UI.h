#include "effects.h"
#include "sound.h"
#ifndef UI_H
#define UI_H
#include <iostream>
#include <iterator>
#include <raylib.h>
#include <string.h>
#include <string>
#include <set>
#include <vector>
using namespace std;
#define RAYGUI_IMPLEMENTATION
#define GUI_CAMERABUTTON_IMPLEMENTATION
#define GUI_PHONEWINDOW_IMPLEMENTATION
#define GUI_CAMERATOGGLE_IMPLEMENTATION
#include "raygui.h"
#include "gui_cameraToggle.h"
#include "gui_phoneWindow.h"

class UI {
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
    phoneWindowState = InitGuiPhoneWindow();
    cameraToggleState = InitGuiCameraToggle();
    rect = {0, 0, 200, 120};
    currentFrame = 0;
    FrameCount = 0;
    FrameSpeed = 5;
    captureRect = {0, 0, 200, 120};
    captureTexture = LoadTexture("../assets/effects/Spritesheet_UI_Flat_Select_01.png");
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
  void Animate(){
    if (FrameCount >= (60 / FrameSpeed)) {
      currentFrame = (currentFrame + 1) % maxFrame;
      captureRect.x = currentFrame * 200;
      FrameCount = 0;
    }
    FrameCount++;
  }
  void Draw() {
    if (capture == 1) {
        DrawTexturePro(captureTexture,  captureRect, rect , {0,0}, 0, WHITE);    
    } else if (capture == 2) {
      Vector2 *pos = new Vector2{mousePos.x, mousePos.y};
    }
    Animate();
    GuiCameraToggle(&cameraToggleState);
    phoneWindowState.PhoneBox001Active = cameraToggleState.Toggle000Active;
    // phoneWindowState.anchor02 = GetScreenToWorld2D(phoneWindowState.anchor02 , *camera);
    // camera->target = save;
    GuiPhoneWindow(&phoneWindowState);
    cameraToggleState.Toggle000Active = phoneWindowState.PhoneBox001Active;

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
    if (CheckCollisionPointRec(mousePos, GetGuiCameraToggleBounds(&cameraToggleState)))
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
      EffectManager::addEffectUI(Capture , &mousePos, 10, true);
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
