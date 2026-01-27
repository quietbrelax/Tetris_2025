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
        int level;
        std::vector<Block> GetAllBlocks();
        Grid grid;
    
    private:
        int type;
        int totalLinesCleared;
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
        Sound rotateSound;
        Sound clearSound;
};