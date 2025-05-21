#pragma once
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <exception>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include "Piece.hpp"
#include "Position.h"
class Piece;

class BoardException : public std::exception {
    private:
        std::string message;
    public:
        BoardException(const std::string& msg) : message(msg) {}
        const char* what() const noexcept override {
            return message.c_str();
        }
    };

class Cell : public std::enable_shared_from_this<Cell> {
private:
    Position position;
    std::shared_ptr<Piece> piece;
    bool is_goal;
public:
    Cell(int x, int y, const std::shared_ptr<Piece> piece) :position(Position(x, y)), piece(piece) {}
    std::shared_ptr<Piece> get_piece() const { return piece; }
    bool operator==(const Cell& other) const {
        return position == other.position && piece == other.piece;
    }
    bool operator!=(const Cell& other) const {
        return !(*this == other);
    }
    void set_piece(const std::shared_ptr<Piece> piece) { this->piece = piece; }
    void set_empty() { piece = nullptr; }
    void set_goal() { is_goal = true; }
    bool get_is_empty() { return piece == nullptr; }
    bool get_is_goal() { return is_goal; }
};

class Board : public std::enable_shared_from_this<Board> {
private:
    int rows;
    int cols;
    std::vector<std::vector<std::unique_ptr<Cell>>> grid;
    Position goal_pos;
    std::map<std::string, std::shared_ptr<Piece>> pieces;
    void update_cell(std::shared_ptr<Piece> piece);
    void clear_cell(std::shared_ptr<Piece> piece);
public:
    Board() = default;
    Board(int rows, int cols, Position goal_pos = {0,0});
    Board(const Board& other);
    ~Board() = default;
    int get_rows() const { return rows;}
    int get_cols() const { return cols;}
    Position get_goal_pos() const { return goal_pos;}
    void set_goal_pos(Position pos) { goal_pos = pos;}
    void add_piece(const std::shared_ptr<Piece>& piece);
    void remove_piece(const std::shared_ptr<Piece>& piece);
    void move_piece(std::string piece_id, Position new_head);
    bool is_valid_pos(Position pos) const;
    bool is_valid_piece(std::shared_ptr<Piece> piece, Position new_head) const;
    Cell& get_cell(Position pos) const;
    std::vector<std::vector<std::unique_ptr<Cell>>>& get_grid() { return grid;}
    const std::map<std::string, std::shared_ptr<Piece>>& get_pieces() const { return pieces;}
    std::shared_ptr<Piece> get_piece(Position pos) const { return grid[pos.row][pos.col]->get_piece();}
    std::shared_ptr<Piece> get_piece(std::string id) const;
    std::vector<Position> get_possible_positions(std::shared_ptr<Piece> piece) const;
    Board& operator=(const Board& other);
    bool operator==(const Board& other) const;
    bool operator!=(const Board& other) const { return !(*this == other);}
    std::string to_string() const;
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