#include "ProgramController.hpp"

ProgramController::ProgramController() {
    model = std::make_unique<ProgramModel>();
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

void ProgramController::run_solver(const std::string& algorithm) {
    try 
    {
        model->run_solver(algorithm);
    } 
    catch (const std::exception& e) 
    {
        throw std::runtime_error("Error running solver: " + std::string(e.what()));
    }
    
}
