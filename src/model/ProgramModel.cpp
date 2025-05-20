#include "model/ProgramModel.hpp"

ProgramModel::ProgramModel() {
    init();
}

void ProgramModel::init() {
    algorithms["A*"] = std::make_unique<AStar>();
    algorithms["UCS"] = std::make_unique<UCS>();
    algorithms["GBFS"] = std::make_unique<GBFS>();
}

void ProgramModel::create_board(int rows, int cols, int sec_count, std::vector<std::string> board_config) {
    initial_board = Board(rows, cols);
    std::map<char, std::vector<Position>> pieces_positions;
    Position goal_pos(-1, -1);
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            char piece_char = board_config[row][col];
            if (piece_char == 'K') {
                goal_pos = Position(row, col);
                if (initial_board.get_goal_pos().row != -1 || initial_board.get_goal_pos().col != -1) {
                    throw InvalidGoalCellException("Yang bener lah rek. Masa pintunya ada dua?");
                }
            }
            if (piece_char != '.' && piece_char != 'K') {
                pieces_positions[piece_char].emplace_back(row, col);
            }
        }
    }
    if (initial_board.get_goal_pos().row == -1 || initial_board.get_goal_pos().col == -1) {
        throw InvalidGoalCellException("Yang bener lah rek. Keluarnya lewat mana?");
    }
    if (pieces_positions.find('P') == pieces_positions.end()) {
        throw MissingPrimaryCarException("Yang bener lah rek. Mobil lu mana?");
    }
    int secondary_count = 0;
    for (const auto& pair : pieces_positions) {
        if (pair.first != 'P' && pair.first != '.') {
            secondary_count++;
        }
    }
    if (secondary_count != sec_count) {
        throw InvalidPieceCountException("Yang bener lah rek. Kurang mobilnya.");
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
    initial_board.set_goal_pos(goal_pos);
}

void ProgramModel::run_solver(const std::string& algorithm) {
    auto it = algorithms.find(algorithm);
    if (it == algorithms.end()) {
        throw std::runtime_error("Algorithm not found: " + algorithm);
    }
    auto result = it->second->solve(initial_board);
}