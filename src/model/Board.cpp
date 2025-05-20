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

Board::Board(int rows, int cols, Position goal_pos) : rows(rows), cols(cols), goal_pos(goal_pos) {
    if (rows <= 0 || cols <= 0) {
        throw BoardException("Invalid board size");
    }
    if (goal_pos.row < 0 || goal_pos.row >= rows || goal_pos.col < 0 || goal_pos.col >= cols) {
        throw BoardException("Invalid goal position");
    }
    grid.resize(rows, std::vector<std::unique_ptr<Cell>>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j] = std::make_unique<Cell>(i, j, nullptr);
            if (i == goal_pos.row && j == goal_pos.col) {
                grid[i][j]->set_goal(); // Set the goal cell to empty
            }
        }
    }
}

Board::Board(const Board& other) : rows(other.rows), cols(other.cols) {
    grid.resize(rows, std::vector<std::unique_ptr<Cell>>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j] = std::make_unique<Cell>(i, j, nullptr);
        }
    }

    for (const auto& [id, piece] : other.pieces) {
        auto new_piece = std::make_shared<Piece>(*piece);
        pieces[id] = new_piece;
        update_cell(new_piece);
    }
}

std::shared_ptr<Piece> Board::get_piece(std::string id) const {
    auto it = pieces.find(id);
    if (it != pieces.end()) {
        return it->second;
    }
    return nullptr;
}

bool Board::operator==(const Board& other) const {
    if (rows != other.rows || cols != other.cols) {
        return false;
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (*grid[i][j] != *other.grid[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void Board::add_piece(const std::shared_ptr<Piece>& piece) {
    auto positions = piece->get_positions();
    if (!is_valid_piece(piece, piece->get_head())) {
        throw InvalidMoveException("Invalid move");
    }
    pieces.emplace(piece->get_id(), piece);
    update_cell(piece);
}

void Board::remove_piece(const std::shared_ptr<Piece>& piece) {
    auto positions = piece->get_positions();
    for (auto const& i : positions){
        if (grid[i.row][i.col]->get_is_empty()) {
            throw PieceNotFoundException("Piece not found at this position");
        }
    }
    pieces.erase(piece->get_id());
    clear_cell(piece);
}

void Board::move_piece(std::string piece_id, Position new_head) {
    auto it = pieces.find(piece_id);
    if (it == pieces.end()) {
        throw PieceNotFoundException("Piece not found");
    }
    auto piece = it->second;
    if (!piece->can_move(new_head)) {
        throw InvalidOrientationException("Invalid move orientation");
    }
    if (!is_valid_pos(new_head)) {
        throw InvalidPositionException("Invalid position");
    }
    auto positions = piece->get_positions();
    if (!is_valid_piece(piece, new_head)) {
        throw InvalidMoveException("Invalid move");
    }
    clear_cell(piece);
    piece->move(new_head);
    add_piece(piece);
}

bool Board::is_valid_pos(Position pos) const {
    if (pos.row < 0 || pos.row >= rows || pos.col < 0 || pos.col >= cols) {
        throw InvalidPositionException("Invalid position");
        return false;
    }
    return true;
}

bool Board::is_valid_piece(std::shared_ptr<Piece> piece, Position new_head) const {
    Position offset = {
        new_head.row - piece->get_head().row,
        new_head.col - piece->get_head().col
    };
    auto current_positions = piece->get_positions();
    for (const auto& pos : current_positions) {
        Position new_pos = {pos.row + offset.row, pos.col + offset.col};
        if (!is_valid_pos(new_pos)) {
            return false;
        }
        if (!grid[new_pos.row][new_pos.col]->get_is_empty()) {
            auto occupying_piece = grid[new_pos.row][new_pos.col]->get_piece();
            if (occupying_piece && occupying_piece->get_id() != piece->get_id()) {
                return false;
            }
        }
    }
}

Cell& Board::get_cell(Position pos) const {
    if (!is_valid_pos(pos)) {
        throw InvalidPositionException("Invalid position");
    }
    return *grid[pos.row][pos.col];
}

std::vector<Position> Board::get_possible_positions(std::shared_ptr<Piece> piece) const {
    std::vector<Position> positions;
    Position current_position = piece->get_head();
    if (piece->get_orientation() == Orientation::HORIZONTAL) {
        for (int i = 0; i < cols; ++i) {
            Position pos(current_position.row, i);
            if (is_valid_piece(piece, pos) && pos != current_position) {
                positions.push_back(pos);
            }
        }
    } else {
        for (int i = 0; i < rows; ++i) {
            Position pos(i, current_position.col);
            if (is_valid_piece(piece, pos) && pos != current_position) {
                positions.push_back(pos);
            }
        }
    }
}