#include "model/Reader.hpp"


bool Reader::is_contiguous_piece(const std::vector<Position>& positions) {
    if (positions.size() <= 1) {
        return true;
    }
    bool isHorizontal = true;
    bool isVertical = true;
    int row = positions[0].row;
    int col = positions[0].col;
    for (size_t i = 1; i < positions.size(); i++) {
        if (positions[i].row != row) {
            isHorizontal = false;
        }
        if (positions[i].col != col) {
            isVertical = false;
        }
    }
    if (!isHorizontal && !isVertical) {
        return false;
    }
    if (isHorizontal) {
        std::vector<Position> sortedPos = positions;
        std::sort(sortedPos.begin(), sortedPos.end(), 
                 [](const Position& a, const Position& b) { return a.col < b.col; });
        for (size_t i = 1; i < sortedPos.size(); i++) {
            if (sortedPos[i].col != sortedPos[i-1].col + 1) {
                return false;
            }
        }
    } else {
        std::vector<Position> sortedPos = positions;
        std::sort(sortedPos.begin(), sortedPos.end(), 
                 [](const Position& a, const Position& b) { return a.row < b.row; });
        
        for (size_t i = 1; i < sortedPos.size(); i++) {
            if (sortedPos[i].row != sortedPos[i-1].row + 1) {
                return false;
            }
        }
    }
    return true;
}

Orientation Reader::determine_orientation(const std::vector<Position>& positions) {
    if (positions.size() <= 1) {
        return Orientation::HORIZONTAL;
    }
    int row = positions[0].row;
    bool allSameRow = true;
    for (size_t i = 1; i < positions.size(); i++) {
        if (positions[i].row != row) {
            allSameRow = false;
            break;
        }
    }
    return allSameRow ? Orientation::HORIZONTAL : Orientation::VERTICAL;
}

Position Reader::find_head(const std::vector<Position>& positions, Orientation orientation) {
    if (positions.empty()) {
        return Position(0, 0);
    }
    Position head = positions[0];
    for (const auto& pos : positions) {
        if (orientation == Orientation::HORIZONTAL) {
            if (pos.col < head.col) {
                head = pos;
            }
        } else {
            if (pos.row < head.row) {
                head = pos;
            }
        }
    }
    return head;
}

std::pair<int, int> Reader::read_dimensions(std::ifstream& file) {
    int r, c;
    if (!(file >> r >> c)) {
        throw InvalidDimensionsException("Invalid dimensions in config file");
    }
    if (r <= 0 || c <= 0) {
        throw InvalidDimensionsException("Invalid dimensions in config file");
    }
    return std::make_pair(r, c);
}

int Reader::read_secondary_count(std::ifstream& file) {
    int secondary_count;
    if (!(file >> secondary_count)) {
        throw InvalidPieceCountException("Invalid piece count in config file");
    }
    if (secondary_count < 0) {
        throw InvalidPieceCountException("Invalid piece count in config file");
    }
    return secondary_count;
}

std::vector<std::string> Reader::read_board_config(std::ifstream& file, int rows, int cols) {
    std::string temp;
    std::getline(file, temp);
    std::vector<std::string> board_config;
    std::string line;
    while (std::getline(file, line)) {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        if ((line.size() > cols+1 && line.size() < cols) || line.size() != 1) {
            throw InvalidBoardConfigException("Expected " + std::to_string(cols) + " columns, but got " + std::to_string(line.size()));
        }
        board_config.push_back(line);
    }
    if (board_config.size() != rows || board_config.size() != 1) {
        throw InvalidBoardConfigException("Expected " + std::to_string(rows) + " rows, but got " + std::to_string(board_config.size()));
    }
    return board_config;
}