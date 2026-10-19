#include <raylib.h>
#include <enet/enet.h>
#include <string>
#include "inc/inputbox.hpp"
#include "inc/vars.hpp"
#include "inc/funcs.hpp"
#include "inc/game.hpp"
#include "inc/button.hpp"
#include "../network/inc/client.hpp"
using namespace std;

enum class AppState {
    MENU,
    SINGLEPLAYER,
    MULTIPLAYER_LOBBY,
    MULTIPLAYER_GAME
};

int main() {
    InitWindow(WIDTH, HEIGHT, "Connect Four");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    AppState state = AppState::MENU;

    Button singleplayerBtn{ {WIDTH/2.0f - 100, 200, 200, 60 }};
    Button multiplayerBtn { {WIDTH/2.0f - 100, 290, 200, 60 }};
    Button quitBtn        { {WIDTH/2.0f - 100, 380, 200, 60 }};

    Button hostBtn{ {WIDTH/2.0f - 100, 200, 200, 60 }};
    Button joinBtn { {WIDTH/2.0f - 100, 290, 200, 60 }};

    InputBox ipBox = { {WIDTH/2.0f - 100, 400, 200, 40}, "", false, 32};

    Game game{};
    int winner = 0;
    bool winLineFound = false;

    Client netClient;
    string ip = "192.168.1.6";

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
                            game.MakeMove((game.turn % 2 == 0) ? 2 : 1, i);
                }

                if (!game.game_over) {
                    if (game.CheckWin(1)) { winner = 1; game.game_over = true; }
                    else if (game.CheckWin(2)) { winner = 2; game.game_over = true; }
                    else if (!game.CheckWin(1) && !game.CheckWin(2) && game.FullBoard()) { winner = 3; game.game_over = true; }
                }

                if (game.game_over && !winLineFound) {
                    game.FindWinningLine(winner);
                    winLineFound = true;
                }

                if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_CAPS_LOCK)) state = AppState::MENU;
                break;
            }

            case AppState::MULTIPLAYER_LOBBY: {
                ipBox.Update();
                string pushedValue = ipBox.PushValue();
                if (IsValidIPv4(pushedValue)) {
                    ip = pushedValue;
                }

                if (hostBtn.Pressed(pos)) {
                    if (netClient.Connect(ip.c_str(), 7777)) {
                        state = AppState::MULTIPLAYER_GAME;
                    } else {
                        string connectError = "Couldn't connect. Is server running?";
                    }
                } else if (joinBtn.Pressed(pos)) {
                    if (netClient.Connect(ip.c_str(), 7777)) {
                        state = AppState::MULTIPLAYER_GAME;
                    } else {
                        string connectError = "Couldn't connect. Is server running?";
                    }
                }

                if (IsKeyPressed(KEY_ESCAPE)) state = AppState::MENU;
                break;
            }

            case AppState::MULTIPLAYER_GAME: {
                netClient.Poll();

                if (!netClient.connected) {
                    state = AppState::MENU;
                    break;
                }

                if (!netClient.game_over && netClient.myPlayerId == netClient.turn
                    && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    for (int i = 0; i < COLS; i++)
                        if (CheckCollisionPointRec(pos, GetColumnRect(i)))
                            netClient.SendMove(i);
                }

                if (netClient.game_over && IsKeyPressed(KEY_SPACE)) {
                    netClient.SendReady();
                }

                if (IsKeyPressed(KEY_ESCAPE)) {
                    netClient.Disconnect();
                    state = AppState::MENU;
                }
                break;
            }
        }

        // ---- DRAW ----
        BeginDrawing();
        ClearBackground(BLACK);

        switch (state) {
            case AppState::MENU: {
                DrawText("Connect 4", WIDTH/2 - 110, 100, 40, WHITE);
                singleplayerBtn.Draw("Single Player");
                multiplayerBtn.Draw("Multiplayer");
                quitBtn.Draw("Quit");
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
                    else if (winner == 3) DrawText("Draw!", 50, 50, 30, WHITE);
                    for (size_t i = 0; i < game.winningCells.size(); i++) {
                        Vector2 starPos = CellToScreen(game.winningCells[i].row, game.winningCells[i].col);
                        DrawStar(starPos, 30, 10, 5, GOLD);
                    }
                }
                break;
            }

            case AppState::MULTIPLAYER_LOBBY:
                DrawText("Multiplayer", WIDTH/2 - 110, 100, 40, WHITE);
                joinBtn.Draw("Join");
                hostBtn.Draw("Host");
                ipBox.Draw("Ip: (Default is localhost)");
                break;

            case AppState::MULTIPLAYER_GAME: {
                DrawRectangleRounded(BoardBackground, 0.1, 15, GRAY);
                DrawHoles();
                DrawPreview(pos, netClient);
                DrawChips(netClient);

                if (netClient.game_over) {
                    if (netClient.winner == 1) DrawText("Player 1 Wins!", 50, 50, 30, WHITE);
                    else if (netClient.winner == 2) DrawText("Player 2 Wins!", 50, 50, 30, WHITE);
                    else if (netClient.winner == 3) DrawText("Draw!", 50, 50, 30, WHITE);
                    for (size_t i = 0; i < netClient.winningCells.size(); i++) {
                        Vector2 starPos = CellToScreen(netClient.winningCells[i].row, netClient.winningCells[i].col);
                        DrawStar(starPos, 30, 10, 5, GOLD);
                    }
                    DrawText("Press SPACE when ready for next round", 50, 50, 20, WHITE);
                }
                break;
            }
        }

        EndDrawing();
        if (game.game_over && IsKeyPressed(KEY_SPACE)) {
            game.EndGame();
            winner = 0;
            winLineFound = false;
        }
    }

    netClient.Disconnect();
    CloseWindow();
    return 0;
}
