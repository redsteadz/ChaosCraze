#ifndef UI_H
#define UI_H
#include <iostream>
#include <raylib.h>
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
  UI() {
    phoneWindowState = InitGuiPhoneWindow();
    cameraToggleState = InitGuiCameraToggle();
    ResetCapture();
  }
  void Draw() {
    GuiCameraToggle(&cameraToggleState);
    phoneWindowState.PhoneBox001Active = cameraToggleState.Toggle000Active;
    GuiPhoneWindow(&phoneWindowState);
    cameraToggleState.Toggle000Active = phoneWindowState.PhoneBox001Active;
    // DrawTexture(screenShotTexture, 0, 0, WHITE);
  }
  void HandleCapture() {
    // Make a Rectangle around the cursor
    Vector2 mousePos = GetMousePosition();
    // Check if in bounds of toggle button OR it is toggled on AND in Phone
    // Bound
    if (CheckCollisionPointRec(mousePos, GetGuiCameraToggleBounds()))
      return;
    if (cameraToggleState.Toggle000Active &&
        CheckCollisionPointRec(mousePos, GuiGetPhoneBox001()))
      return;
    float rectHeight = 120;
    float rectWidth = 160;
    Rectangle rect = {mousePos.x - rectWidth / 2, mousePos.y - rectHeight / 2,
                      rectWidth, rectHeight};

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
      DrawRectangleRec(rect, RED);
    } else {
      DrawRectangleLinesEx(rect, 3, BLACK);
    }
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
