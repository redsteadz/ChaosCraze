/*******************************************************************************************
 *
 *   PhoneWindow v1.0.0 - Tool Description
 *
 *   MODULE USAGE:
 *       #define GUI_PHONEWINDOW_IMPLEMENTATION
 *       #include "gui_phoneWindow.h"
 *
 *       INIT: GuiPhoneWindowState state = InitGuiPhoneWindow();
 *       DRAW: GuiPhoneWindow(&state);
 *
 *   LICENSE: Propietary License
 *
 *   Copyright (c) 2022 raylib technologies. All Rights Reserved.
 *
 *   Unauthorized copying of this file, via any medium is strictly prohibited
 *   This project is proprietary and confidential unless the owner allows
 *   usage in any other form by expresely written permission.
 *
 **********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including
// this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>

#ifndef GUI_PHONEWINDOW_H
#define GUI_PHONEWINDOW_H

typedef struct {
  Vector2 anchor02;

  bool PhoneBox001Active;
  bool DropdownNoun1Edit;
  int DropdownNoun1Active;
  bool DropdownConnectorEdit;
  int DropdownConnectorActive;
  bool DropdownNoun2Edit;
  int DropdownNoun2Active;

  char* DropdownNoun1;
  char* DropdownConnector;
  char* DropdownNoun2;


  int posted;
  // Custom state variables (depend on development software)
  Texture2D Image001;
  // NOTE: This variables should be added manually if required

} GuiPhoneWindowState;

#ifdef __cplusplus
extern "C" { // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiPhoneWindowState InitGuiPhoneWindow(void);
void GuiPhoneWindow(GuiPhoneWindowState *state);
Rectangle GuiGetPhoneBox001(void);
void setImage001(Texture2D texture, GuiPhoneWindowState &state);

static void Button005();

#ifdef __cplusplus
}
#endif

#endif // GUI_PHONEWINDOW_H

/***********************************************************************************
 *
 *   GUI_PHONEWINDOW IMPLEMENTATION
 *
 ************************************************************************************/

#if defined(GUI_PHONEWINDOW_IMPLEMENTATION)

#include "raygui.h"
#include <iostream>
//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiPhoneWindowState InitGuiPhoneWindow(void) {
  GuiPhoneWindowState state = {0};
  state.anchor02 = (Vector2){24, 300 - 24};

  state.PhoneBox001Active = false;
  state.DropdownNoun1Edit = false;
  state.DropdownNoun1Active = 0;
  state.DropdownConnectorEdit = false;
  state.DropdownConnectorActive = 0;
  state.DropdownNoun2Edit = false;
  state.DropdownNoun2Active = 0;

  state.DropdownNoun1 = "-;-;-";
  state.DropdownConnector = "attack;slap;galnotch;hugs;treat";
  // Negative Connector Snetiment dec 
  // Positive Connector Sentiment inc
  state.DropdownNoun2 = "-;-;-";

  state.posted = 0;
  // state.Image001 = LoadTexture("../assets/DefaultCapture.png");
  // Custom variables initialization

  return state;
}

Rectangle GuiGetPhoneBox001(void) {
  return (Rectangle){24 + 0, 300 - 24 + -216, 304, 424};
}

static void Button005(int A, int B, int C, GuiPhoneWindowState &state) {
  // TODO: Implement control logic
  state.posted = 1;
  if (IsTextureReady(state.Image001))
    UnloadTexture(state.Image001);
  Image screenShot = LoadImage("../assets/DefaultCapture.png");
  ImageResize(&screenShot, 250, 180);
  state.Image001 = LoadTextureFromImage(screenShot);
  UnloadImage(screenShot);
}

void GuiPhoneWindow(GuiPhoneWindowState *state) {
  if (state->DropdownNoun1Edit || state->DropdownConnectorEdit ||
      state->DropdownNoun2Edit)
    GuiLock();

  if (state->PhoneBox001Active) {

    state->PhoneBox001Active = !GuiWindowBox(
        (Rectangle){state->anchor02.x + 0, state->anchor02.y + -216, 304, 424},
        "Phone");
    GuiGroupBox(
        (Rectangle){state->anchor02.x + 24, state->anchor02.y + -160, 256, 200},
        "IMAGE");
    if (IsTextureReady(state->Image001)) {
      DrawTextureV(state->Image001,
                   Vector2{state->anchor02.x + 26, state->anchor02.y + -156},
                   WHITE);
    }
    if (GuiDropdownBox(
            (Rectangle){state->anchor02.x + 32, state->anchor02.y + 56, 80, 24},
            state->DropdownNoun1, &state->DropdownNoun1Active,
            state->DropdownNoun1Edit))
      state->DropdownNoun1Edit = !state->DropdownNoun1Edit;
    if (GuiDropdownBox((Rectangle){state->anchor02.x + 112,
                                   state->anchor02.y + 56, 80, 24},
                       state->DropdownConnector, &state->DropdownConnectorActive,
                       state->DropdownConnectorEdit))
      state->DropdownConnectorEdit = !state->DropdownConnectorEdit;
    if (GuiDropdownBox((Rectangle){state->anchor02.x + 192,
                                   state->anchor02.y + 56, 80, 24},
                       state->DropdownNoun2, &state->DropdownNoun2Active,
                       state->DropdownNoun2Edit))
      state->DropdownNoun2Edit = !state->DropdownNoun2Edit;
    if (GuiButton((Rectangle){state->anchor02.x + 192, state->anchor02.y + 160,
                              88, 32},
                  "POST"))
      Button005(state->DropdownNoun1Active, state->DropdownConnectorActive,
                state->DropdownNoun2Active, *state);
  }

  GuiUnlock();
}

#endif // GUI_PHONEWINDOW_IMPLEMENTATION
