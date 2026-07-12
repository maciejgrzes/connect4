#include <raylib.h>

#include "inc/vars.h"
#include "inc/funcs.h"
using namespace std;

int main() {
    InitWindow(WIDTH, HEIGHT, "Connect Four");
    SetTargetFPS(60);

    int turn = 1;

    bool game_over = false;

    int heights[COLS] = {5, 5, 5, 5, 5, 5, 5};

    int board[ROWS][COLS] = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    while (!game_over && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleRounded(BoardBackground, 0.1, 15, GRAY);


        for (int j = 0; j < ROWS; j++) {
            for (int i = 0; i < COLS; i++) {
                DrawCircle(board_top_left_x_for_holes + i * (hole_diameter + gap_between_holes), 
                           board_top_left_y_for_holes + j * (hole_diameter + gap_between_holes), 
                           50, BLACK);
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 pos = GetMousePosition();
            if (CheckCollisionPointRec(pos, column_one)) {
                if (heights[0] >= 0) {
                    board[heights[0]][0] = (turn % 2 == 0) ? 1 : 2;
                    heights[0]--;
                    turn++;
                }
            } else if (CheckCollisionPointRec(pos, column_two)) {
                if (heights[1] >= 0) {
                    board[heights[1]][1] = (turn % 2 == 0) ? 1 : 2;
                    heights[1]--;
                    turn++;
                }
            } else if (CheckCollisionPointRec(pos, column_three)) {
                if (heights[2] >= 0) {
                    board[heights[2]][2] = (turn % 2 == 0) ? 1 : 2;
                    heights[2]--;
                    turn++;
                }
            } else if (CheckCollisionPointRec(pos, column_four)) {
                if (heights[3] >= 0) {
                    board[heights[3]][3] = (turn % 2 == 0) ? 1 : 2;
                    heights[3]--;
                    turn++;
                }
            } else if (CheckCollisionPointRec(pos, column_five)) {
                if (heights[4] >= 0) {
                    board[heights[4]][4] = (turn % 2 == 0) ? 1 : 2;
                    heights[4]--;
                    turn++;
                }
            } else if (CheckCollisionPointRec(pos, column_six)) {
                if (heights[5] >= 0) {
                    board[heights[5]][5] = (turn % 2 == 0) ? 1 : 2;
                    heights[5]--;
                    turn++;
                }
            } else if (CheckCollisionPointRec(pos, column_seven)) {
                if (heights[6] >= 0) {
                    board[heights[6]][6] = (turn % 2 == 0) ? 1 : 2;
                    heights[6]--;
                    turn++;
                }
            }
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

        if (check_win(board, 1)) {
            DrawText("Player 1 Wins!", 50, 50, 50, WHITE);
            game_over = true;
        } else if (check_win(board, 2)) {
            DrawText("Player 2 Wins!", 50, 50, 50, WHITE);
            game_over = true;
        }

        EndDrawing();
    }
}
