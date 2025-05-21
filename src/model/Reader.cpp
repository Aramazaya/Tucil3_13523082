#include "model/Reader.hpp"


bool Reader::is_contiguous_piece(const std::vector<Position>& positions) {
    if (positions.size() <= 1) {
        return true;
    }
    bool isHorizontal = true;
    bool isVertical = true;
    int row = positions[0].row;
    int col = positions[0].col;
    for (size_t i = 1; i < positions.size(); i++) {
        if (positions[i].row != row) {
            isHorizontal = false;
        }
        if (positions[i].col != col) {
            isVertical = false;
        }
    }
    if (!isHorizontal && !isVertical) {
        return false;
    }
    if (isHorizontal) {
        std::vector<Position> sortedPos = positions;
        std::sort(sortedPos.begin(), sortedPos.end(), 
                 [](const Position& a, const Position& b) { return a.col < b.col; });
        for (size_t i = 1; i < sortedPos.size(); i++) {
            if (sortedPos[i].col != sortedPos[i-1].col + 1) {
                return false;
            }
        }
    } else {
        std::vector<Position> sortedPos = positions;
        std::sort(sortedPos.begin(), sortedPos.end(), 
                 [](const Position& a, const Position& b) { return a.row < b.row; });
        
        for (size_t i = 1; i < sortedPos.size(); i++) {
            if (sortedPos[i].row != sortedPos[i-1].row + 1) {
                return false;
            }
        }
    }
    return true;
}

Orientation Reader::determine_orientation(const std::vector<Position>& positions) {
    if (positions.size() <= 1) {
        return Orientation::HORIZONTAL;
    }
    int row = positions[0].row;
    bool allSameRow = true;
    for (size_t i = 1; i < positions.size(); i++) {
        if (positions[i].row != row) {
            allSameRow = false;
            break;
        }
    }
    return allSameRow ? Orientation::HORIZONTAL : Orientation::VERTICAL;
}

Position Reader::find_head(const std::vector<Position>& positions, Orientation orientation) {
    if (positions.empty()) {
        return Position(0, 0);
    }
    Position head = positions[0];
    for (const auto& pos : positions) {
        if (orientation == Orientation::HORIZONTAL) {
            if (pos.col < head.col) {
                head = pos;
            }
        } else {
            if (pos.row < head.row) {
                head = pos;
            }
        }
    }
    return head;
}

std::pair<int, int> Reader::read_dimensions(std::ifstream& file) {
    int r, c;
    if (!(file >> r >> c)) {
        throw InvalidDimensionsException("Invalid dimensions in config file");
    }
    if (r <= 0 || c <= 0) {
        throw InvalidDimensionsException("Invalid dimensions in config file");
    }
    return std::make_pair(r, c);
}

int Reader::read_secondary_count(std::ifstream& file) {
    int secondary_count;
    if (!(file >> secondary_count)) {
        throw InvalidPieceCountException("Invalid piece count in config file");
    }
    if (secondary_count < 0) {
        throw InvalidPieceCountException("Invalid piece count in config file");
    }
    return secondary_count;
}

std::vector<std::string> Reader::read_board_config(std::ifstream& file, int rows, int cols) {
    std::string temp;
    std::getline(file, temp);
    std::vector<std::string> board_config;
    std::string line;
    while (std::getline(file, line)) {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        board_config.push_back(line);
    }
    if (board_config.size() < rows) {
        throw InvalidBoardConfigException("Expected at least " + std::to_string(rows) + 
                                         " rows, but got " + std::to_string(board_config.size()));
    }
    if (board_config.size() > rows + 2) {
        throw InvalidBoardConfigException("Too many rows in configuration: " + 
                                         std::to_string(board_config.size()));
    }
    return board_config;
}

Position Reader::find_goal_position(const std::vector<std::string>& board_config, int rows, int cols) {
    Position goal_pos(-1, -1);
    bool goal_found = false;
    int goal_row = -1;
    int goal_col = -1;
    
    // Scan the entire configuration to find the 'K' and ensure there's only one
    for (size_t i = 0; i < board_config.size(); i++) {
        const std::string& line = board_config[i];
        
        size_t k_pos = line.find('K');
        if (k_pos != std::string::npos) {
            // Found a 'K'
            if (goal_found) {
                // Already found a 'K' before, so this is a second one
                throw InvalidGoalCellException("Multiple goal positions (K) found in the board configuration");
            }
            
            goal_found = true;
            goal_row = static_cast<int>(i);
            goal_col = static_cast<int>(k_pos);
            
            // Check if there's another 'K' in the same line
            if (line.find('K', k_pos + 1) != std::string::npos) {
                throw InvalidGoalCellException("Multiple goal positions (K) found in the board configuration");
            }
        }
    }
    
    if (!goal_found) {
        throw InvalidGoalCellException("No goal position (K) found in the board configuration");
    }
    
    // Now determine where the 'K' is (top, bottom, left, right, or within grid)
    bool is_top_row = (goal_row == 0 && board_config.size() > rows);
    bool is_bottom_row = (goal_row == static_cast<int>(board_config.size() - 1) && board_config.size() > rows);
    bool is_left_edge = (goal_col == 0);
    bool is_right_edge = false;
    
    // Check if 'K' is at the right edge (end of line)
    if (!is_top_row && !is_bottom_row) {
        const std::string& line = board_config[goal_row];
        // Right edge if 'K' is the last character or followed only by whitespace
        is_right_edge = (goal_col == static_cast<int>(line.length() - 1) || 
                         line.find_first_not_of(" \t\r\n", goal_col + 1) == std::string::npos);
    }
    
    // Check if goal is in a row that only contains 'K' and whitespace
    bool is_k_only_row = true;
    for (char c : board_config[goal_row]) {
        if (c != 'K' && !std::isspace(c)) {
            is_k_only_row = false;
            break;
        }
    }
    
    // Determine the grid-relative position of the goal
    int grid_row, grid_col;
    
    if (is_top_row && is_k_only_row) {
        // Top goal - map to first row, same column
        grid_row = 0;
        grid_col = goal_col;
        
        // Ensure column is within bounds
        if (grid_col >= cols) grid_col = cols - 1;
    } 
    else if (is_bottom_row && is_k_only_row) {
        // Bottom goal - map to last row, same column
        grid_row = rows - 1;
        grid_col = goal_col;
        
        // Ensure column is within bounds
        if (grid_col >= cols) grid_col = cols - 1;
    }
    else if (is_left_edge) {
        // Left edge goal - map to leftmost column, same row
        int row_offset = 0;
        
        // If there's a top goal row, adjust the row offset
        if (board_config.size() > rows && is_k_only_row && is_top_row) {
            row_offset = 1;
        }
        
        grid_row = goal_row - row_offset;
        grid_col = 0;
        
        // Ensure row is within bounds
        if (grid_row >= rows) grid_row = rows - 1;
        if (grid_row < 0) grid_row = 0;
    }
    else if (is_right_edge) {
        // Right edge goal - map to rightmost column, same row
        int row_offset = 0;
        
        // If there's a top goal row, adjust the row offset
        if (board_config.size() > rows && is_k_only_row && is_top_row) {
            row_offset = 1;
        }
        
        grid_row = goal_row - row_offset;
        grid_col = cols - 1;
        
        // Ensure row is within bounds
        if (grid_row >= rows) grid_row = rows - 1;
        if (grid_row < 0) grid_row = 0;
    }
    else {
        // Goal within the grid - adjust row if there's a top goal row
        int row_offset = 0;
        
        // If there's a top goal row, adjust the row offset
        if (board_config.size() > rows && 
            board_config[0].find('K') != std::string::npos && 
            board_config[0].find_first_not_of(" \t\r\nK") == std::string::npos) {
            row_offset = 1;
        }
        
        grid_row = goal_row - row_offset;
        grid_col = goal_col;
        
        // Ensure coordinates are within bounds
        if (grid_row >= rows) grid_row = rows - 1;
        if (grid_row < 0) grid_row = 0;
        if (grid_col >= cols) grid_col = cols - 1;
        if (grid_col < 0) grid_col = 0;
    }
    
    return Position(grid_row, grid_col);
}