#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "Board.hpp"
#include "Algorithms.hpp"

class ProgramView {
public:
    ProgramView()= default;
    void display_welcome_message();
    void display_board(const Board& board);
    void display_solution(const Algorithms::Result& solution);
    std::string get_user_input();
    void display_algorithms();
    void display_heuristics();
    void display_loader();
    void display_main_menu(const Board& initial_board);
    void pause();
    std::string normalizeString(const std::string& input);
};