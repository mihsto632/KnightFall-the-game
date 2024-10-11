#include "Horse.cpp"
#include<iostream>
//#include<cmath>
#include<cstring>
//Libraries that allow the use of unicode
#include <locale>
#include <codecvt>
//Libraries that allow exception handling methods outOfRangeExcetion and invalid_argumet
#include <stdexcept>
#include <limits>

using namespace std;

int main(){

    // Set locale to support Unicode
    locale::global(locale("en_US.UTF-8")); // Set locale for Unicode
    wcout.imbue(locale("en_US.UTF-8")); // Set wcout to use UTF-8

    //Initialize all board elements to ' '
    char board[8][8];
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            board[i][j] = ' ';

    //Seeding random number generator with the current time
    srand(static_cast<unsigned>(time(0)));

    //Initializing variables
    int difficulty{0}, initial_number_of_mines, add_mines, move_count{0}, max_moves, x_count{0}, number_of_rounds{0}, current_number_of_mines{0}, generate_remove_number{8}, max_mines_allowed{30};
    char current_position = 'H', target_position = 'X', mine_position = '*', letter_y, letter_x; //♞
    const char* filename;

    //Enter name logic that uses do-while to ensure user doesn't enter an invalid name
    char name[4];
    name[3] = '\0';
    do{
        wcout<<"Enter name: ";
        cin>>name;
        if (strlen(name) != 3){
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    while(strlen(name) != 3);

    //Initializing initial and target positions
    int initial_x{generateRandomNumber()};
    int initial_y{generateRandomNumber()};
    int target_x{generateRandomNumber()};
    int target_y{generateRandomNumber()};
    //RE-initializing target position if the first one was a move away from initial position
    while ((target_x == initial_x && target_y == initial_y) || move_valid(initial_x, initial_y, target_x, target_y)){
         target_x = generateRandomNumber();
         target_y = generateRandomNumber();
    }
    //Printing initial and target positions to the terminal
    wcout<<"\n\nInitial position: "<<initial_x<<' '<<initial_y<<'\n';
    wcout<<"Target position : "<<target_x<<' '<<target_y<<'\n';

    //Setting board spaces as 'H' and 'X'
    board[initial_x-1][initial_y-1] = current_position;
    board[target_x-1][target_y-1]   = target_position;
    wcout<<"\n\n\n";

    //Logic that determines default number of mines and number of mines to be added in each itteration based on difficulty
    wcout<<"Set difficulty (0 - Easy\t1 - Medium\t2 - Hard\t3 - Survival): ";
    do{
        cin>>difficulty;
    }
    while (difficulty<0 || difficulty>3);
    wcout<<"\n\n\n";

    //Setting up variables based on difficulty
    setup_variables(difficulty, number_of_rounds, initial_number_of_mines, add_mines, max_moves);

    //Updating current number of mines after adding the initial number
    current_number_of_mines = initial_number_of_mines;

    //Generating number of mines and outputing info about number of mines and moves, and drawing the initial board
    do{
        generate_initial_mines(initial_number_of_mines, board, current_position, target_position, mine_position, target_x, target_y);
    }
    while(!is_move_available(initial_x, initial_y, board));

    draw_board(board);

    //Defining filename logic based on difficulty
    if (difficulty < 3){
        wcout<<"Number of moves left: "<<max_moves<<"\n";
        if (difficulty == 1)
            filename = "horse_medium.txt";
        else if (difficulty == 2)
            filename = "horse_hard.txt";
        else
            filename = "horse_easy.txt";
    }
    else{
        filename = "horse_survival.txt";
        wcout<<"Initial number of mines: "<<initial_number_of_mines<<"\n\n\n";
    }

    //Printing the current scoreboard
    read_from_file(filename);

    //PROGRAM MAIN LOGIC

    //Clearing the previous knight position (square goes from 'H' to ' ')
    while(move_count <= max_moves){ //all program main logic inside this while condition
        board[initial_x-1][initial_y-1] = ' ';
        int next_x, next_y; //initializing the upcoming knight position
        int valid_counter{0}; //counter that exits the program once user enter 3 invalid moves in a row

        do {
            // If valid counter reaches 3, exit the program
            if (valid_counter == 3) {
                wcout << "You entered 3 invalid inputs in a row. You lose.\n";
                //In case of ending the program, write_to_file writes current player score to an appropriate file, and read_from_file should (CURRENTLY NOT WORKING) print same info to terminal
                write_to_file(name, x_count, move_count, filename);
                read_from_file(filename);
                return 0; // Exit the program after 3 invalid attempts
            }

            cin.clear();  // Clear the error flags caused by impropper coordinates
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n'); //ignoring previous things found in cin
            wcout << "Enter next valid move: ";
            cin >>letter_y>> letter_x; //Entering new values, that are treated as strings
            valid_counter++;

            //Converting string coordinate inputs to int values that the 2d array uses (ex: D3 becomes 5 4)
            next_y = letter_to_int_conversion_y(letter_y, next_y);
            next_x = letter_to_int_conversion_x(letter_x, next_x);
        }
        while (!move_valid(initial_x, initial_y, next_x, next_y) || ((letter_y<'a' || letter_y>'z') && (letter_y<'A' || letter_y>'Z')) || (letter_x<'1' || letter_x>'9'));

        //Resseting valid_counter after a succesfull new move
        valid_counter = 0;
        initial_x = next_x; //updating knight to new x coordinate
        initial_y = next_y; //updating knight to next y coordinate

        //Checks if player found X for easy difficulty
        if (difficulty == 0 && board[next_x-1][next_y-1] == target_position){
            board[next_x-1][next_y-1] = current_position; ////sets 'H' on new knight square
            draw_board(board);
            wcout<<"Congratulations! You found the target within "<<move_count+1<<" moves.You won!\n\n\n"; //if easy difficulty, first time player finds the flag is also last
            //wcout<<"Congratulations! You found the toilet within "<<move_count+1<<" moves.You won!\n\n\n";
            write_to_file(name, x_count, move_count, filename);
            read_from_file(filename);

            return 0;
        }

        //Checks if player found an X for all other difficulties
        if ((difficulty>0) && (board[next_x-1][next_y-1] == target_position)){
            x_count++;
            board[next_x-1][next_y-1] = current_position; //sets 'H' on new knight square
            if (difficulty < 3){ //in case of medium and hard difficulty
                wcout<<"Congratulations! You found target "<<x_count<<" of "<<number_of_rounds<<"\n";
                //wcout<<"Congratulations! You found toilet "<<x_count<<" of "<<number_of_rounds<<"\n";
            }
            else if (difficulty == 3){
                //Logic to ensure Survival mode doesn't go on forever
                if(x_count == 100){
                    wcout<<"Survival mode complete. You win!\n\n\n";
                    //wcout<<"You are officialy a king SHITHEAD!\n\n\n";
                    write_to_file(name, x_count, move_count, filename);
                    read_from_file(filename);
                    return 0;
                }
                //Continuing logic for Survival mode (difficulty ==3)
                //Every 5th time player finds an 'X', while there are already max number of mines allowed(30) on the board, there is 1 more mine generated
                if (current_number_of_mines >= max_mines_allowed && x_count > 9){
                    //Swaps 5 random mines by removing and generating the same number in random places
                    remove_mines(board, generate_remove_number, mine_position);
                    generate_initial_mines(generate_remove_number, board, current_position, target_position, mine_position, target_x, target_y);

                    //Generates 1 additional mine every 10 points
                    if (x_count % 10 == 0)
                        generate_initial_mines(1, board, current_position, target_position, mine_position, target_x, target_y);
                }

            }
            board[target_x-1][target_y-1] = current_position; //Resetting last X spot

            //If last X found (Won't do anything if difficulty is 3)
            if(x_count == number_of_rounds){
                //board[target_x-1][target_y-1] = current_position;
                draw_board(board);
                wcout<<"You won the game! Target position found within "<<move_count+1<<" moves\n\n\n";
                //wcout<<"You found the toilet within "<<move_count+1<<" moves.\n\n\n";
                write_to_file(name, x_count, move_count, filename);
                read_from_file(filename);
                return 0;
            }

            //Always done if player hasn't found the last flag
            //Entering a new valid target position
            int tmp_x = target_x;
            int tmp_y = target_y;

            //Repeats the do block if: 1.there is 1 move between old and new 'X', there isn't 1 move that can lead to the new 'X', 3. and 4. new 'X' doesnt fall on '*' or 'H'
            do{
                target_x = generateRandomNumber();
                target_y = generateRandomNumber();
            }
            while(move_valid(target_x, target_y, tmp_x, tmp_y) || !is_move_available(target_x-1, target_y-1, board) || board[target_x-1][target_y-1] == '*' || board[target_x-1][target_y-1]=='H');// || move_valid(tmp_x, tmp_y, target_x, target_y));
                board[target_x-1][target_y-1] = target_position;
                draw_board(board);
        }

        //ENDGAME LOGIC
        //STEPPED ON A MINE
        else if (board[next_x-1][next_y-1] == mine_position){
            board[next_x-1][next_y-1] = current_position;
            draw_board(board);
            if (difficulty < 3){
                wcout<<"You stepped on a mine. Game over.\n\n\n";
                //wcout<<"Game over. You have explosive diarrhea.\n\n\n";

            }
            else
                wcout<<"You stepped on a mine. Final score is: "<<x_count<<".\n";
                //wcout<<"Explosive diarrhea  beat you! Final score is: "<<x_count<<".\n\n\n";
            write_to_file(name, x_count, move_count, filename);
            read_from_file(filename);
            return 0;
        }

        //Updating current position before the next iteration, drawing the board and generating more mines before next turn
        board[next_x-1][next_y-1] = current_position;

        //Add more mines before the next iteration
        //In case of Survival mode, max mines should be 30
        if (difficulty == 3) //Survival mode
            generate_additional_mines(add_mines, board, current_position, target_position, mine_position, current_number_of_mines, target_x, target_y, max_mines_allowed);
        else
            //In case of normal modes, max mines goes up to 47
            generate_additional_mines(add_mines, board, current_position, target_position, mine_position, current_number_of_mines, target_x, target_y, 47);
        draw_board(board);

        //Checks if the knight can move
        if (!is_move_available(next_x-1, next_y-1, board)){
            board[next_x-1][next_y-1] = current_position;
            wcout<<"Knight is trapped. You lost.\n\n\n";
            //wcout<<"You are so constipated, you can't move. You lost.\n\n\n";
            write_to_file(name, x_count, move_count, filename);
            read_from_file(filename);
            return 0;
        };

        //Checks if it's still possible to reach the destination
        if (difficulty == 3 && !is_move_available(target_x-1, target_y-1, board)){
            wcout<<"Impossible to reach the destination. You lost.\n";
            //wcout<<"Toilet is too far away. You lost.\n";
            wcout<<"Final score is: "<<x_count<<".\n\n\n";
            write_to_file(name, x_count, move_count, filename);
            read_from_file(filename);
            return 0;
        }
        else if (!is_move_available(target_x-1, target_y-1, board)){
            wcout<<"Impossible to reach destination. You lost.\n\n\n";
            //wcout<<"Toilet is too far away. You lost.\n";
            write_to_file(name, x_count, move_count, filename);
            read_from_file(filename);
            return 0;
        }

        //Always performed
        move_count++;

        if (difficulty<3){
            wcout<<"Moves remaining: "<<max_moves-move_count<<"\n\n\n";
        }
        else
            wcout<<"Current score: "<<x_count<<'\n';

        //MAX MOVES REACHED
        if (move_count == max_moves){
            draw_board(board);
            wcout<<"Max moves reached. You lost.\n\n\n";
            //wcout<<"You used all your moves, but the toilet remains too far away.\nPro tip: get some newspapers.\n\n\n";
            write_to_file(name, x_count, move_count, filename);
            read_from_file(filename);
            return 0;
        }
    }

    return 0;
}
