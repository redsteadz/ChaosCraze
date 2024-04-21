/*******************************************************************************************
*
*   CameraToggle v1.0.0 - Tool Description
*
*   MODULE USAGE:
*       #define GUI_CAMERATOGGLE_IMPLEMENTATION
*       #include "gui_cameraToggle.h"
*
*       INIT: GuiCameraToggleState state = InitGuiCameraToggle();
*       DRAW: GuiCameraToggle(&state);
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

#ifndef GUI_CAMERATOGGLE_H
#define GUI_CAMERATOGGLE_H

typedef struct {
    bool Toggle000Active;

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiCameraToggleState;

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
GuiCameraToggleState InitGuiCameraToggle(void);
void GuiCameraToggle(GuiCameraToggleState *state);
Rectangle GetGuiCameraToggleBounds(void);

#ifdef __cplusplus
}
#endif

#endif // GUI_CAMERATOGGLE_H

/***********************************************************************************
*
*   GUI_CAMERATOGGLE IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_CAMERATOGGLE_IMPLEMENTATION)

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
GuiCameraToggleState InitGuiCameraToggle(void)
{
    GuiCameraToggleState state = { 0 };

    state.Toggle000Active = false;

    // Custom variables initialization

    return state;
}

Rectangle GetGuiCameraToggleBounds(void)
{
    return (Rectangle){ 24, 300 - 24, 48, 48 };
}

void GuiCameraToggle(GuiCameraToggleState *state)
{
    GuiToggle(GetGuiCameraToggleBounds(), "#184#", &state->Toggle000Active);
}

#endif // GUI_CAMERATOGGLE_IMPLEMENTATION
