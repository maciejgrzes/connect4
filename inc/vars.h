#pragma once

#include <raylib.h>


const Color semi_red = {255, 0, 0, 64};
const Color semi_green = {0, 255, 0, 64};

constexpr int WIDTH = 1400;
constexpr int HEIGHT = 800;

constexpr int B_WIDTH = 780;
constexpr int B_HEIGHT = 670;

constexpr int ROWS = 6;
constexpr int COLS = 7;


constexpr int board_top_left_x = (WIDTH / 2) - (B_WIDTH / 2);
constexpr int board_top_left_y = (HEIGHT / 2) - (B_HEIGHT / 2);


constexpr int board_top_left_x_for_holes = board_top_left_x + 60;
constexpr int board_top_left_y_for_holes = board_top_left_y + 60;


constexpr int hole_radius = 50;
constexpr int hole_diameter = 2 * hole_radius;
constexpr int gap_between_holes = 10;

const Rectangle BoardBackground = {(WIDTH / 2) - (B_WIDTH / 2), (HEIGHT / 2) - (B_HEIGHT / 2), B_WIDTH, B_HEIGHT};
