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
    grid.clear();
    grid.reserve(rows);
    for (int i = 0; i < rows; ++i) {
        std::vector<std::unique_ptr<Cell>> row;
        row.reserve(cols);
        for (int j = 0; j < cols; ++j) {
            row.push_back(std::make_unique<Cell>(i, j, nullptr));
        }
        grid.push_back(std::move(row));
    }
}

Board::Board(const Board& other) : std::enable_shared_from_this<Board>(), rows(other.rows), cols(other.cols), goal_pos(other.goal_pos) {
    grid.resize(rows);
    for (int i = 0; i < rows; ++i) {
        grid[i].reserve(cols);
        for (int j = 0; j < cols; ++j) {
            grid[i].push_back(std::make_unique<Cell>(i, j, nullptr));
            if (other.grid[i][j]->get_is_goal()) {
                grid[i][j]->set_goal();
            }
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
        throw InvalidPositionException("Invalid position at move_piece" + std::to_string(new_head.row) + " " + std::to_string(new_head.col) + "Piece: " + piece->get_id());
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
    return true;
}

Cell& Board::get_cell(Position pos) const {
    if (!is_valid_pos(pos)) {
        throw InvalidPositionException("Invalid position");
    }
    return *grid[pos.row][pos.col];
}

std::vector<Position> Board::get_possible_positions(std::shared_ptr<Piece> piece) const {
    std::vector<Position> possible_positions;
    Position current_head = piece->get_head();
    int length = piece->get_length();
    
    if (piece->get_orientation() == Orientation::HORIZONTAL) {
        // For horizontal pieces, check left and right movement
        
        // Check leftward movement
        for (int col = current_head.col - 1; col >= 0; col--) {
            Position check_pos(current_head.row, col);
            
            // Check if this position is occupied
            if (!grid[check_pos.row][check_pos.col]->get_is_empty()) {
                // Position is occupied by another piece, can't move further left
                break;
            }
            
            // This position is free, so we can move the piece here
            Position new_head(current_head.row, col);
            possible_positions.push_back(new_head);
        }
        
        // Check rightward movement
        int rightmost_col = current_head.col + length - 1;
        for (int col = rightmost_col + 1; col < cols; col++) {
            Position check_pos(current_head.row, col);
            
            // Check if this position is occupied
            if (!grid[check_pos.row][check_pos.col]->get_is_empty()) {
                // Position is occupied by another piece, can't move further right
                break;
            }
            
            // This position is free, so we can move the piece with its head at (row, col-length+1)
            Position new_head(current_head.row, col - length + 1);
            possible_positions.push_back(new_head);
        }
    } else {
        // For vertical pieces, check upward and downward movement
        
        // Check upward movement
        for (int row = current_head.row - 1; row >= 0; row--) {
            Position check_pos(row, current_head.col);
            
            // Check if this position is occupied
            if (!grid[check_pos.row][check_pos.col]->get_is_empty()) {
                // Position is occupied by another piece, can't move further up
                break;
            }
            
            // This position is free, so we can move the piece here
            Position new_head(row, current_head.col);
            possible_positions.push_back(new_head);
        }
        
        // Check downward movement
        int bottommost_row = current_head.row + length - 1;
        for (int row = bottommost_row + 1; row < rows; row++) {
            Position check_pos(row, current_head.col);
            
            // Check if this position is occupied
            if (!grid[check_pos.row][check_pos.col]->get_is_empty()) {
                // Position is occupied by another piece, can't move further down
                break;
            }
            
            // This position is free, so we can move the piece with its head at (row-length+1, col)
            Position new_head(row - length + 1, current_head.col);
            possible_positions.push_back(new_head);
        }
    }
    
    return possible_positions;
}

Board& Board::operator=(const Board& other) {
    if (this != &other) {
        rows = other.rows;
        cols = other.cols;
        goal_pos = other.goal_pos;
        grid.clear();
        grid.reserve(rows);
        for (int i = 0; i < rows; ++i) {
            std::vector<std::unique_ptr<Cell>> row;
            row.reserve(cols);
            for (int j = 0; j < cols; ++j) {
                row.push_back(std::make_unique<Cell>(i, j, nullptr));
                if (i == goal_pos.row && j == goal_pos.col) {
                    row.back()->set_goal();
                }
            }
            grid.push_back(std::move(row));
        }
        pieces.clear();
        for (const auto& [id, piece] : other.pieces) {
            auto new_piece = std::make_shared<Piece>(*piece);
            pieces[id] = new_piece;
            update_cell(new_piece);
        }
    }
    return *this;
}

std::string Board::to_string() const {
    std::ostringstream oss;
    int rows = this->rows;
    int cols = this->cols;
    Position goalPos = this->goal_pos;
    const std::string RED = "\033[1;31m";      // Primary car
    const std::string GREEN = "\033[1;32m";    // Goal cell/border
    const std::string YELLOW = "\033[1;33m";   // Secondary car option
    const std::string BLUE = "\033[1;34m";     // Secondary car option
    const std::string MAGENTA = "\033[1;35m";  // Secondary car option
    const std::string CYAN = "\033[1;36m";     // Secondary car option
    const std::string RESET = "\033[0m";       // Reset color
    std::unordered_map<std::string, std::string> pieceColors;
    for (const auto& [id, piece] : pieces) {
        if (id == "P" || id == "R") {
            pieceColors[id] = RED;
        } else {
            char firstChar = id[0];
            int colorIndex = (firstChar % 4);
            
            switch (colorIndex) {
                case 0: pieceColors[id] = YELLOW; break;
                case 1: pieceColors[id] = BLUE; break;
                case 2: pieceColors[id] = MAGENTA; break;
                case 3: pieceColors[id] = CYAN; break;
                default: pieceColors[id] = YELLOW; break;
            }
        }
    }
    bool isTopGoal = (goalPos.row == 0);
    bool isBottomGoal = (goalPos.row == rows - 1);
    bool isLeftGoal = (goalPos.col == 0);
    bool isRightGoal = (goalPos.col == cols - 1);
    oss << "╔";
    for (int c = 0; c < cols; c++) {
        if (isTopGoal && c == goalPos.col) {
            oss << GREEN << "═══" << RESET;
        } else {
            oss << "═══";
        }
    }
    oss << "╗\n";
    for (int r = 0; r < rows; r++) {
        if (isLeftGoal && r == goalPos.row) {
            oss << GREEN << "║" << RESET;
        } else {
            oss << "║";
        }
        for (int c = 0; c < cols; c++) {
            Position pos(r, c);
            auto cell = grid[r][c].get();
            auto piece = cell->get_piece();
            
            if (piece) {
                const std::string& pieceId = piece->get_id();
                std::string colorCode = pieceColors[pieceId];
                oss << " " << colorCode << pieceId << RESET << " ";
            } else {
                oss << "   ";
            }
        }
        if (isRightGoal && r == goalPos.row) {
            oss << GREEN << "║" << RESET << "\n";
        } else {
            oss << "║\n";
        }
    }
    oss << "╚";
    for (int c = 0; c < cols; c++) {
        if (isBottomGoal && c == goalPos.col) {
            oss << GREEN << "═══" << RESET;
        } else {
            oss << "═══";
        }
    }
    oss << "╝\n";
    return oss.str();
}