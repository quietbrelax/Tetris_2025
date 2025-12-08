#include "block.h"
#include <vector>
#include "position.h"

Block::Block()
{
    cellSize = 30;
    rotationState = 0;
    colors = GetCellColors();
    rowOffset = 0;
    colOffset = 0;
}

void Block::Draw(int offsetX, int offsetY, int offsetPlayer)
{
    std::vector<Position> tiles = GetCellPositions();
    for ( Position item : tiles)
    {
        DrawRectangle(item.col * cellSize + offsetX + offsetPlayer, item.row * cellSize + offsetY, cellSize - 1, cellSize - 1, colors[id]);
    }
}

void Block::Move(int rows, int columns)
{
    rowOffset += rows;
    colOffset += columns;
}

void Block::Rotate()
{
    rotationState++;
    rotationState = (rotationState%(int(cells.size())));
}

void Block::UndoRotation()
{
    rotationState--;
    if(rotationState == -1)
    {
        rotationState = cells.size() - 1;
    }
    
}

std::vector<Position> Block::GetCellPositions()
{
    std::vector<Position> tiles = cells[rotationState];
    std::vector<Position> movedTiles;

    for(Position item: tiles)
    {
        Position newPos = Position(item.row + rowOffset, item.col + colOffset);
        movedTiles.push_back(newPos);
    }

    return movedTiles;
}

