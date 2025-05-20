#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <map>
#include "model/ProgramModel.hpp"
#include "view/ProgramView.hpp"

class ProgramController {
private:
    std::unique_ptr<ProgramModel> model;
    std::unique_ptr<ProgramView> view;
public:
    ProgramController();
    void load_board(const std::string& filepath);
    void run_solver(const std::string& algorithm);
};