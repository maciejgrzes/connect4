#include <raylib.h>

#include "inc/vars.hpp"
#include "inc/funcs.hpp"
#include "inc/structs.hpp"
using namespace std;

int main() {
    InitWindow(WIDTH, HEIGHT, "Connect Four");
    SetTargetFPS(60);

    Game game{};

    while (!WindowShouldClose()) {

        Vector2 pos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i = 0; i < COLS; i++)
                if (CheckCollisionPointRec(pos, GetColumnRect(i)))
                    game.MakeMove((game.turn % 2 == 0) ? 1 : 2, i);
        }

        if (game.CheckWin(1)) {
            DrawText("Player 1 Wins!", 50, 50, 30, WHITE);
            game.game_over = true;
        } else if (game.CheckWin(2)) {
            DrawText("Player 2 Wins!", 50, 50, 50, WHITE);
            game.game_over = true;
        }

        
        BeginDrawing();


        ClearBackground(BLACK);

        DrawRectangleRounded(BoardBackground, 0.1, 15, GRAY);
        
        DrawHoles();
        
        DrawPreview(pos, game);
        
        DrawChips(game);

        if (game.FindWinningLine(1) || game.FindWinningLine(2)) {
            for (int i = 0; i < 4; i++) {
                Vector2 starPos = CellToScreen(game.winningCells[i].row, game.winningCells[i].col);
                DrawStar(starPos, 30, 10, 5, GOLD);
            }
        }
        
        EndDrawing();
        

        game.EndGame();

    }

    CloseWindow();
    return 0;
}
