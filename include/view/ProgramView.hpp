#include <iostream>
#include <string>
#include <vector>
#include "Board.hpp"
#include "Algorithms.hpp"

class ProgramView {
public:
    void display_board(const Board& board);
    void display_solution(const std::vector<Steps>& solution);
    std::string get_algorithm_choice();
    std::string get_file_path();
    void display_main_menu();
};