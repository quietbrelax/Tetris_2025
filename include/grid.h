#pragma once
#include <vector>
#include "raylib.h"

class Grid{

    public:
        Grid();
        void Initialize();
        int grid[20][10];
        void Draw();
        bool IsCellOutside(int row, int column);
        void print();
        bool IsCellEmpty(int row, int column);
        int ClearFullRows();

    private:
        bool IsRowFull(int row);
        void ClearRow(int row);
        void MoveRowDown(int row, int numRows);
        int numRows;
        int numCols;
        int cellSize;
        std::vector<Color> colors;
};
