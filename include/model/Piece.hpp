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
    Orientation orientation;
    Color color;
public:
    Piece(int id, int length, const Position& pos, const Orientation& orient, const Color& co) : id(id), length(length), head(pos), orientation(orient), color(co) {}

    int getId() const { return id; }
    int getLength() const { return length; }
    Orientation getOrientation() const { return orientation; }
    Color getColor() const { return color; }
    Position getHead() const { return head; }
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