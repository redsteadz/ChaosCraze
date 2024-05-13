/*******************************************************************************************
*
*   Menu v1.0.0 - Tool Description
*
*   MODULE USAGE:
*       #define GUI_MENU_IMPLEMENTATION
*       #include "gui_menu.h"
*
*       INIT: GuiMenuState state = InitGuiMenu();
*       DRAW: GuiMenu(&state);
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
#include "terminal.h"
// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "terminal.h"
#include <string.h>     // Required for: strcpy()

#ifndef GUI_MENU_H
#define GUI_MENU_H

typedef struct {
    // Define anchors
    Vector2 anchor01;            // ANCHOR ID:1
    
    // Define controls variables

    // Define rectangles
    Rectangle layoutRecs[3];

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiMenuState;

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
GuiMenuState InitGuiMenu(void);
void GuiMenu(GuiMenuState *state);
static void Button000();                // Button: Button000 logic
static void Button001();                // Button: Button001 logic
static void Button002();                // Button: Button002 logic

#ifdef __cplusplus
}
#endif

#endif // GUI_MENU_H

/***********************************************************************************
*
*   GUI_MENU IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_MENU_IMPLEMENTATION)

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
GuiMenuState InitGuiMenu(void)
{
    GuiMenuState state = { 0 };

    // Init anchors
    state.anchor01 = (Vector2){ 456, 472 };            // ANCHOR ID:1
    
    // Initilize controls variables

    // Init controls rectangles
    state.layoutRecs[0] = (Rectangle){ state.anchor01.x + -56, state.anchor01.y + -16, 120, 24 };// Button: Button000
    state.layoutRecs[1] = (Rectangle){ state.anchor01.x + -56, state.anchor01.y + 32, 120, 24 };// Button: Button001
    state.layoutRecs[2] = (Rectangle){ state.anchor01.x + -56, state.anchor01.y + 80, 120, 24 };// Button: Button002

    // Custom variables initialization

    return state;
}
// Button: Button000 logic
static void Button000()
{
   GuiLoadStyleTerminal();
}
// Button: Button001 logic
static void Button001()
{
    GuiLoadStyleTerminal();
}
// Button: Button002 logic
static void Button002()
{
    GuiLoadStyleTerminal();
}


void GuiMenu(GuiMenuState *state)
{
    // Const text
    const char *Button000Text = "PLAY GAME";    // BUTTON: Button000
    const char *Button001Text = "OPTIONS";    // BUTTON: Button001
    const char *Button002Text = "QUIT";    // BUTTON: Button002
    
    // Draw controls
    if (GuiButton(state->layoutRecs[0], Button000Text)) Button000(); 
    if (GuiButton(state->layoutRecs[1], Button001Text)) Button001(); 
    if (GuiButton(state->layoutRecs[2], Button002Text)) Button002(); 
}

#endif // GUI_MENU_IMPLEMENTATION
