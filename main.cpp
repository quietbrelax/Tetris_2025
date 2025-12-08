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


#define MENU 0
#define SINGLEPLAYER 1
#define MULTILOCAL 2
#define MULTIONLINE 3
#define GAMEOVER 4
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

double lastUpdateTime = 0;
int phase = MENU;


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

    float spacing = 40.0f;      // left/right spacing between vertical lines
    float depthSpacing = 20.0f; // vertical spacing between perspective rows

    int bottom = 620;       // bottom of grid
    int top = 500;      // horizon line (middle of window)

    float y = bottom;
    float step = depthSpacing;


   // 300 x 600
   InitWindow(1000, 620, "Tetris");
   SetTargetFPS(60);

   Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
   
   Game game = Game();
   Game playerTwo = Game(2);
   int keyPressed;
   int i = 0;
   int j = 0;
   int aux = 0;
   int scoreP1 = 0;
   int scoreP2 = -1;

   while(WindowShouldClose() == false)
   {

    switch (phase)
    {
    case MENU:
    {
        std::vector<Block> blocks = game.GetAllBlocks();
        keyPressed = GetKeyPressed();
        BeginDrawing();
        ClearBackground(darkBlue);
        DrawRectangle(15, 15, 970, 590, BLACK);
        DrawTextEx(font, "TETRIS", {400, 50}, 100, 2, WHITE);
        DrawTextEx(font, "Produced by: Luiz Gariglios and Clara Ruas", {120, 150}, 32, 2, WHITE);
        DrawTextEx(font, "Final project for Object Oriented Programing, ENSTA", {100, 200}, 32, 2, WHITE);

        if(EventTriggered(0.8))
        {
            i++;
            i = i%7;
        }

        blocks.at(i).Draw(370, 250, 0);

        y = bottom;
        step = depthSpacing;
        aux = 0;
        j = 0;

        while (y >= top) {
            DrawLine(15 + aux, (int)y, 985 - aux, (int)y, purple);

            // perspective: spacing gets smaller as it gets close to "top"
            step = step * 0.92f;   // shrink each step
            y -= step;
            j++;
            if(j > 3) 
            {
                aux += 80;
            }

        }

        // --------- Draw Vertical Lines (converging) ---------
        int numLines = 970 / spacing;

        for (int i = - numLines; i <= numLines; i++) {
            float x = 970 / 2 + i * spacing + 15;

            // Each vertical line goes from bottom → converging point
            DrawLine(
                (int)x, bottom,                 // bottom point
                500, top,           // horizon converge point
                purple
            );
        }

     

        DrawTextEx(font, "PRESS S FOR SINGLEPLAYER", {200, 340}, 46, 2, WHITE);
        DrawTextEx(font, "PRESS L FOR LOCAL MULTIPLAYER", {200, 390}, 46, 2, WHITE);
        DrawTextEx(font, "PRESS O FOR ONLINE MULTIPLAYER", {200, 440}, 46, 2, WHITE);


        if(keyPressed == KEY_S)
        {
            phase = SINGLEPLAYER;
        }
        else if(keyPressed == KEY_L)
        {
            phase = MULTILOCAL;
        }

        EndDrawing();
        break;
    }
    case SINGLEPLAYER:
    {
        UpdateMusicStream(game.music);
        game.HandleInput();
        if(EventTriggered(0.2))
        {
            game.MoveBlockDown();
        }
        BeginDrawing();
        ClearBackground(darkBlue);
        DrawTextEx(font, "Score", {665, 15}, 38, 2, WHITE);
        DrawTextEx(font, "Next", {670, 175}, 38, 2, WHITE);
        if(game.gameOver)
        {
            //DrawTextEx(font, "GAME OVER", {320, 450}, 38, 2, WHITE);
            scoreP1 = game.score;
            phase = GAMEOVER;
        }

        DrawRectangleRounded({620, 55, 170, 60}, 0.3, 6, lightBlue);
    
        char scoreText[10];
        sprintf(scoreText, "%d", game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
        DrawTextEx(font, scoreText, {620 + (170 - textSize.x)/2, 65}, 38, 2, WHITE);

        DrawRectangleRounded({620, 215, 170, 180}, 0.3, 6, lightBlue);


        game.Draw(300);
    
        EndDrawing();

        break;
    }
    case MULTILOCAL:
    {
        
        UpdateMusicStream(game.music);
        //

        keyPressed = GetKeyPressed();

        //game.HandleInput(keyPressed);

        if(keyPressed == KEY_A || keyPressed == KEY_W || keyPressed == KEY_S || keyPressed == KEY_D)
        {
            playerTwo.HandleInput(keyPressed);
        }

        else{
            game.HandleInput(keyPressed);
        }
        
        
        if(EventTriggered(0.2))
        {
            game.MoveBlockDown();
            playerTwo.MoveBlockDown();
        }
        BeginDrawing();
        ClearBackground(darkBlue);
        DrawTextEx(font, "Score", {365, 15}, 38, 2, WHITE);
        DrawTextEx(font, "Next", {370, 175}, 38, 2, WHITE);

        DrawTextEx(font, "Score", {865, 15}, 38, 2, WHITE);
        DrawTextEx(font, "Next", {870, 175}, 38, 2, WHITE);
        if(game.gameOver && playerTwo.gameOver)
        {
            //DrawTextEx(font, "GAME OVER", {320, 450}, 38, 2, WHITE);
            scoreP1 = game.score;
            scoreP2 = playerTwo.score;
            phase = GAMEOVER;
        }

        DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lightBlue);
        DrawRectangleRounded({820, 55, 170, 60}, 0.3, 6, lightBlue);
    
        char scoreText[10];
        char scoreTextTwo[10];
        sprintf(scoreText, "%d", game.score);
        sprintf(scoreTextTwo, "%d", playerTwo.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
        Vector2 textSizeTwo = MeasureTextEx(font, scoreTextTwo, 38, 2);
        DrawTextEx(font, scoreText, {320 + (170 - textSize.x)/2, 65}, 38, 2, WHITE);
        DrawTextEx(font, scoreTextTwo, {820 + (170 - textSizeTwo.x)/2, 65}, 38, 2, WHITE);

        DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, lightBlue);

        DrawRectangleRounded({820, 215, 170, 180}, 0.3, 6, lightBlue);


        game.Draw(0);
        playerTwo.Draw(0);
    
        EndDrawing();
    }
        
        break;
    case MULTIONLINE:
        
        break;

    case GAMEOVER:
    {
        BeginDrawing();
        game.HandleInput();
        playerTwo.Reset();
        playerTwo.gameOver = false;
        if(!game.gameOver){
            phase = MENU;
        }
        ClearBackground(darkBlue);
        DrawRectangle(15, 15, 970, 590, BLACK);
        DrawTextEx(font, "GAME OVER", {360, 20}, 100, 2, WHITE);

        if(scoreP2 == -1)
        {
            DrawTextEx(font, "SCORE:", {390, 150}, 56, 2, WHITE);
            char scoreText[10];
            sprintf(scoreText, "%d", scoreP1);
            Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
            DrawTextEx(font, scoreText, {390 + (170 - textSize.x)/2, 240}, 40, 2, WHITE);
        }

        else
        {

            DrawTextEx(font, "PLAYER 1:", {150, 220}, 56, 2, WHITE);
            char scoreText[10];
            sprintf(scoreText, "%d", scoreP1);
            Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
            DrawTextEx(font, scoreText, {220 + (170 - textSize.x)/2, 160}, 38, 2, WHITE);

            DrawTextEx(font, "PLAYER 2:", {150, 320}, 56, 2, WHITE);

            char scoreTextTwo[10];
            sprintf(scoreTextTwo, "%d", scoreP2);
            Vector2 textSizeTwo = MeasureTextEx(font, scoreTextTwo, 38, 2);
            DrawTextEx(font, scoreTextTwo, {320 + (170 - textSizeTwo.x)/2, 160}, 38, 2, WHITE);

            if(scoreP1 > scoreP2)
            {
                DrawTextEx(font, "PLAYER 1 WINS!!", {270, 240}, 56, 2, WHITE);
            }

            else
            {
                DrawTextEx(font, "PLAYER 2 WINS!!", {270, 240}, 56, 2, WHITE);
            }

        }

        

        DrawTextEx(font, "PRESS ANYTHING TO RETURN TO THE MENU", {40, 560}, 38, 2, WHITE);
        EndDrawing();

        break;
    }
    default:
        break;
    }
    

   }

   CloseWindow();

   return 0;
}