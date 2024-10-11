#ifndef HORSE_H
#define HORSE_H

#include<iostream>
#include<ctime>
#include<cstring>
#include<fstream>
using namespace std;

//Function that draws a board
void draw_board(char (*board)[8]);

// Function to generate a random number between 1 and 8
int generateRandomNumber() {
    return rand() % 8 + 1;
}

//Checks if the move is valid
bool move_valid(int initial_x, int initial_y, int next_x, int next_y);

//Function that adds a number of mines depending on difficulty
void generate_initial_mines(int initial_number_of_mines, char (*board)[8], char current_position, char target_position, char mine_position, int& target_x, int& target_y);
void generate_additional_mines(int number_of_mines, char (*board)[8], char current_position, char target_position, char mine_position, int& current_number_of_mines, int& target_x, int& target_y, int max_mines_allowed);

//Function that determines whether it is possible to jump to X
bool is_move_available(int next_x, int next_y, char (*board)[8]);

//Functions that do conversions between a char that user puts in into proper position on the board
int letter_to_int_conversion_y(int coordinate_y_letter, int coordinate_y_number);
int letter_to_int_conversion_x(int coordinate_x_letter, int coordinate_x_number);

//Function that takes the difficulty and based on that makes setups of variables
void setup_variables(int& difficulty, int& number_of_rounds, int& initial_number_of_mines, int& add_mines, int& max_moves);

//Function that removes a number of mines from the board
void remove_mines(char (*board)[8], int remove_number, char mine_position);

//Function that writes to a file
void write_to_file(char name[4], int x_count, int move_count, const char* filename);

//Function that reads from a file
void read_from_file(const char* filename);

#endif
