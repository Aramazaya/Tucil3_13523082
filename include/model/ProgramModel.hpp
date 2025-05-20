#pragma once
#include <memory>
#include "model/Algorithms.hpp"
#include "model/Board.hpp"
#include "model/Reader.hpp"

class ProgramModel {
private:
    Board initial_board;
    std::map<std::string, std::unique_ptr<Algorithms>> algorithms;
public:
    ProgramModel();
    void init();
    void create_board(int rows, int cols, int sec_count, std::vector<std::string> board_config);
    void run_solver(const std::string& algorithm);
};