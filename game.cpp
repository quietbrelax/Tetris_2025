#include "game.h"
#include <random>
#include <ctime>

Game::Game()
{
    srand(time(0));
    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
    score = 0;
    totalLinesCleared = 0;
    level = 0;
    InitAudioDevice();
    music = LoadMusicStream("Sounds/music.mp3");
    PlayMusicStream(music);
    rotateSound = LoadSound("Sounds/rotate.mp3");
    clearSound = LoadSound("Sounds/clear.mp3");
    type = 1;
}

Game::Game(int type)
{
    srand(time(0) + 15);
    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
    score = 0;
    type = type;
    totalLinesCleared = 0;
    level = 0;
}

Game::~Game()
{
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
    UnloadMusicStream(music);
    CloseAudioDevice();
}

Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        blocks = GetAllBlocks();
    }

    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);

    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

void Game::Draw(int offset)
{
    if(type == 1){
        grid.Draw(offset);
        currentBlock.Draw(11, 11, offset);
        switch (nextBlock.id)
        {
        case 3:
            nextBlock.Draw(255, 290, offset);
            break;
        case 4:
            nextBlock.Draw(255, 280, offset);
            break;
        default:
            nextBlock.Draw(270, 270, offset);
            break;
        }

    }

    else{

        grid.Draw(0);
        currentBlock.Draw(11, 11, 0);
        switch (nextBlock.id)
        {
        case 3:
            nextBlock.Draw(255, 290, 0);
            break;
        case 4:
            nextBlock.Draw(255, 280, 0);
            break;
        default:
            nextBlock.Draw(270, 270, 0);
            break;
        }

    }
    
    
}

void Game::HandleInput(){
    int keyPressed = GetKeyPressed();

    if(gameOver && keyPressed != 0)
    {
        gameOver = false;
        Reset();
        return;
    }

    if(type == 1){
        switch (keyPressed)
        {
        case KEY_LEFT:
            MoveBlockLeft();
            break;
        case KEY_RIGHT:
            MoveBlockRight();
            break;
        case KEY_DOWN:
            MoveBlockDown();
            UpdateScore(0, 1);
            break;
        case KEY_UP:
            RotateBlock();
            break;
        }

        return;

    }

       
}

void Game::HandleInput(int keyPressed){
    if(gameOver){
        return;
    }
        switch (keyPressed)
        {
        case KEY_LEFT:
            MoveBlockLeft();
            break;
        case KEY_A:
            MoveBlockLeft();
            break;

        case KEY_RIGHT:
            MoveBlockRight();
            break;
            
        case KEY_D:
            MoveBlockRight();
            break;

        case KEY_DOWN:
            MoveBlockDown();
            UpdateScore(0, 1);
            break;
        
        case KEY_S:
            MoveBlockDown();
            UpdateScore(0, 1);
            break;

        case KEY_UP:
            RotateBlock();
            break;
        
        case KEY_W:
            RotateBlock();
            break;

        }


}

void Game::MoveBlockLeft()
{
    if(!gameOver)
    {
        currentBlock.Move(0, -1);
        if(IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, 1);
        }
    }
}

void Game::MoveBlockRight()
{
    if(!gameOver)
    {
        currentBlock.Move(0, 1);
        if(IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, -1);
        }
    }
}

void Game::MoveBlockDown()
{
    if(!gameOver)
    {
        currentBlock.Move(1, 0);
        if(IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(-1, 0);
            LockBlock();
        }
    }
}

bool Game::IsBlockOutside(){
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item : tiles)
    {
        if(grid.IsCellOutside(item.row, item.col))
        {
            return true;
        }
    }
    return false;
}

void Game::RotateBlock(){
    if(!gameOver)
    {
        currentBlock.Rotate();

        if(IsBlockOutside())
        {
            currentBlock.UndoRotation();
        }
        else
        {
            PlaySound(rotateSound);
        }
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item: tiles)
    {
        grid.grid[item.row][item.col] = currentBlock.id;
    }

    currentBlock = nextBlock;
    if(BlockFits() == false)
    {
        gameOver = true;
    }
    nextBlock = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    if(rowsCleared > 0)
    {
        if(type == 1){
            PlaySound(clearSound);
        }
        UpdateScore(rowsCleared, 0);
    } 
}

bool Game::BlockFits(){
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if(grid.IsCellEmpty(item.row, item.col) == false)
        {
            return false;
        }
    }

    return true;
    
}

void Game::Reset()
{
    grid.Initialize();
    blocks = GetAllBlocks();

    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();

    score = 0;
}

void Game::UpdateScore(int LinesCleared, int MovedDownPoints)
{
    totalLinesCleared += LinesCleared;
    level = int(totalLinesCleared/5);

    switch (LinesCleared)
    {
    case 1:
        score += 40 * (level + 1);
        break;
    
    case 2:
        score += 100 * (level + 1);
        break;
    case 3:
        score += 300 * (level + 1);
        break;

    case 4:
        score =+ 1200 * (level + 1);
        break;
    
    default:
        break;
    }

    score += MovedDownPoints;
}