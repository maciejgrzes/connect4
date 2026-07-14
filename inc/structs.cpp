#include "structs.h"
#include "funcs.h"
#include "vars.h"
#include <vector>

bool Game::CheckWin(int player) {
    // Horizontal
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c <= COLS - 4; c++)
        {
            if (board[r][c] == player &&
                board[r][c+1] == player &&
                board[r][c+2] == player &&
                board[r][c+3] == player)
            {
                return true;
            }
        }
    }

    // Vertical
    for (int r = 0; r <= ROWS - 4; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            if (board[r][c] == player &&
                board[r+1][c] == player &&
                board[r+2][c] == player &&
                board[r+3][c] == player)
            {
                return true;
            }
        }
    }

    // Diagonal bottom-left -> top-right
    for (int r = 3; r < ROWS; r++)
    {
        for (int c = 0; c <= COLS - 4; c++)
        {
            if (board[r][c] == player &&
                board[r-1][c+1] == player &&
                board[r-2][c+2] == player &&
                board[r-3][c+3] == player)
            {
                return true;
            }
        }
    }

    // Diagonal top-left -> bottom-right
    for (int r = 0; r <= ROWS - 4; r++)
    {
        for (int c = 0; c <= COLS - 4; c++)
        {
            if (board[r][c] == player &&
                board[r+1][c+1] == player &&
                board[r+2][c+2] == player &&
                board[r+3][c+3] == player)
            {
                return true;
            }
        }
    }

    return false;
}

bool Game::FindWinningLine(int player) {
    // Horizontal
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c <= COLS - 4; c++)
        {
            if (board[r][c] == player &&
                board[r][c+1] == player &&
                board[r][c+2] == player &&
                board[r][c+3] == player)
            {
                winningCells.push_back({r, c});
                winningCells.push_back({r, c+1});
                winningCells.push_back({r, c+2});
                winningCells.push_back({r, c+3});
                return true;
            }
        }
    }

    // Vertical
    for (int r = 0; r <= ROWS - 4; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            if (board[r][c] == player &&
                board[r+1][c] == player &&
                board[r+2][c] == player &&
                board[r+3][c] == player)
            {
                winningCells.push_back({r, c});
                winningCells.push_back({r+1, c});
                winningCells.push_back({r+2, c});
                winningCells.push_back({r+3, c});
                return true;
            }
        }
    }

    // Diagonal bottom-left -> top-right
    for (int r = 3; r < ROWS; r++)
    {
        for (int c = 0; c <= COLS - 4; c++)
        {
            if (board[r][c] == player &&
                board[r-1][c+1] == player &&
                board[r-2][c+2] == player &&
                board[r-3][c+3] == player)
            {
                winningCells.push_back({r, c});
                winningCells.push_back({r-1, c+1});
                winningCells.push_back({r-2, c+2});
                winningCells.push_back({r-3, c+3});
                return true;
            }
        }
    }

    // Diagonal top-left -> bottom-right
    for (int r = 0; r <= ROWS - 4; r++)
    {
        for (int c = 0; c <= COLS - 4; c++)
        {
            if (board[r][c] == player &&
                board[r+1][c+1] == player &&
                board[r+2][c+2] == player &&
                board[r+3][c+3] == player)
            {
                winningCells.push_back({r, c});
                winningCells.push_back({r+1, c+1});
                winningCells.push_back({r+2, c+2});
                winningCells.push_back({r+3, c+3});
                return true;
            }
        }
    }

    return false;
}


void Game::EndGame() {
    if (game_over) {
        WaitTime(5);
        game_over = false;

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                board[i][j] = 0;
            }
        }

        for (int i = 0; i < COLS; i++) {
            heights[i] = 5;
        }

        winningCells.clear();
    }
}

void Game::MakeMove() {
    Vector2 pos = GetMousePosition();
    for (int i = 0; i < COLS; i++) {
        if (CheckCollisionPointRec(pos, GetColumnRect(i))) {
            if (heights[i] >= 0) {
                board[heights[i]][i] = (turn % 2 == 0) ? 1 : 2;
                heights[i]--;
                turn++;
            }
        }
    }
}
