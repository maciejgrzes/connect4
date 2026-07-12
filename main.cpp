#include <raylib.h>
#include <vector>

#include "inc/vars.h"
#include "inc/funcs.h"
using namespace std;

int main() {
    InitWindow(WIDTH, HEIGHT, "Connect Four");
    SetTargetFPS(60);

    int turn = 0;

    bool game_over = false;

    int heights[COLS] = {5, 5, 5, 5, 5, 5, 5};

    Rectangle columns[7] = {
        column_one,
        column_two,
        column_three,
        column_four,
        column_five,
        column_six,
        column_seven
    };

    int board[ROWS][COLS] = {};

    vector<Cell> winningCells;

    while (!WindowShouldClose()) {

        Vector2 pos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i = 0; i < 7; i++) {
                if (CheckCollisionPointRec(pos, columns[i])) {
                    if (heights[i] >= 0) {
                        board[heights[i]][i] = (turn % 2 == 0) ? 1 : 2;
                        heights[i]--;
                        turn++;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleRounded(BoardBackground, 0.1, 15, GRAY);
        DrawHoles();
        DrawPreview(pos, columns, heights, turn);
        DrawChips(board);

        if (CheckWin(board, 1)) {
            DrawText("Player 1 Wins!", 50, 50, 30, WHITE);
            game_over = true;
        } else if (CheckWin(board, 2)) {
            DrawText("Player 2 Wins!", 50, 50, 50, WHITE);
            game_over = true;
        }


        if (FindWinningLine(board, 1, winningCells) || FindWinningLine(board, 2, winningCells)) {
            for (int i = 0; i < 4; i++) {
                Vector2 starPos = {
                    (float)(board_top_left_x_for_holes + winningCells[i].col * (hole_diameter + gap_between_holes)),
                    (float)(board_top_left_y_for_holes + winningCells[i].row * (hole_diameter + gap_between_holes))
                };

                DrawStar(starPos, 30, 10, 5, GOLD);
            }
        }

        EndDrawing();

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

        }

    }

    CloseWindow();
    return 0;
}
