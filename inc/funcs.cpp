#include "vars.h"

bool check_win(int board[ROWS][COLS], int player)
{
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
