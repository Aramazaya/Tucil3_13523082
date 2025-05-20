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
    int id;
    int length;
    Position head;
    std::vector<Position> positions;
    Orientation orientation;
    Color color;
public:
    Piece(int id, int length, const Position& pos, const Orientation& orient, const Color& co) : id(id), length(length), head(pos), orientation(orient), color(co) {
        if (orient == Orientation::HORIZONTAL) {
            for (int i = head.col;i<head.col + length; i++){
                Position x(head.row, i);
                positions.push_back(x);
            }
        } else {
            for (int i = head.row; i < head.row + length; i++){
                Position x(i, head.col);
                positions.push_back(x);
            }
        }
    }

    int get_id() const { return id; }
    int get_length() const { return length; }
    Orientation get_orientation() const { return orientation; }
    std::vector<Position> get_positions() const { return positions;}
    Color get_color() const { return color; }
    Position get_head() const { return head; }
    virtual void move(Position newHead);
};

class Primary : public Piece {
public:
    Primary(int length, const Position& pos, const Orientation& orientation): Piece(0, length, pos, orientation, RED) {}
    
};

class Secondary : public Piece {
private:
    static int counter;
public:
    Secondary(int id, int length, const Position& pos, const Orientation& orientation) : Piece(id, length, pos, orientation, static_cast<Color>(counter % 3 + 1)) {
        counter++;
    }
};