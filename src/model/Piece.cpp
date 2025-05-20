#include "Piece.hpp"

int Secondary::counter = 0;
Secondary::Secondary(std::string id, int length, const Position& pos, const Orientation& orientation) : Piece(id, length, pos, orientation, static_cast<Color>(counter % 3 + 1)) {
    counter++;
}

Piece::Piece(std::string id, int length, const Position& pos, const Orientation& orient, const Color& co) : id(id), length(length), head(pos), orientation(orient), color(co) {
    positions.clear();
    if (orientation == Orientation::HORIZONTAL) {
        for (int i = head.col; i < head.col + length; i++) {
            Position x(head.row, i);
            positions.push_back(x);
        }
    } else {
        for (int i = head.row; i < head.row + length; i++) {
            Position x(i, head.col);
            positions.push_back(x);
        }
    }
}
Piece::Piece(const Piece& other) : id(other.id), length(other.length), head(other.head), orientation(other.orientation), color(other.color) {
    positions.clear();
    for (const auto& pos : other.positions) {
        positions.push_back(pos);
    }
}

void Piece::move(Position newHead) {
    head = newHead;
    positions.clear();
    if (orientation == Orientation::HORIZONTAL) {
        for (int i = head.col; i < head.col + length; i++) {
            Position x(head.row, i);
            positions.push_back(x);
        }
    } else {
        for (int i = head.row; i < head.row + length; i++) {
            Position x(i, head.col);
            positions.push_back(x);
        }
    }
}

bool Piece::can_move(const Position& new_head) const {
    if (orientation == Orientation::HORIZONTAL) {
        return new_head.row == get_head().row;
    } else {
        return new_head.col == get_head().col;
    }
}