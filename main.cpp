#include <raylib.h>

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

    int board[ROWS][COLS] = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

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

        if (CheckWin(board, 1)) {
            DrawText("Player 1 Wins!", 50, 50, 50, WHITE);
            game_over = true;
        } else if (CheckWin(board, 2)) {
            DrawText("Player 2 Wins!", 50, 50, 50, WHITE);
            game_over = true;
        }

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
