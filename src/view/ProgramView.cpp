#include "ProgramView.hpp"

void ProgramView::display_welcome_message() {
    std::string welcome_message = R"(
░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓███████▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓███████▓▒░  
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓████████▓▒░      ░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓███████▓▒░  
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░ 
                                                                                                                 
                                                                                                                 
 ░▒▓███████▓▒░░▒▓██████▓▒░░▒▓█▓▒░   ░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓███████▓▒░                                      
░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░   ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░                                     
░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░    ░▒▓█▓▒▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░                                     
 ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░    ░▒▓█▓▒▒▓█▓▒░░▒▓██████▓▒░ ░▒▓███████▓▒░                                      
       ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░     ░▒▓█▓▓█▓▒░ ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░                                     
       ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░     ░▒▓█▓▓█▓▒░ ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░                                     
░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓████████▓▒░▒▓██▓▒░  ░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░                                     
                                                                                                                 
)";
    std::cout << welcome_message << std::endl;
}

void ProgramView::display_board(const Board& board) {
    std::cout << board.to_string() << std::endl;
}

void ProgramView::display_solution(const Algorithms::Result& solution) {
    std::string solution_message = R"(
______ _____ _____ _   _ _    _____ 
| ___ \  ___/  ___| | | | |  |_   _|
| |_/ / |__ \ `--.| | | | |    | |  
|    /|  __| `--. \ | | | |    | |  
| |\ \| |___/\__/ / |_| | |____| |  
\_| \_\____/\____/ \___/\_____/\_/  
====================================
)";
    std::cout << solution_message << std::endl;
    std::cout << "Solution found in " << solution.move_count << std::endl << "Moves." << std::endl;
    std::cout << "States explored: " << solution.states_explored << std::endl;
    std::cout << "Time taken: " << solution.time_taken << " seconds." << std::endl;
    std::cout << "Initial state:" << std::endl;
    std::cout << solution.path[0].to_string() << std::endl;
    pause();
    for (int i = 1; i < solution.path.size(); ++i) {
        std::cout << solution_message << std::endl;
        std::cout << "Solution found in " << solution.move_count << std::endl << "Moves." << std::endl;
        std::cout << "States explored: " << solution.states_explored << std::endl;
        std::cout << "Time taken: " << solution.time_taken << " seconds." << std::endl;
        std::cout << "Move " << i << std::endl;
        std::cout << solution.path[i].to_string() << std::endl;
        pause();
    }
}
void ProgramView::pause() {
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::system("clear");
}

void ProgramView::display_main_menu(const Board& initial_board) {
    std::system("clear");
    display_board(initial_board);
    std::cout << "====================================" << std::endl;
    std::cout << "Main Menu" << std::endl;
    std::cout << "[Load Board]" << std::endl;
    std::cout << "[Run Solver]" << std::endl;
    std::cout << "[Exit]" << std::endl;
}

void ProgramView::display_heuristics(){
    std::cout << "Available Heuristics:" << std::endl;
    std::cout << "1. Manhattan Distance" << std::endl;
    std::cout << "2. Blocked Car Heuristic" << std::endl;
}

std::string ProgramView::get_user_input() {
    std::string input;
    std::cout << "> ";
    std::getline(std::cin, input);
    return input;
}

void ProgramView::display_algorithms() {
    std::system("clear");
    std::string algorithm_message = R"(
  ___   _     _____ ___________ _____ _____ _   _ ___  ___ _____ 
 / _ \ | |   |  __ \  _  | ___ \_   _|_   _| | | ||  \/  |/  ___|
/ /_\ \| |   | |  \/ | | | |_/ / | |   | | | |_| || .  . |\ `--. 
|  _  || |   | | __| | | |    /  | |   | | |  _  || |\/| | `--. \
| | | || |___| |_\ \ \_/ / |\ \ _| |_  | | | | | || |  | |/\__/ /
\_| |_/\_____/\____/\___/\_| \_|\___/  \_/ \_| |_/\_|  |_/\____/ 
=================================================================
)";
    std::cout << algorithm_message << std::endl;
    std::cout << "Available Algorithms:" << std::endl;
    std::cout << "1. A*" << std::endl;
    std::cout << "2. Greedy Best First Search" << std::endl;
    std::cout << "3. Uniform Cost Search" << std::endl;
    std::cout << "4. Iterative Deepening A*" << std::endl;
}
void ProgramView::display_loader() {
    std::system("clear");
    std::string loading_message = R"(
 _     _____  ___ ______ ___________ 
| |   |  _  |/ _ \|  _  \  ___| ___ \
| |   | | | / /_\ \ | | | |__ | |_/ /
| |   | | | |  _  | | | |  __||    / 
| |___\ \_/ / | | | |/ /| |___| |\ \ 
\_____/\___/\_| |_/___/ \____/\_| \_|
=====================================
)";
    std::cout << loading_message << std::endl;
    std::cout << "Input Filepath (True Path): ";
}

std::string ProgramView::normalizeString(const std::string& input) {
    std::string result;
    
    // Reserve space to avoid reallocations
    result.reserve(input.length());
    
    // Copy non-whitespace characters to result, converting to lowercase
    for (char c : input) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            result.push_back(std::tolower(static_cast<unsigned char>(c)));
        }
    }
    
    return result;
}