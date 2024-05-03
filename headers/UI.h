#ifndef UI_H
#define UI_H
#include <iostream>
#include <raylib.h>
#include <string.h>
#include <string>
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

public:
  vector<string> DropdownNoun1;
  vector<string> DropdownNoun2;
  
  Rectangle rect;
  int capture = 0;
  UI() {
    phoneWindowState = InitGuiPhoneWindow();
    cameraToggleState = InitGuiCameraToggle();
    rect = {0, 0, 160, 120};
    ResetCapture();
  }
  int getPostedState() { return phoneWindowState.posted; };
  void setPostedState(int i) { phoneWindowState.posted = i; };
  int ActiveDropdownNoun2(){return phoneWindowState.DropdownNoun2Active;};
  void Draw() {
    if (capture == 1) {
      DrawRectangleLinesEx(rect, 3, BLACK);
    } else if (capture == 2) {
      DrawRectangleRec(rect, RED);
      // Handle capture !
    }
    GuiCameraToggle(&cameraToggleState);
    phoneWindowState.PhoneBox001Active = cameraToggleState.Toggle000Active;
    GuiPhoneWindow(&phoneWindowState);
    cameraToggleState.Toggle000Active = phoneWindowState.PhoneBox001Active;

    // DrawTexture(screenShotTexture, 0, 0, WHITE);
  }
  void HandleCapture() {
    // Make a Rectangle around the cursor
    Vector2 mousePos = GetMousePosition();
    capture = 0;
    // Check if in bounds of toggle button OR it is toggled on AND in Phone
    // Bound
    if (CheckCollisionPointRec(mousePos, GetGuiCameraToggleBounds()))
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
      return;
    }
    // DrawRectangleLinesEx(rect, 3, BLACK);
    capture = 1;
  }

  void setCaption(vector<string> names) {
    string s = "";
    for (string x : names) {
      s += x;
      if (x != names.back())
        s += ";";
    }
    DropdownNoun1 = names;
    DropdownNoun2 = names;
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
