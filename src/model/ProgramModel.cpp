#pragma once
#include "model/A-star.hpp"
//#include "model/Dijkstra.hpp"
//#include "model/Graph.hpp"
#include "model/Board.hpp"

class ProgramModel {
private:
    Board board;
    std::string filepath;
    std::map<std::string, std::unique_ptr<Algorithm>> algorithms;
public:
    ProgramModel(std::string filepath)
    void run_A_Star();


};