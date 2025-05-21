#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <chrono>
#include <stack>
#include <thread>
#include "Board.hpp"

class AlgorithmsException : public std::exception {
private:
    std::string message;
public:
    AlgorithmsException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class Steps {
private:
    std::map<std::string, Position> pieces;
    Board board;
public:
    Steps() : board() {}
    Steps(const Board& board);
    Steps(const Steps& other);
    std::map<std::string, Position> get_pieces() const { return pieces; }
    const Board& get_board() const { return board; }
    Steps& operator=(const Steps& other);
    bool operator==(const Steps& other) const;
    bool operator!=(const Steps& other) const { return !(*this == other);}
    std::vector<Steps> get_possible_steps() const;
    size_t hash() const;
    bool is_goal() const;
    std::string to_string() const;
};

class Algorithms {
protected:
    template<typename T>
    struct PQCompare {
        bool operator()(const std::pair<T, Steps>& a, const std::pair<T, Steps>& b) const {
            return a.first > b.first;
        }
    };
    static std::vector<Steps> get_path(std::unordered_map<Steps, Steps>& from, Steps& current, Steps& start);
    std::string heuristics;
    static double manhattan_distance(const Steps& state);
    static double blocked_car_heuristic(const Steps& state);
public:
    Algorithms(std::string heuristics = "") : heuristics(heuristics) {};
    virtual ~Algorithms() = default;
    void set_heuristics(const std::string& heuristics) { this->heuristics = heuristics; }
    double heuristic(const Steps& state) const {
        if (heuristics == "blockedcarheuristic") {
            return blocked_car_heuristic(state);
        } else if (heuristics == "manhattandistance") {
            return manhattan_distance(state);
        }
    }
    struct Result {
        std::vector<Steps> path;
        int move_count;
        int states_explored;
        double time_taken;
    };
    virtual Result solve(const Board& board) = 0;
};

class AStar : public Algorithms {
public:
    AStar() = default;
    Result solve(const Board& board) override;
};

class GBFS : public Algorithms {
public:
    GBFS() = default;
    Result solve(const Board& board) override;
};

class UCS : public Algorithms {
public:
    UCS() = default;
    Result solve(const Board& board) override;
};

class IDA : public Algorithms {
public:
    IDA() = default;
    Result solve(const Board& board) override;
};