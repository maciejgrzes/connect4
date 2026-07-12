#include "vars.h"
#include <cmath>
#include <raylib.h>

void DrawStar(Vector2 center, float outerRadius, float innerRadius, int points, Color color) {
    const float PI2 = 2.0f * PI;
    const int vertexCount = points * 2 + 2;

    Vector2 vertices[64];

    vertices[0] = center;

    for (int i = 0; i <= points * 2; i++)
    {
        float angle = -PI / 2.0f + i * PI / points;
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;

        vertices[i + 1] = {
            center.x + cosf(angle) * radius,
            center.y + sinf(angle) * radius
        };
    }

    DrawTriangleFan(vertices, vertexCount, color);
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

bool FindWinningLine(int board[ROWS][COLS], Cell (&winningCells)[4]) {
    // Horizontal
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c <= COLS - 4; c++)
        {
            if (board[r][c] != 0 &&
                board[r][c+1] != 0 &&
                board[r][c+2] != 0 &&
                board[r][c+3] != 0)
            {
                winningCells[0] = {r, c};
                winningCells[1] = {r, c+1};
                winningCells[2] = {r, c+2};
                winningCells[3] = {r, c+3};
                return true;
            }
        }
    }

    // Vertical
    for (int r = 0; r <= ROWS - 4; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            if (board[r][c] != 0 &&
                board[r+1][c] != 0 &&
                board[r+2][c] != 0 &&
                board[r+3][c] != 0)
            {
                winningCells[0] = {r, c};
                winningCells[1] = {r+1, c};
                winningCells[2] = {r+2, c};
                winningCells[3] = {r+3, c};
                return true;
            }
        }
    }

    // Diagonal bottom-left -> top-right
    for (int r = 3; r < ROWS; r++)
    {
        for (int c = 0; c <= COLS - 4; c++)
        {
            if (board[r][c] != 0 &&
                board[r-1][c+1] != 0 &&
                board[r-2][c+2] != 0 &&
                board[r-3][c+3] != 0)
            {
                winningCells[0] = {r, c};
                winningCells[1] = {r-1, c+1};
                winningCells[2] = {r-2, c+2};
                winningCells[3] = {r-3, c+3};
                return true;
            }
        }
    }

    // Diagonal top-left -> bottom-right
    for (int r = 0; r <= ROWS - 4; r++)
    {
        for (int c = 0; c <= COLS - 4; c++)
        {
            if (board[r][c] != 0 &&
                board[r+1][c+1] != 0 &&
                board[r+2][c+2] != 0 &&
                board[r+3][c+3] != 0)
            {
                winningCells[0] = {r, c};
                winningCells[1] = {r+1, c+1};
                winningCells[2] = {r+2, c+2};
                winningCells[3] = {r+3, c+3};
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
        if (CheckCollisionPointRec(pos, columns[i])) {
            DrawCircle(board_top_left_x_for_holes + i * (hole_diameter + gap_between_holes), 
                    board_top_left_y_for_holes + heights[i] * (hole_diameter + gap_between_holes), 
                    50, (turn % 2 == 0) ? semi_red : semi_green);
        }
    }   
}
