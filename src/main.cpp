/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute 'raylib_compile_execute' script
*   Note that compiled executable is placed in the same folder as .c file
*
*   To test the examples on Web, press F6 and execute 'raylib_compile_execute_web' script
*   Web version of the program is generated in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "game.h"
#include "colors.h"
#include <iostream>
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

double lastUpdateTime = 0;

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }

    return false;
}

int main(void)
{
   // 300 x 600
   InitWindow(500, 620, "Tetris");
   SetTargetFPS(60);

   Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
   
   Game game = Game();

   while(WindowShouldClose() == false)
   {
    UpdateMusicStream(game.music);
    game.HandleInput();
    if(EventTriggered(0.2))
    {
        game.MoveBlockDown();
    }
    BeginDrawing();
    ClearBackground(darkBlue);
    DrawTextEx(font, "Score", {365, 15}, 38, 2, WHITE);
    DrawTextEx(font, "Next", {370, 175}, 38, 2, WHITE);
    if(game.gameOver)
    {
        DrawTextEx(font, "GAME OVER", {320, 450}, 38, 2, WHITE);
    }

    DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lightBlue);
    
    char scoreText[10];
    sprintf(scoreText, "%d", game.score);
    Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
    DrawTextEx(font, scoreText, {320 + (170 - textSize.x)/2, 65}, 38, 2, WHITE);

    DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, lightBlue);


    game.Draw();
    
    EndDrawing();

   }

   CloseWindow();

   return 0;
}
