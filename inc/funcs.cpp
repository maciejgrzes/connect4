#include "funcs.h"

#include "vars.h"
#include "structs.h"
#include <cmath>
#include <raylib.h>
#include <vector>

Rectangle GetColumnRect(int col) {
    return {
        (float)(board_top_left_x + col * (hole_diameter + gap_between_holes)),
        board_top_left_y,
        hole_diameter + gap_between_holes,
        B_HEIGHT
    };
}

Vector2 CellToScreen(int col, int row) {
    return {
        (float)(board_top_left_x_for_holes + row * (hole_diameter + gap_between_holes)),
        (float)(board_top_left_y_for_holes + col * (hole_diameter + gap_between_holes))
    };
}

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
            Vector2 pos = CellToScreen(j, i);
            DrawCircle(pos.x, pos.y, hole_radius, BLACK);
        }
    }
}

void DrawPreview(Vector2 pos, int heights[7], int turn) {
    for (int i = 0; i < COLS; i++) {
        if (CheckCollisionPointRec(pos, GetColumnRect(i)) && heights[i] >= 0) {
            Vector2 pos = CellToScreen(heights[i], i);
            DrawCircle(pos.x, pos.y, hole_radius, (turn % 2 == 0) ? semi_red : semi_green);
        }
    }   
}

void DrawChips(int board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            Vector2 pos = CellToScreen(i, j);
            if (board[i][j] == 1) {
                DrawCircle(pos.x, pos.y, hole_radius, RED);
            } else if (board[i][j] == 2) {
                DrawCircle(pos.x, pos.y, hole_radius, GREEN);
            }
        }
    }
}

void EndGame(bool (&game_over), int (&board)[ROWS][COLS], int (&heights)[COLS], std::vector<Cell> (&winningCells)) {
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
