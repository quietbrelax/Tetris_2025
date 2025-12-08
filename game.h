#pragma once
#include "grid.h"
#include "blocks.cpp"
#include <vector>

class Game
{
    public:
        Game();
        Game(int type);
        ~Game();
        void Draw(int offset);
        void HandleInput();

        void HandleInput(int keyPressed);
        void MoveBlockDown();
        void Reset();
        bool gameOver;
        int score;
        Music music;
        std::vector<Block> GetAllBlocks();
    
    private:
        int type;
        void MoveBlockLeft();
        void MoveBlockRight();
        Block GetRandomBlock();
        
        bool IsBlockOutside();
        void RotateBlock();
        void LockBlock();
        bool BlockFits();
        void UpdateScore(int LinesCleared, int MovedDownPoints);
        std::vector<Block> blocks;
        Block currentBlock;
        Block nextBlock;
        Grid grid;
        Sound rotateSound;
        Sound clearSound;
};