#pragma once
#include "grid.h"
#include "blocks.cpp"
#include <vector>

class Game
{
    public:
        Game();
        ~Game();
        void Draw();
        void HandleInput();
        void MoveBlockDown();
        bool gameOver;
        int score;
        Music music;
    
    private:
        void MoveBlockLeft();
        void MoveBlockRight();
        Block GetRandomBlock();
        std::vector<Block> GetAllBlocks();
        bool IsBlockOutside();
        void RotateBlock();
        void LockBlock();
        bool BlockFits();
        void UpdateScore(int LinesCleared, int MovedDownPoints);
        std::vector<Block> blocks;
        Block currentBlock;
        Block nextBlock;
        Grid grid;
        void Reset();
        Sound rotateSound;
        Sound clearSound;
};