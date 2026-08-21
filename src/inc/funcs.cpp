#include "funcs.hpp"
#include "vars.hpp"
#include "game.hpp"

#include <cmath>
#include <raylib.h>
#include <string>
#include <sstream>


bool IsValidIPv4(std::string ip) {
    std::stringstream ss(ip);
    std::string octet;
    int count = 0;

    while (std::getline(ss, octet, '.')) {
        count++;
        if (count > 4) return false;

        // must be non-empty, digits only, max 3 chars
        if (octet.empty() || octet.size() > 3) return false;
        for (char c : octet) {
            if (!isdigit((unsigned char)c)) return false;
        }

        // reject leading zeros like "01" or "007" (but allow "0" itself)
        if (octet.size() > 1 && octet[0] == '0') return false;

        int value = std::stoi(octet);
        if (value < 0 || value > 255) return false;
    }

    // must end without a trailing dot, and have exactly 4 parts
    if (count != 4 || ip.back() == '.') return false;

    return true;
}

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

void DrawHoles() {
    for (int j = 0; j < ROWS; j++) {
        for (int i = 0; i < COLS; i++) {
            Vector2 pos = CellToScreen(j, i);
            DrawCircle(pos.x, pos.y, hole_radius, BLACK);
        }
    }
}

void DrawPreview(Vector2 pos, Game game) {
    for (int i = 0; i < COLS; i++) {
        if (CheckCollisionPointRec(pos, GetColumnRect(i)) && game.heights[i] >= 0) {
            Vector2 pos = CellToScreen(game.heights[i], i);
            DrawCircle(pos.x, pos.y, hole_radius, (game.turn == 1) ? semi_red : semi_blue);
        }
    }   
}

void DrawChips(Game game) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            Vector2 pos = CellToScreen(i, j);
            if (game.board[i][j] == 1) {
                DrawCircle(pos.x, pos.y, 50, red);
            } else if (game.board[i][j] == 2) {
                DrawCircle(pos.x, pos.y, 50, blue);
            }
        }
    }
}
