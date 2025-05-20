#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include "model/Board.hpp"
#include "model/Piece.hpp"


class ConfigFileException : public std::exception {
private:
    std::string message;
public:
    ConfigFileException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidDimensionsException : public ConfigFileException {
public:
    InvalidDimensionsException(const std::string& msg) : ConfigFileException(msg) {}
};

class InvalidPieceCountException : public ConfigFileException {
public:
    InvalidPieceCountException(const std::string& msg) : ConfigFileException(msg) {}
};

class InvalidBoardSizeException : public ConfigFileException {
public:
    InvalidBoardSizeException(const std::string& msg) : ConfigFileException(msg) {}
};

class InvalidBoardConfigException : public ConfigFileException {
public:
    InvalidBoardConfigException(const std::string& msg) : ConfigFileException(msg) {}
};

class MissingPrimaryCarException : public ConfigFileException {
public:
    MissingPrimaryCarException(const std::string& msg) : ConfigFileException(msg) {}
};

class InvalidGoalCellException : public ConfigFileException {
public:
    InvalidGoalCellException(const std::string& msg) : ConfigFileException(msg) {}
};

class Reader {
public:
    static bool is_contiguous_piece(const std::vector<Position>& positions);
    static Orientation determine_orientation(const std::vector<Position>& positions);
    static Position find_head(const std::vector<Position>& positions, Orientation orientation);
    static std::pair<int, int> read_dimensions(std::ifstream& file);
    static int read_secondary_count(std::ifstream& file);
    static std::vector<std::string> read_board_config(std::ifstream& file, int rows, int cols);
};