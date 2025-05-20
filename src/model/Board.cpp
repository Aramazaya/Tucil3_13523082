#include "Board.hpp"

void Board::update_cell(std::shared_ptr<Piece> piece) {
    auto positions = piece->get_positions();
    for (auto const& i : positions){
        grid[i.row][i.col]->set_piece(piece);
    }
}

void Board::clear_cell(std::shared_ptr<Piece> piece) {
    auto positions = piece->get_positions();
    for (auto const& i : positions){
        grid[i.row][i.col]->set_piece(nullptr);
    }
}