#include "Algorithms.hpp"

Steps::Steps(const Board& board) : board(board) {
    for (const auto& [id, piece] : board.get_pieces()) {
        pieces[id] = piece->get_head();
    }
}

Steps::Steps(const Steps& other) : pieces(other.pieces), board(other.board) {}

std::vector<Steps> Steps::get_possible_steps() const {
    std::vector<Steps> possible_steps;
    for (const auto& [id, piece] : board.get_pieces()) {
        for (const auto& pos : board.get_possible_positions(piece)) {
            Board new_board = board;
            try{
                new_board.move_piece(id, pos);
                possible_steps.emplace_back(new_board);
            } catch (const Board::InvalidMoveException& e) {
                continue;
            } catch (const Board::InvalidPositionException& e) {
                continue;
            } catch (const Board::PieceNotFoundException& e) {
                continue;
            }
        }
    }
    return possible_steps;
}

size_t Steps::hash() const {
    size_t hash_val = 0;
    for (const auto& [id, pos] : pieces) {
        // Combine hash with piece ID and position
        hash_val ^= std::hash<std::string>{}(id) ^ 
                    std::hash<int>{}(pos.row) ^ 
                    std::hash<int>{}(pos.col);
    }
    return hash_val;
}

bool Steps::is_goal() const {
    for (const auto& [id, piece] : board.get_pieces()) {
        if (piece->get_id() == "P") {
            Position goal_pos = {piece->get_head().row, board.get_cols() - 1};
            if (piece->get_head() != goal_pos) {
                return false;
            }
        }
    }
    return true;
}

//Placeholder
std::string Steps::to_string() const {
    std::string result = "Steps:\n";
    for (const auto& [id, pos] : pieces) {
        result += "Piece ID: " + id + ", Position: (" + std::to_string(pos.row) + ", " + std::to_string(pos.col) + ")\n";
    }
    return result;
}

namespace std {
    template<>
    struct hash<Steps> {
        size_t operator()(const Steps& state) const {
            return state.hash();
        }
    };
}

std::vector<Steps> Algorithms::get_path(std::unordered_map<Steps, Steps>& from, Steps& current) {
    std::vector<Steps> path;
    while (from.find(current) != from.end()) {
        path.push_back(current);
        current = from[current];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

double Algorithms::manhattan_distance(const Steps& state) {
    auto primary = state.get_board().get_piece("P");
    auto primary_pos = primary->get_positions();
    auto goal_pos = state.get_board().get_goal_pos();
    Position closest = primary_pos[0];
    for (const auto& pos : primary_pos) {
        if (pos.col > closest.col) {
            closest = pos;
        }
    }
    return std::abs(closest.col - goal_pos.col);
}

Algorithms::Result AStar::solve(const Board& board) {
    auto start_time = std::chrono::high_resolution_clock::now();
    int states_explored = 0;
    Steps start(board);
    std::priority_queue<std::pair<double, Steps>, std::vector<std::pair<double, Steps>>,PQCompare<double>> frontier;
    std::unordered_set<Steps> closed_set;
    std::unordered_map<Steps, Steps> from;
    std::unordered_map<Steps, double> g_score;
    g_score[start] = 0.0;
    std::unordered_map<Steps, double> f_score;
    f_score[start] = manhattan_distance(start);
    frontier.push({f_score[start], start});
    while (!frontier.empty()) {
        auto current = frontier.top().second;
        frontier.pop();
        states_explored++;
        if(current.is_goal()){
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_time = end_time - start_time;
            std::vector<Steps> path = get_path(from, current);
            return {path, path.size(), states_explored, elapsed_time.count()};
        }
        if (closed_set.find(current) != closed_set.end()) {
            continue;
        }
        closed_set.insert(current);
        for (const auto& next : current.get_possible_steps()) {
            if (closed_set.find(next) != closed_set.end()) {
                continue;
            }
            double tentative_g_score = g_score[current] + 1.0;
            if (g_score.find(next) == g_score.end() || tentative_g_score < g_score[next]) {
                from[next] = current;
                g_score[next] = tentative_g_score;
                f_score[next] = tentative_g_score + manhattan_distance(next);
                frontier.push({f_score[next], next});
            }
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    return {{}, 0, states_explored, elapsed_time.count()};
}

Algorithms::Result GBFS::solve(const Board& board) {
    auto start_time = std::chrono::high_resolution_clock::now();
    int states_explored = 0;
    Steps start(board);
    std::priority_queue<std::pair<double, Steps>, std::vector<std::pair<double, Steps>>, PQCompare<double>> frontier;
    std::unordered_set<Steps> visited;
    std::unordered_map<Steps, Steps> from;
    frontier.push({manhattan_distance(start), start});
    while (!frontier.empty()) {
        auto current = frontier.top().second;
        frontier.pop();
        states_explored++;
        if (current.is_goal()) {
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_time = end_time - start_time;
            std::vector<Steps> path = get_path(from, current);
            return {path, path.size(), states_explored, elapsed_time.count()};
        }
        if (visited.find(current) != visited.end()) {
            continue;
        }
        visited.insert(current);
        for (const auto& next : current.get_possible_steps()) {
            if (visited.find(next) != visited.end()) {
                continue;
            }
            frontier.push({manhattan_distance(next), next});
            from[next] = current;
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    return {{}, 0, states_explored, elapsed_time.count()};
}

Algorithms::Result UCS::solve(const Board& board) {
    auto start_time = std::chrono::high_resolution_clock::now();
    int states_explored = 0;
    Steps start(board);
    std::priority_queue<std::pair<double, Steps>, std::vector<std::pair<double, Steps>>, PQCompare<double>> frontier;
    std::unordered_set<Steps> visited;
    std::unordered_map<Steps, Steps> from;
    std::unordered_map<Steps, double> g_score;
    g_score[start] = 0.0;
    frontier.push({g_score[start], start});
    while (!frontier.empty()) {
        auto current = frontier.top().second;
        frontier.pop();
        states_explored++;
        if (current.is_goal()) {
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_time = end_time - start_time;
            std::vector<Steps> path = get_path(from, current);
            return {path, path.size(), states_explored, elapsed_time.count()};
        }
        if (visited.find(current) != visited.end()) {
            continue;
        }
        visited.insert(current);
        for (const auto& next : current.get_possible_steps()) {
            if (visited.find(next) != visited.end()) {
                continue;
            }
            double tentative_g_score = g_score[current] + 1.0;
            if (g_score.find(next) == g_score.end() || tentative_g_score < g_score[next]) {
                from[next] = current;
                g_score[next] = tentative_g_score;
                frontier.push({g_score[next], next});
            }
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    return {{}, 0, states_explored, elapsed_time.count()};
}