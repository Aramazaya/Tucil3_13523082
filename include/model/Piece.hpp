#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Board.hpp"
enum Orientation {
    HORIZONTAL,
    VERTICAL
};
enum Color {
    RED,
    GREEN,
    BLUE,
    YELLOW
};
class Piece {
protected:
    std::string id;
    int length;
    Position head;
    std::vector<Position> positions;
    Orientation orientation;
    Color color;
public:
    Piece(std::string id, int length, const Position& pos, const Orientation& orient, const Color& co);
    Piece(const Piece& other);
    bool can_move(const Position& newHead) const;
    std::string get_id() const { return id; }
    int get_length() const { return length; }
    Orientation get_orientation() const { return orientation; }
    std::vector<Position> get_positions() const { return positions;}
    Color get_color() const { return color; }
    Position get_head() const { return head; }
    void move(Position newHead);
};

class Primary : public Piece {
public:
    Primary(int length, const Position& pos, const Orientation& orientation): Piece("P", length, pos, orientation, RED) {}
};

class Secondary : public Piece {
private:
    static int counter;
public:
    Secondary(std::string id, int length, const Position& pos, const Orientation& orientation);
};