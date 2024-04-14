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

#include <string.h> // Required for: strcpy()

#ifndef GUI_PHONEWINDOW_H
#define GUI_PHONEWINDOW_H

typedef struct {
  Vector2 anchor02;

  bool PhoneBox001Active;
  bool DropdownBox005EditMode;
  int DropdownBox005Active;
  bool DropdownBox003EditMode;
  int DropdownBox003Active;
  bool DropdownBox004EditMode;
  int DropdownBox004Active;

  // Custom state variables (depend on development software)
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

  state.anchor02 = (Vector2){60, 328};

  state.PhoneBox001Active = true;
  state.DropdownBox005EditMode = false;
  state.DropdownBox005Active = 0;
  state.DropdownBox003EditMode = false;
  state.DropdownBox003Active = 0;
  state.DropdownBox004EditMode = false;
  state.DropdownBox004Active = 0;

  // Custom variables initialization

  return state;
}
static void Button005(int A, int B, int C) {
  // TODO: Implement control logic
  const char *text[] = {"Huffy", "Snuffy", "Abeera"};
  const char *text2[] = {"And", "Or", "Slaps"};
  std::cout << text[A] << " " << text2[B] << " " << text[C] << std::endl;
}

void GuiPhoneWindow(GuiPhoneWindowState *state) {
  if (state->DropdownBox005EditMode || state->DropdownBox003EditMode ||
      state->DropdownBox004EditMode)
    GuiLock();

  if (state->PhoneBox001Active) {
    state->PhoneBox001Active = !GuiWindowBox(
        (Rectangle){state->anchor02.x + 0, state->anchor02.y + -216, 304, 424},
        "Phone");
    GuiGroupBox(
        (Rectangle){state->anchor02.x + 24, state->anchor02.y + -160, 256, 200},
        "IMAGE");
    if (GuiDropdownBox(
            (Rectangle){state->anchor02.x + 32, state->anchor02.y + 56, 80, 24},
            "Huffy;Snuffy;Abeera", &state->DropdownBox005Active,
            state->DropdownBox005EditMode))
      state->DropdownBox005EditMode = !state->DropdownBox005EditMode;
    if (GuiDropdownBox((Rectangle){state->anchor02.x + 112,
                                   state->anchor02.y + 56, 80, 24},
                       "And;Or;Slaps", &state->DropdownBox003Active,
                       state->DropdownBox003EditMode))
      state->DropdownBox003EditMode = !state->DropdownBox003EditMode;
    if (GuiDropdownBox((Rectangle){state->anchor02.x + 192,
                                   state->anchor02.y + 56, 80, 24},
                       "Huffy;Snuffy;Abeera", &state->DropdownBox004Active,
                       state->DropdownBox004EditMode))
      state->DropdownBox004EditMode = !state->DropdownBox004EditMode;
    if (GuiButton((Rectangle){state->anchor02.x + 192, state->anchor02.y + 160,
                              88, 32},
                  "POST"))
      Button005( state->DropdownBox005Active,state->DropdownBox003Active, state->DropdownBox004Active);
  }

  GuiUnlock();
}

#endif // GUI_PHONEWINDOW_IMPLEMENTATION
