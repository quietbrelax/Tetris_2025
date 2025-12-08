#include "master.h"
#include <random>
#include <ctime>

Master::Master()
{
    phase = MENU;
}


void Master::masterHandleInput()
{
    int keyPressed;


    switch (phase)
    {
    case MENU:
    {
        //keyPressed = GetKeyPressed();
        //if(keyPressed != 0)
        //{
        //    phase = SINGLEPLAYER;
        //}
        printf("Entrou no phase do master\n");
        break;
    }
    case SINGLEPLAYER:

        phase = SINGLEPLAYER;
        player1.HandleInput();
        break;

    case GAMEOVER:

        if(player1.gameOver && keyPressed != 0)
        {
            player1.gameOver = false;
            player1.Reset();
            phase = SINGLEPLAYER;
        }
        break;
    default:
        break;
    }

}

void Master::Draw()
{

}


int Master::getPhase()
{
    return phase;
}