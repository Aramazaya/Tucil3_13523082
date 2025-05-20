#pragma once
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <exception>
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
    Cell(int x, int y, const std::shared_ptr<Piece> piece) :position(Position(x, y)), piece(piece) {}
    std::shared_ptr<Piece> get_piece() const { return piece; }
    void set_piece(const std::shared_ptr<Piece> piece) { this->piece = piece; }
    bool get_is_empty() { return piece == nullptr; }
    bool get_is_goal() { return is_goal; }
};
class Board : public std::enable_shared_from_this<Board> {
private:
    int rows;
    int cols;
    std::vector<std::vector<std::unique_ptr<Cell>>> grid;
    std::map<std::string, std::shared_ptr<Piece>> pieces;
    void update_cell(std::shared_ptr<Piece> piece);
    void clear_cell(std::shared_ptr<Piece> piece);
public:
    Board(int rows, int cols);
    void add_piece(const std::shared_ptr<Piece>& piece);
    void remove_piece(const std::shared_ptr<Piece>& piece);
    void move_piece(std::shared_ptr<Piece>& piece, Position newHead);
    BoardException is_valid_pos(Position pos) const;
    Cell& get_cell(Position pos) const;
    class InvalidPositionException : public BoardException {
    public:
        InvalidPositionException(const std::string& msg) : BoardException(msg) {}
    };
    class PieceNotFoundException : public BoardException {
    public:
        PieceNotFoundException(const std::string& msg) : BoardException(msg) {}
    };
    class InvalidMoveException : public BoardException {
    public:
        InvalidMoveException(const std::string& msg) : BoardException(msg) {}
    };
    class PieceAlreadyExistsException : public BoardException {
    public:
        PieceAlreadyExistsException(const std::string& msg) : BoardException(msg) {}
    };
    class CellOccupiedException : public BoardException {
    public:
        CellOccupiedException(const std::string& msg) : BoardException(msg) {}
    };
    class CellNotFoundException : public BoardException {
    public:
        CellNotFoundException(const std::string& msg) : BoardException(msg) {}
    };
    class InvalidPieceException : public BoardException {
    public:
        InvalidPieceException(const std::string& msg) : BoardException(msg) {}
    };
    class InvalidOrientationException : public BoardException {
    public:
        InvalidOrientationException(const std::string& msg) : BoardException(msg) {}
    };
    class InvalidColorException : public BoardException {
    public:
        InvalidColorException(const std::string& msg) : BoardException(msg) {}
    };
};

class BoardException : public std::exception {
private:
    std::string message;
public:
    BoardException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};