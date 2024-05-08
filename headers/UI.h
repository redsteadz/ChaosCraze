#ifndef UI_H
#define UI_H
#include <iostream>
#include <iomanip>
#include <sstream>
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
class StatusBar{
  public:
  void DrawStausBar(int alive_count,int health_count,float sentiment_count,int total_count){
    ostringstream oss;
    oss <<fixed << setprecision(2) << sentiment_count;
    string formatted_sentiment_count = oss.str();
     DrawRectangle(46,13,135,25,SKYBLUE);
     DrawRectangleLines(46,13,135,25,SKYBLUE);
     DrawText("Alive ",63,16,18,WHITE);
     DrawText(to_string(alive_count).c_str(),117,16,18,WHITE);
     DrawText("/",126,16,18,WHITE);
     DrawText(to_string(total_count).c_str(),140,16,18,WHITE);
     DrawRectangle(327,13,135,25,SKYBLUE);
     DrawRectangleLines(327,13,135,25,SKYBLUE);
     DrawText("Health ",349,16,18,WHITE);
     DrawText(to_string(health_count).c_str(),420,16,18,WHITE);
     DrawRectangle(600,13,135,25,SKYBLUE);
     DrawRectangleLines(600,13,135,25,SKYBLUE);
     DrawText("Sentiment  ",610,16,18,WHITE);
     DrawText(formatted_sentiment_count.c_str(),697,16,18,WHITE);
  }


};
class UI :public StatusBar{
  // Constructor makes all the necessary UI elements
  GuiPhoneWindowState phoneWindowState;
  GuiCameraToggleState cameraToggleState;
  Image screenShot;
  Texture2D screenShotTexture;

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
    rect = {0, 0, 160, 120};
    ResetCapture();
  }
  int getPostedState() { return phoneWindowState.posted; };
  void setPostedState(int i) { phoneWindowState.posted = i; };
  int ActiveDropdownNoun2() { return phoneWindowState.DropdownNoun2Active; };
  int ActiveDropdownConnector() {
    return phoneWindowState.DropdownConnectorActive;
  }
  void Draw(int alive_count,int health_count,float sentiment_total,int total_count) {
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
    DrawStausBar(alive_count,health_count,sentiment_total,total_count);
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
