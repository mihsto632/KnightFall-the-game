#include "Horse.h"
#include<iostream>
#include<random>
using namespace std;

//Function that draws a board
void draw_board(char (*board)[8]){
    wcout << "\033[2J\033[H\n";
    wcout<<" ___ ___ ___ ___ ___ ___ ___ ___\n";
    int count_rows{8};
    for (int i=0; i<8; i++){
        wcout << "| ";
        for (int j=0; j<8; j++){
            if (board[i][j] == 'H'){
                wcout << L'\u265E'; //♞
                //wcout<<L"\U0001F4A9"; //💩
                wcout<<" | ";
            }
            else if(board[i][j] == 'X'){
                wcout<<L"\U0001F6A9"; //🚩
                //wcout<<L"\U0001F6BD"; //🚽
                //wcout << L'\u274C'; //❌
                wcout<<"| ";
            }

            else if(board[i][j] == '*'){
                wcout<<L'\U0001F4A3'; //💣
                //wcout << L'\u1F4A'; //alt bomb
                wcout<<"| ";
            }
            else{
                wcout<<(board[i][j])<< " | ";
            }
        }
        wcout<<"  "<<count_rows;
        count_rows --;
        wcout<<"\n|___|___|___|___|___|___|___|___|\n";
    }
        wcout << "  A   B   C   D   E   F   G   H\n";
}

//Checks if the move is valid between 2 points
bool move_valid(int initial_x, int initial_y, int next_x, int next_y){
    if        ((next_x == initial_x+2 && next_y == initial_y+1)
            || (next_x == initial_x+2 && next_y == initial_y-1)
            || (next_x == initial_x-2 && next_y == initial_y+1)
            || (next_x == initial_x-2 && next_y == initial_y-1)
            || (next_x == initial_x+1 && next_y == initial_y+2)
            || (next_x == initial_x+1 && next_y == initial_y-2)
            || (next_x == initial_x-1 && next_y == initial_y+2)
            || (next_x == initial_x-1 && next_y == initial_y-2))
        return true;
    else
        return false;
}

void generate_initial_mines(int initial_number_of_mines, char (*board)[8], char current_position, char target_position, char mine_position, int& target_x, int& target_y){
    int x{0}, y{0};
    for (int i=0; i<initial_number_of_mines; i++){
        do{
            x = generateRandomNumber();
            y = generateRandomNumber();
        }
        while ((board[x-1][y-1] == current_position || board[x-1][y-1] == target_position || board[x-1][y-1] == mine_position || move_valid(x, y, target_x, target_y)));

        board [x-1][y-1] = mine_position;
    }
}

void generate_additional_mines(int number_of_mines, char (*board)[8], char current_position, char target_position, char mine_position, int& current_number_of_mines, int& target_x, int& target_y, int max_mines_allowed){
    int x{0}, y{0};
    for (int i=0; i<number_of_mines; i++){
        if (current_number_of_mines >= max_mines_allowed)
            return;
        do{
            x = generateRandomNumber();
            y = generateRandomNumber();
        }
        while ((board[x-1][y-1] == current_position || board[x-1][y-1] == target_position || board[x-1][y-1] == mine_position || move_valid(x, y, target_x, target_y)));
        board [x-1][y-1] = mine_position;
        current_number_of_mines++;
    }
}

//Function that checks if any of possible knight moves from a point (target_x,target_y) are valid and not blocked by '*' or 'H'
bool is_move_available(int target_x, int target_y, char (*board)[8]){
    if((target_x+1<8 && target_x+1>=0) && (target_y+2<8 && target_y+2>=0) && board[target_x+1][target_y+2] != '*')
        return true;
    if((target_x+1<8 && target_x+1>=0) && (target_y-2<8 && target_y-2>=0) && board[target_x+1][target_y-2] != '*')
        return true;
    if((target_x-1<8 && target_x-1>=0) && (target_y+2<8 && target_y+2>=0) && board[target_x-1][target_y+2] != '*')
        return true;
    if((target_x-1<8 && target_x-1>=0) && (target_y-2<8 && target_y-2>=0) && board[target_x-1][target_y-2] != '*')
        return true;
    if((target_x+2<8 && target_x+2>=0) && (target_y+1<8 && target_y+1>=0) && board[target_x+2][target_y+1] != '*')
        return true;
    if((target_x+2<8 && target_x+2>=0) && (target_y-1<8 && target_y-1>=0) && board[target_x+2][target_y-1] != '*')
        return true;
    if((target_x-2<8 && target_x-2>=0) && (target_y+1<8 && target_y+1>=0) && board[target_x-2][target_y+1] != '*')
        return true;
    if((target_x-2<8 && target_x-2>=0) && (target_y-1<8 && target_y-1>=0) && board[target_x-2][target_y-1] != '*')
        return true;

    // If no valid move is found
    return false;
}

int letter_to_int_conversion_y(int coordinate_y_letter, int coordinate_y_number){
    switch(coordinate_y_letter){
        case 'A':
        case 'a':
            coordinate_y_number = 1;
            break;
        case 'B':
        case 'b':
            coordinate_y_number = 2;
            break;
        case 'C':
        case 'c':
            coordinate_y_number = 3;
            break;
        case 'D':
        case 'd':
            coordinate_y_number = 4;
            break;
        case 'E':
        case 'e':
            coordinate_y_number = 5;
            break;
        case 'F':
        case 'f':
            coordinate_y_number = 6;
            break;
        case 'G':
        case 'g':
            coordinate_y_number = 7;
            break;
        case 'H':
        case 'h':
            coordinate_y_number = 8;
            break;
        default:
            coordinate_y_number = 0;
            break;
    }
    return coordinate_y_number;
}

//Function that does conversion of X value 1-8 to X value 8-1 (to achieve proper square numeration in chess)
int letter_to_int_conversion_x(int coordinate_x_letter, int coordinate_x_number){
    switch (coordinate_x_letter){
        case '1':
            coordinate_x_number = 8;
            break;
        case '2':
            coordinate_x_number = 7;
            break;
        case '3':
            coordinate_x_number = 6;
            break;
        case '4':
            coordinate_x_number = 5;
            break;
        case '5':
            coordinate_x_number = 4;
            break;
        case '6':
            coordinate_x_number = 3;
            break;
        case '7':
            coordinate_x_number = 2;
            break;
        case '8':
            coordinate_x_number = 1;
            break;
        default:
            coordinate_x_number = 0;
            break;
    }
    return coordinate_x_number;
}

//Function that takes the difficulty and based on that makes setups of variables
void setup_variables(int& difficulty, int& number_of_rounds, int& initial_number_of_mines, int& add_mines, int& max_moves){
    switch (difficulty){
        case 0: //EASY DIFFICULTY VARIABLE SETUP
            initial_number_of_mines = 10;
            add_mines = 1;
            max_moves = 10;
            break;
        case 1: //MEDIUM DIFFICULTY VARIABLE SETUP
            number_of_rounds = 2;
            initial_number_of_mines = 12;
            add_mines = 2;
            max_moves = 12;
            break;
        case 2: //HARD DIFFICULTY VARIABLE SETUP
            number_of_rounds = 3;
            initial_number_of_mines = 15;
            add_mines = 3;
            max_moves = 14;
            break;
        case 3: //SURVIVAL DIFFICULTY VARIABLE SETUP
            number_of_rounds = 100;
            initial_number_of_mines = 5;
            add_mines = 1;
            max_moves = 10000;
            break;
        default:
            break;
    }
}

//Function that removes a set number of random mines
void remove_mines(char (*board)[8], int remove_number, char mine_position){
    int x, y;
    for (int i=0; i<remove_number; i++){
        do{
            x = generateRandomNumber();
            y = generateRandomNumber();
        }
        while (board[x-1][y-1] != mine_position);
        //while (board[x-1][y-1] == 'H' || board[x-1][y-1] == 'X');
        board[x-1][y-1] = ' ';
    }
}

//Function that writes to a file
void write_to_file(char name[4], int x_count, int move_count, const char* filename){
    ofstream file(filename, ios::app);

    //Check if file is open
    if (!file){
        cerr<<"Error: Could not open the file for writing.\n";
        return;
    }

    //Write data to the file
    file<<"Player: "<<name<<"||\tScore: "<<x_count<<"||\t||Move count: "<<move_count<<'\n';
    file.close();
}

//Function that reads from a file
/*FOLLOWING FUNCTION NOT WORKING CORRECTLY, NEEDS TO BE FIXED*/
void read_from_file(const char* filename){
//void read_from_file(char name[], int x_count, int move_count, const char* filename){
    ifstream file(filename);

    //Checking if file is open for reading
    if (!file) {
    cerr << "Error: Could not open the file for reading.\n";
    return;
    }
    char line[1000];
    //Read data from file
    while (file.getline(line, 1000)){
        cout<<line<<'\n';
    }
    file.close();
}
