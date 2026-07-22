#pragma once

#include "vars.hpp"
#include <vector>

struct Cell {
    int row;
    int col;
};


struct Game {
    int turn;

    bool game_over;

    int heights[COLS] = {5, 5, 5, 5, 5, 5, 5};

    int board[ROWS][COLS];

    std::vector<Cell> winningCells;


    bool CheckWin(int player);

    bool FindWinningLine(int player);

    void EndGame();

    void MakeMove(int player, int column);
};
