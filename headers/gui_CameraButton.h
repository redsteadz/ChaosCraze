/*******************************************************************************************
*
*   CameraButton v1.0.0 - Tool Description
*
*   MODULE USAGE:
*       #define GUI_CAMERABUTTON_IMPLEMENTATION
*       #include "gui_CameraButton.h"
*
*       INIT: GuiCameraButtonState state = InitGuiCameraButton();
*       DRAW: GuiCameraButton(&state);
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

#ifndef GUI_CAMERABUTTON_H
#define GUI_CAMERABUTTON_H

typedef struct {
    int clicked;
    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiCameraButtonState;

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
GuiCameraButtonState InitGuiCameraButton(void);
void GuiCameraButton(GuiCameraButtonState *state);
static void Button000();

#ifdef __cplusplus
}
#endif

#endif // GUI_CAMERABUTTON_H

/***********************************************************************************
*
*   GUI_CAMERABUTTON IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_CAMERABUTTON_IMPLEMENTATION)

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
GuiCameraButtonState InitGuiCameraButton(void)
{
    GuiCameraButtonState state = { 0 };


    // Custom variables initialization

    return state;
}
static void Button000(bool &PhoneBox001Active)
{
    // TODO: Implement control logic
    // Updates the PhoneWindow Active State 
    PhoneBox001Active = !PhoneBox001Active;
}


void GuiCameraButton(GuiCameraButtonState *state, bool *PhoneBox001Active)
{
    if (GuiButton((Rectangle){ 24, 300 - 24, 48, 48 }, "#184#")) Button000(*PhoneBox001Active); 
}

#endif // GUI_CAMERABUTTON_IMPLEMENTATION
