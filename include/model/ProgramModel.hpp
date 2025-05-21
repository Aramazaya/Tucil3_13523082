#pragma once
#include <memory>
#include "model/Algorithms.hpp"
#include "model/Board.hpp"
#include "model/Reader.hpp"

class ProgramModel {
private:
    Board initial_board;
    std::map<std::string, std::shared_ptr<Algorithms>> algorithms;
public:
    ProgramModel();
    void init();
    void create_board(int rows, int cols, int sec_count, std::vector<std::string> board_config);
    Board get_initial_board() const { return initial_board; }
    Algorithms::Result run_solver(const std::string& algorithm, const std::string& heuristics = "blockedcarheuristic");
};