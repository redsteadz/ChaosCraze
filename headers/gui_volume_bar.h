/*******************************************************************************************
*
*   VolumeBar v1.0.0 - Tool Description
*
*   MODULE USAGE:
*       #define GUI_VOLUME_BAR_IMPLEMENTATION
*       #include "gui_volume_bar.h"
*
*       INIT: GuiVolumeBarState state = InitGuiVolumeBar();
*       DRAW: GuiVolumeBar(&state);
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

#ifndef GUI_VOLUME_BAR_H
#define GUI_VOLUME_BAR_H

typedef struct {
    // Define anchors
    Vector2 anchor01;            // ANCHOR ID:1
    
    // Define controls variables
    bool Spinner002EditMode;
    int *Spinner002Value;   
    int value;         // Spinner: Spinner002
    const char* Spinner002Text;
    // Define rectangles
    Rectangle layoutRecs[1];

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiVolumeBarState;

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
GuiVolumeBarState InitGuiVolumeBar(void);
void GuiVolumeBar(GuiVolumeBarState *state);


#ifdef __cplusplus
}
#endif

#endif // GUI_VOLUME_BAR_H

/***********************************************************************************
*
*   GUI_VOLUME_BAR IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_VOLUME_BAR_IMPLEMENTATION)

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
GuiVolumeBarState InitGuiVolumeBar(void)
{
    GuiVolumeBarState state = { 0 };

    // Init anchors
    state.anchor01 = (Vector2){ 392, 496 };            // ANCHOR ID:1
    
    // Initilize controls variables
    state.Spinner002EditMode = false;
     state.value=0;// Spinner: Spinner002
     state.Spinner002Value=&state.value;
     state.Spinner002Text="";
    // Init controls rectangles
    state.layoutRecs[0] = (Rectangle){ state.anchor01.x + -140, state.anchor01.y + -130, 300, 40 };// Spinner: Spinner002

    // Custom variables initialization

    return state;
}


void GuiVolumeBar(GuiVolumeBarState *state)
{
    // Const text
        // Draw controls
    if (GuiSpinner(state->layoutRecs[0], state->Spinner002Text, state->Spinner002Value,0, 100, state->Spinner002EditMode)) state->Spinner002EditMode = !state->Spinner002EditMode;
}

#endif // GUI_VOLUME_BAR_IMPLEMENTATION
