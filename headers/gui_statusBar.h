/*******************************************************************************************
*
*   StatusBar v1.0.0 - Tool Description
*
*   MODULE USAGE:
*       #define GUI_STATUSBAR_IMPLEMENTATION
*       #include "gui_statusBar.h"
*
*       INIT: GuiStatusBarState state = InitGuiStatusBar();
*       DRAW: GuiStatusBar(&state);
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

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_STATUSBAR_H
#define GUI_STATUSBAR_H

typedef struct {
    Vector2 anchor01;

    char* name;
    
    float ProgressBar001Value;

    Rectangle layoutRecs[2];

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiStatusBarState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
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
GuiStatusBarState InitGuiStatusBar(void);
void GUIStatusBar(GuiStatusBarState *state);

#ifdef __cplusplus
}
#endif

#endif // GUI_STATUSBAR_H

/***********************************************************************************
*
*   GUI_STATUSBAR IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_STATUSBAR_IMPLEMENTATION)

#include "raygui.h"

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
GuiStatusBarState InitGuiStatusBar(void)
{
    GuiStatusBarState state = { 0 };

    state.anchor01 = (Vector2){ 216, 168 };
    
    state.ProgressBar001Value = 0.0f;

    state.layoutRecs[0] = (Rectangle){ state.anchor01.x + -64, state.anchor01.y + -24, 128 - 30, 24 };
    state.layoutRecs[1] = (Rectangle){ state.anchor01.x + -48, state.anchor01.y + -16, 104 - 30, 12 };

    // Custom variables initialization

    return state;
}
void GUIStatusBar(GuiStatusBarState *state)
{
    GuiGroupBox(state->layoutRecs[0], state->name);
    GuiProgressBar(state->layoutRecs[1], NULL, NULL, &state->ProgressBar001Value, 0, 1);
}

#endif // GUI_STATUSBAR_IMPLEMENTATION
