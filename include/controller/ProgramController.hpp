#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <map>
#include "model/ProgramModel.hpp"
#include "view/ProgramView.hpp"

class ProgramController {
private:
    std::shared_ptr<ProgramModel> model;
    std::shared_ptr<ProgramView> view;
public:
    ProgramController();
    void init();
    void main_menu();
    void load();
    void solver();
    void exit();
    void load_board(const std::string& filepath);
    void run_solver(const std::string& algorithm, const std::string& heuristics = "blockedcarheuristic");
};