#include "model/ProgramModel.hpp"
#include <iostream>

ProgramModel::ProgramModel() {
    init();
}

void ProgramModel::init() {
    algorithms["a*"] = std::make_shared<AStar>();
    algorithms["uniformcostsearch"] = std::make_shared<UCS>();
    algorithms["ucs"] = std::make_shared<UCS>();
    algorithms["greddybestfirstsearch"] = std::make_shared<GBFS>();
    algorithms["gbfs"] = std::make_shared<GBFS>();
    algorithms["a"] = std::make_shared<AStar>();
    algorithms["iterativedeepeninga*"] = std::make_shared<IDA>();
    algorithms["ida*"] = std::make_shared<IDA>();

}

void ProgramModel::create_board(int rows, int cols, int sec_count, std::vector<std::string> board_config) {
    Position goal_pos = Reader::find_goal_position(board_config, rows, cols);
    std::cout << "Goal position: " << goal_pos.row << ", " << goal_pos.col << std::endl;
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    initial_board = Board(rows, cols, goal_pos);
    std::map<char, std::vector<Position>> pieces_positions;
    int start_row = 0;
    if (board_config.size() > rows && 
        board_config[0].find('K') != std::string::npos &&
        board_config[0].find_first_not_of(" \t\r\nK") == std::string::npos) {
        start_row = 1;
    }
    for (int row = 0; row < rows; ++row) {
        int config_row = row + start_row;
        
        if (config_row < board_config.size()) {
            const std::string& line = board_config[config_row];
            
            for (int col = 0; col < cols && col < line.size(); ++col) {
                char piece_char = line[col];
                
                // Skip empty cells and 'K' (already processed)
                if (piece_char != '.' && piece_char != ' ' && piece_char != 'K') {
                    pieces_positions[piece_char].emplace_back(row, col);
                }
            }
        }
    }
    if (pieces_positions.find('P') == pieces_positions.end()) {
        throw MissingPrimaryCarException("Primary car (P) not found in the board configuration");
    }
    int secondary_count = 0;
    for (const auto& pair : pieces_positions) {
        if (pair.first != 'P' && pair.first != '.') {
            std::cout << "Secondary car: " << pair.first << std::endl;
            secondary_count++;
        }
    }
    if (secondary_count != sec_count) {
        throw InvalidPieceCountException("Yang bener lah rek. Kurang mobilnya. Expected " + std::to_string(sec_count) + 
                                         " secondary pieces, but found " + std::to_string(secondary_count));
    }
    for (const auto& pair : pieces_positions) {
        char id = pair.first;
        const auto& positions = pair.second;
        if (!Reader::is_contiguous_piece(positions)) {
            throw InvalidBoardConfigException("Invalid piece configuration for piece " + std::string(1, id));
        }
        Orientation orientation = Reader::determine_orientation(positions);
        Position head = Reader::find_head(positions, orientation);
        std::shared_ptr<Piece> piece;
        if (id == 'P'){
            piece = std::make_shared<Primary>(positions.size(), head, orientation);
        } else {
            piece = std::make_shared<Secondary>(std::string(1, id), positions.size(), head, orientation);
        }

        try{
            initial_board.add_piece(piece);
        } catch (const Board::InvalidMoveException& e) {
            throw InvalidBoardConfigException("Invalid piece configuration for piece " + std::string(1, id));
        } catch (const Board::InvalidPositionException& e) {
            throw InvalidBoardConfigException("Invalid piece configuration for piece " + std::string(1, id));
        } catch (const Board::PieceNotFoundException& e) {
            throw InvalidBoardConfigException("Invalid piece configuration for piece " + std::string(1, id));
        }
    }
}

Algorithms::Result ProgramModel::run_solver(const std::string& algorithm, const std::string& heuristics) {
    auto it = algorithms.find(algorithm);
    if (it == algorithms.end()) {
        throw std::runtime_error("Algorithm not found: " + algorithm);
    }
    it->second->set_heuristics(heuristics);
    return it->second->solve(initial_board);
}