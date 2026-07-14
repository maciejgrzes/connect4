#pragma once

#include <raylib.h>


const Color semi_red = {255, 36, 00, 100};
const Color semi_blue = {55, 55, 255, 100}; 

const Color red =  {255, 36, 00, 255};
const Color blue = {55, 55, 255, 255}; 

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

const Rectangle BoardBackground = {((float)WIDTH / 2) - ((float)B_WIDTH / 2), ((float)HEIGHT / 2) - ((float)B_HEIGHT / 2), B_WIDTH, B_HEIGHT};
