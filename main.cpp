#include <raylib.h>
#include "inc/vars.hpp"
#include "inc/funcs.hpp"
#include "inc/structs.hpp"
#include "inc/button.hpp"
using namespace std;

enum class AppState {
    MENU,
    SINGLEPLAYER,
    MULTIPLAYER_LOBBY,   // placeholder for host/join screen, built later
    MULTIPLAYER_GAME     // placeholder, built once client networking exists
};

int main() {
    InitWindow(WIDTH, HEIGHT, "Connect Four");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    AppState state = AppState::MENU;

    Button singleplayerBtn{ {WIDTH/2.0f - 100, 200, 200, 60 }};
    Button multiplayerBtn { {WIDTH/2.0f - 100, 290, 200, 60 }};
    Button quitBtn        { {WIDTH/2.0f - 100, 380, 200, 60 }};

    Game game{};
    int winner = 0;
    bool winLineFound = false;

    while (!WindowShouldClose()) {
        Vector2 pos = GetMousePosition();

        // ---- UPDATE ----
        switch (state) {
            case AppState::MENU: {
                if (singleplayerBtn.Pressed(pos)) {
                    game = Game{};
                    winner = 0;
                    winLineFound = false;
                    state = AppState::SINGLEPLAYER;
                } else if (multiplayerBtn.Pressed(pos)) {
                    state = AppState::MULTIPLAYER_LOBBY;
                } else if (quitBtn.Pressed(pos)) {
                    CloseWindow();
                    return 0;
                }
                break;
            }

            case AppState::SINGLEPLAYER: {
                if (!game.game_over && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    for (int i = 0; i < COLS; i++)
                        if (CheckCollisionPointRec(pos, GetColumnRect(i)))
                            game.MakeMove((game.turn % 2 == 0) ? 1 : 2, i);
                }
                if (!game.game_over) {
                    if (game.CheckWin(1)) { winner = 1; game.game_over = true; }
                    else if (game.CheckWin(2)) { winner = 2; game.game_over = true; }
                }
                if (game.game_over && !winLineFound) {
                    game.FindWinningLine(winner);
                    winLineFound = true;
                }
                // Press ESC (or add a dedicated back-button) to return to menu
                if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_CAPS_LOCK)) state = AppState::MENU;
                break;
            }

            case AppState::MULTIPLAYER_LOBBY:
            case AppState::MULTIPLAYER_GAME:
                // built once client-side networking is wired up
                if (IsKeyPressed(KEY_ESCAPE)) state = AppState::MENU;
                break;
        }

        // ---- DRAW ----
        BeginDrawing();
        ClearBackground(BLACK);

        switch (state) {
            case AppState::MENU: {
                DrawText("Connect 4", WIDTH/2 - 110, 100, 40, WHITE);
                singleplayerBtn.Draw();
                DrawText("Single Player", singleplayerBtn.rec.x + 32, singleplayerBtn.rec.y + 18, 20, BLACK);
                multiplayerBtn.Draw();
                DrawText("Multiplayer", multiplayerBtn.rec.x + 42, multiplayerBtn.rec.y + 18, 20, BLACK);
                quitBtn.Draw();
                DrawText("Quit", quitBtn.rec.x + 80, quitBtn.rec.y + 18, 20, BLACK);
                break;
            }

            case AppState::SINGLEPLAYER: {
                DrawRectangleRounded(BoardBackground, 0.1, 15, GRAY);
                DrawHoles();
                DrawPreview(pos, game);
                DrawChips(game);
                if (game.game_over) {
                    if (winner == 1) DrawText("Player 1 Wins!", 50, 50, 30, WHITE);
                    else if (winner == 2) DrawText("Player 2 Wins!", 50, 50, 30, WHITE);
                    for (size_t i = 0; i < game.winningCells.size(); i++) {
                        Vector2 starPos = CellToScreen(game.winningCells[i].row, game.winningCells[i].col);
                        DrawStar(starPos, 30, 10, 5, GOLD);
                    }
                }
                break;
            }

            case AppState::MULTIPLAYER_LOBBY:
                DrawText("Multiplayer coming soon...", WIDTH/2 - 150, 200, 20, WHITE);
                break;

            case AppState::MULTIPLAYER_GAME:
                break;
        }

        EndDrawing();
        if (game.game_over) game.EndGame();
    }

    CloseWindow();
    return 0;
}
