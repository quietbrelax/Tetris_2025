#pragma once
#include <vector>
#include <map>
#include "game.h"

#define MENU 0
#define SINGLEPLAYER 1
#define MULTILOCAL 2
#define MULTIONLINE 3
#define GAMEOVER 4

class Master
{
    public:
    Master();
    void masterHandleInput();
    void Draw();
    int getPhase();
    void gameOver();

    Game player1 = Game();
    
    

    private:
    
    Game player2 = Game();
    int phase;
};