#include "ProgramController.hpp"

ProgramController::ProgramController() {
    model = std::make_shared<ProgramModel>();
    model->init();
}

void ProgramController::init() {
    view = std::make_shared<ProgramView>();
    load();
}

void ProgramController::load() {
    std::string filepath;
    view->display_loader();
    filepath = view->get_user_input();
    try 
    {
        load_board(filepath);
    } 
    catch (const ConfigFileException& e) 
    {
        std::cerr << "Error loading board: " << e.what() << std::endl;
        exit();
    }
}

void ProgramController::main_menu() {
    view->display_welcome_message();
    view->display_main_menu(model->get_initial_board());
    std::string choice;
    while (true) {
        choice = view->get_user_input();
        if (view->normalizeString(choice) == view->normalizeString("Load Board") || choice == "1") {
            load();
            break;
        } else if (view->normalizeString(choice) == view->normalizeString("Run Solver") 
        || view->normalizeString(choice) == view->normalizeString("Solver") 
        ||  choice == "2") {    
            solver();
            break;
        } else if (view->normalizeString(choice) == view->normalizeString("Exit") 
        || choice == "3" 
        || view->normalizeString(choice) == view->normalizeString("Quit")) {
            exit();
            break;
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}

void ProgramController::exit() {
    std::cout << "Exiting the program." << std::endl;
    std::exit(0);
}

void ProgramController::solver() {
    view->display_algorithms();
    std::string temp;
    std::string algorithm;
    while(true) {
        temp = view->get_user_input();
        algorithm = view->normalizeString(temp);
        if (algorithm == view->normalizeString("Exit") || algorithm == view->normalizeString("Quit")) exit();
        view->display_heuristics();
        temp = view->get_user_input();
        std::string heuristics = view->normalizeString(temp);
        while (heuristics != "blockedcarheuristic" && heuristics != "manhattandistance") {
            std::cout << "Invalid choice. Please try again." << std::endl;
            temp = view->get_user_input();
            heuristics = view->normalizeString(temp);
        }
        try 
        {
            run_solver(algorithm);
            break;
        } 
        catch (const std::exception& e) 
        {
            std::cerr << "Error running solver: " << e.what() << "|| Type 'Exit' to exit" << std::endl;
        }
    }
    
}

void ProgramController::load_board(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }

    try 
    {
        auto [rows, cols] = Reader::read_dimensions(file);
        int secondary_count = Reader::read_secondary_count(file);
        std::vector<std::string> board_config = Reader::read_board_config(file, rows, cols);
        model->create_board(rows, cols, secondary_count, board_config);
    }
    catch (const ConfigFileException& e) 
    {
        throw;
    } 
    catch (const std::exception& e) 
    {
        throw std::runtime_error("Error reading board configuration: " + std::string(e.what()));
    }
    file.close();
}

void ProgramController::run_solver(const std::string& algorithm, const std::string& heuristics) {
    Algorithms::Result sol = {{}, 0, 0, 0.0};
    try 
    {
        std::cout << "Running solver..." << std::endl;
        sol = model->run_solver(algorithm);
    } 
    catch (const std::exception& e) 
    {
        throw std::runtime_error(std::string(e.what()));
    }
    if (sol.path.empty()) {
        std::cout << "No solution found." << std::endl;
        view->pause();
    } else {
        view->display_solution(sol);
    }
}
