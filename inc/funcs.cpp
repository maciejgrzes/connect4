#include "vars.h"
#include <cmath>
#include <raylib.h>
#include <vector>

void DrawStar(Vector2 center, float outerRadius, float innerRadius, int points, Color color) {
    for (int i = 0; i < points * 2; i++)
    {
        float angle1 = -PI / 2 + i * PI / points;
        float angle2 = -PI / 2 + (i + 1) * PI / points;

        float r1 = (i % 2 == 0) ? outerRadius : innerRadius;
        float r2 = ((i + 1) % 2 == 0) ? outerRadius : innerRadius;

        Vector2 p1 = {
            center.x + cosf(angle1) * r1,
            center.y + sinf(angle1) * r1
        };

        Vector2 p2 = {
            center.x + cosf(angle2) * r2,
            center.y + sinf(angle2) * r2
        };

        DrawTriangle(center, p2, p1, color);
    }
}


bool CheckWin(int board[ROWS][COLS], int player) {
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

bool FindWinningLine(int board[ROWS][COLS], int player, std::vector<Cell> (&winningCells)) {
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

void DrawHoles() {
    for (int j = 0; j < ROWS; j++) {
        for (int i = 0; i < COLS; i++) {
            DrawCircle(board_top_left_x_for_holes + i * (hole_diameter + gap_between_holes), 
                       board_top_left_y_for_holes + j * (hole_diameter + gap_between_holes), 
                       50, BLACK);
        }
    }
}

void DrawPreview(Vector2 pos, Rectangle columns[7], int heights[7], int turn) {
    for (int i = 0; i < COLS; i++) {
        if (CheckCollisionPointRec(pos, columns[i]) && heights[i] >= 0) {
            DrawCircle(board_top_left_x_for_holes + i * (hole_diameter + gap_between_holes), 
                    board_top_left_y_for_holes + heights[i] * (hole_diameter + gap_between_holes), 
                    50, (turn % 2 == 0) ? semi_red : semi_green);
        }
    }   
}

void DrawChips(int board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == 1) {
                DrawCircle(board_top_left_x_for_holes + j * (hole_diameter + gap_between_holes),
                           board_top_left_y_for_holes + i * (hole_diameter + gap_between_holes), 
                           50, RED);
            } else if (board[i][j] == 2) {
                DrawCircle(board_top_left_x_for_holes + j * (hole_diameter + gap_between_holes),
                           board_top_left_y_for_holes + i * (hole_diameter + gap_between_holes), 
                           50, GREEN);
            }
        }
    }
}
