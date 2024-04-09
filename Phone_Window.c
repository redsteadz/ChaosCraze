/*******************************************************************************************
*
*   Phone v1.0.0 - Tool Description
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

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Phone_Window");

    // Phone_Window: controls initialization
    //----------------------------------------------------------------------------------
    // Define controls variables
    bool WindowBox001Active = true;            // WindowBox: WindowBox001
    bool DropdownBox002EditMode = false;
    int DropdownBox002Active = 0;            // DropdownBox: DropdownBox002
    bool DropdownBox003EditMode = false;
    int DropdownBox003Active = 0;            // DropdownBox: DropdownBox003
    bool DropdownBox004EditMode = false;
    int DropdownBox004Active = 0;            // DropdownBox: DropdownBox004
    bool Button005Pressed = false;            // Button: Button005
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Implement required update logic
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            // raygui: controls drawing
            //----------------------------------------------------------------------------------
            // Draw controls
            if (DropdownBox002EditMode || DropdownBox003EditMode || DropdownBox004EditMode) GuiLock();

            if (WindowBox001Active)
            {
                WindowBox001Active = !GuiWindowBox((Rectangle){ 34, 89, 416, 416 }, "PHONE");
                GuiGroupBox((Rectangle){ 51, 129, 383, 209 }, "IMAGE");
                Button005Pressed = GuiButton((Rectangle){ 309, 473, 120, 24 }, "POST"); 
                if (GuiDropdownBox((Rectangle){ 49, 361, 120, 24 }, "ONE;TWO;THREE", &DropdownBox002Active, DropdownBox002EditMode)) DropdownBox002EditMode = !DropdownBox002EditMode;
                if (GuiDropdownBox((Rectangle){ 178, 361, 120, 24 }, "ONE;TWO;THREE", &DropdownBox003Active, DropdownBox003EditMode)) DropdownBox003EditMode = !DropdownBox003EditMode;
                if (GuiDropdownBox((Rectangle){ 308, 361, 120, 24 }, "ONE;TWO;THREE", &DropdownBox004Active, DropdownBox004EditMode)) DropdownBox004EditMode = !DropdownBox004EditMode;
            }
            
            GuiUnlock();
            //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------

