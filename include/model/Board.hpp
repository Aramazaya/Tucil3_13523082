#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Piece.hpp"
class Piece;

struct Position {
    int row;
    int col;
    
    Position(int r = 0, int c = 0) : row(r), col(c) {}
    
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

class Cell : public std::enable_shared_from_this<Cell> {
private:
    Position position;
    std::shared_ptr<Piece> piece;
    bool is_goal; // true if the cell is a goal cell
public:
    Cell(Position pos, const std::shared_ptr<Piece> piece) :position(pos), piece(piece) {}
    std::shared_ptr<Piece> get_piece() const { return piece; }
    void set_piece(const std::shared_ptr<Piece> piece) { this->piece = piece; }
    bool get_is_empty() { return piece == nullptr; }
    bool get_is_goal() { return is_goal; }
};
class Board : public std::enable_shared_from_this<Board> {
private:
    int rows;
    int cols;
    std::vector<std::vector<Cell>> grid;
};