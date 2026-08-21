#pragma once

#include "vars.hpp"
#include <vector>

struct Cell {
    int row;
    int col;
};


struct Game {
    int turn = 1;

    bool game_over = false;

    int heights[COLS] = {5, 5, 5, 5, 5, 5, 5};

    int board[ROWS][COLS] = {};

    std::vector<Cell> winningCells;


    bool CheckWin(int player);

    bool FindWinningLine(int player);

    void EndGame();

    bool MakeMove(int player, int column);
};
