#include <raylib.h>
#include <string>

#include "inc/vars.h"
using namespace std;

int main() {
    InitWindow(WIDTH, HEIGHT, "Connect Four");
    SetTargetFPS(60);

    string column = "0";
    
    int hits_one = 5;
    int hits_two = 5;
    int hits_three = 5;
    int hits_four = 5;
    int hits_five = 5;
    int hits_six = 5;
    int hits_seven = 5;

    int board[6][7] = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleRounded(BoardBackground, 0.1, 15, GRAY);


        for (int j = 0; j < 6; j++) {
            for (int i = 0; i < 7; i++) {
                DrawCircle(board_top_left_x_for_holes + i * (hole_diameter + gap_between_holes), 
                           board_top_left_y_for_holes + j * (hole_diameter + gap_between_holes), 
                           50, BLACK);
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 pos = GetMousePosition();
            if (CheckCollisionPointRec(pos, column_one)) {
                column = "1";
                board[hits_one][0] = 1;
                hits_one--;
            } else if (CheckCollisionPointRec(pos, column_two)) {
                column = "2";
                board[hits_two][1] = 1;
                hits_two--;
            } else if (CheckCollisionPointRec(pos, column_three)) {
                column = "3";
                board[hits_three][2] = 1;
                hits_three--;
            } else if (CheckCollisionPointRec(pos, column_four)) {
                column = "4";
                board[hits_four][3] = 1;
                hits_four--;
            } else if (CheckCollisionPointRec(pos, column_five)) {
                column = "5";
                board[hits_five][4] = 1;
                hits_five--;
            } else if (CheckCollisionPointRec(pos, column_six)) {
                column = "6";
                board[hits_six][5] = 1;
                hits_six--;
            } else if (CheckCollisionPointRec(pos, column_seven)) {
                column = "7";
                board[hits_seven][6] = 1;
                hits_seven--;
            }
        }
        DrawText(column.c_str(), 50, 50, 50, WHITE);


        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 7; j++) {
                if (board[i][j] == 1) {
                    DrawCircle(board_top_left_x_for_holes + j * (hole_diameter + gap_between_holes),
                               board_top_left_y_for_holes + i * (hole_diameter + gap_between_holes), 
                               50, RED);
                }
            }
        }

        EndDrawing();
    }
}
