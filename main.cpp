#include <raylib.h>

#include "inc/vars.hpp"
#include "inc/funcs.hpp"
#include "inc/structs.hpp"
using namespace std;

int main() {
    InitWindow(WIDTH, HEIGHT, "Connect 4");
    SetTargetFPS(60);
    Game game{};

    int winner = 0;
    bool winLineFound = false;

    while (!WindowShouldClose()) {
        Vector2 pos = GetMousePosition();

        if (!game.game_over && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i = 0; i < COLS; i++)
                if (CheckCollisionPointRec(pos, GetColumnRect(i)))
                    game.MakeMove((game.turn % 2 == 0) ? 1 : 2, i);
        }

        if (!game.game_over) {
            if (game.CheckWin(1)) {
                winner = 1;
                game.game_over = true;
            } else if (game.CheckWin(2)) {
                winner = 2;
                game.game_over = true;
            }
        }

        if (game.game_over && !winLineFound) {
            game.FindWinningLine(winner);
            winLineFound = true;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangleRounded(BoardBackground, 0.1, 15, GRAY);

        DrawHoles();
        DrawPreview(pos, game);
        DrawChips(game);

        if (game.game_over) {
            if (winner == 1)
                DrawText("Player 1 Wins!", 50, 50, 30, WHITE);
            else if (winner == 2)
                DrawText("Player 2 Wins!", 50, 50, 30, WHITE);

            for (size_t i = 0; i < game.winningCells.size(); i++) {
                Vector2 starPos = CellToScreen(game.winningCells[i].row, game.winningCells[i].col);
                DrawStar(starPos, 30, 10, 5, GOLD);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
