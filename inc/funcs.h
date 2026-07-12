#include "vars.h"

#ifndef FUNCS
#define FUNCS

void DrawStar(Vector2 center, float outerRadius, float innerRadius, int points, Color color);

bool CheckWin(int board[ROWS][COLS], int player);

bool FindWinningLine(int board[ROWS][COLS], Vector2 (&winningCells)[4]);

void DrawHoles();

void DrawPreview(Vector2 pos, Rectangle columns[7], int heights[7], int turn);

#endif
