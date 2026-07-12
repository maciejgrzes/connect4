#include <iostream>
#include <raylib.h>

#ifndef VARS
#define VARS

const int WIDTH = 1400;
const int HEIGHT = 800;

const int B_WIDTH = 780;
const int B_HEIGHT = 670;

constexpr int ROWS = 6;
constexpr int COLS = 7;


const int board_top_left_x = (WIDTH / 2) - (B_WIDTH / 2);
const int board_top_left_y = (HEIGHT / 2) - (B_HEIGHT / 2);


const int board_top_left_x_for_holes = (WIDTH / 2) - (B_WIDTH / 2) + 60;
const int board_top_left_y_for_holes = (HEIGHT / 2) - (B_HEIGHT / 2) + 60;


const int hole_radius = 50;
const int hole_diameter = 2 * hole_radius;
const int gap_between_holes = 10;


const int column_one_start = board_top_left_x; 
const int column_one_end = column_one_start + hole_diameter + gap_between_holes + (gap_between_holes / 2);

const int column_two_start = column_one_end; 
const int column_two_end = column_two_start + hole_diameter + gap_between_holes;

const int column_three_start = column_two_end; 
const int column_three_end = column_three_start + hole_diameter + gap_between_holes;

const int column_four_start = column_three_end; 
const int column_four_end = column_four_start + hole_diameter + gap_between_holes;

const int column_five_start = column_four_end; 
const int column_five_end = column_five_start + hole_diameter + gap_between_holes;

const int column_six_start = column_five_end; 
const int column_six_end = column_six_start + hole_diameter + gap_between_holes;

const int column_seven_start = column_six_end; 
const int column_seven_end = column_seven_start + hole_diameter + gap_between_holes + (gap_between_holes / 2);


const Rectangle BoardBackground = {(WIDTH / 2) - (B_WIDTH / 2), (HEIGHT / 2) - (B_HEIGHT / 2), B_WIDTH, B_HEIGHT};


const Rectangle column_one = {column_one_start, board_top_left_y, 
                              hole_diameter + gap_between_holes + (gap_between_holes / 2),
                              B_HEIGHT};

const Rectangle column_two = {column_two_start, board_top_left_y, 
                              hole_diameter + gap_between_holes,
                              B_HEIGHT};

const Rectangle column_three = {column_three_start, board_top_left_y, 
                              hole_diameter + gap_between_holes,
                              B_HEIGHT};

const Rectangle column_four = {column_four_start, board_top_left_y, 
                              hole_diameter + gap_between_holes,
                              B_HEIGHT};

const Rectangle column_five = {column_five_start, board_top_left_y, 
                              hole_diameter + gap_between_holes,
                              B_HEIGHT};

const Rectangle column_six = {column_six_start, board_top_left_y, 
                              hole_diameter + gap_between_holes,
                              B_HEIGHT};

const Rectangle column_seven = {column_seven_start, board_top_left_y, 
                              hole_diameter + gap_between_holes + (gap_between_holes / 2),
                              B_HEIGHT};

#endif
