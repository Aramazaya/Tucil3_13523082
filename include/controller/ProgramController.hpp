#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <map>
#include "model/ProgramModel.hpp"

class ProgramController {
private:
    std::unique_ptr<ProgramModel> model;
    std::string algorithm;
    
    void init();
    void cleanup();
public:
    ProgramController();
    ~ProgramController();

    void run();
    void load();
    void save();
    void run_Algorithm();
    void set_Algorithm(const std::string& algorithm);
};