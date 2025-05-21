#include "Algorithms.hpp"

Steps::Steps(const Board& board) : board(board) {
    for (const auto& [id, piece] : board.get_pieces()) {
        pieces[id] = piece->get_head();
    }
}

Steps::Steps(const Steps& other) : board(Board(other.board)) {
    pieces = other.pieces;
}

Steps& Steps::operator=(const Steps& other) {
    if (this != &other) {
        board = other.board;
        pieces = other.pieces;
    }
    return *this;
}

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

bool Steps::operator==(const Steps& other) const {
    if (pieces.size() != other.pieces.size()) {
        return false;
    }
    
    for (const auto& [id, pos] : pieces) {
        auto it = other.pieces.find(id);
        if (it == other.pieces.end()) {
            return false;
        }
        
        if (it->second.row != pos.row || it->second.col != pos.col) {
            return false;
        }
    }
    
    return true;
}

size_t Steps::hash() const {
    size_t result = 0;
    const size_t prime = 31;
    
    for (const auto& [id, pos] : pieces) {
        size_t piece_hash = std::hash<std::string>{}(id);
        piece_hash = piece_hash * prime + std::hash<int>{}(pos.row);
        piece_hash = piece_hash * prime + std::hash<int>{}(pos.col);
        result = result * prime + piece_hash;
    }
    
    return result;
}

bool Steps::is_goal() const {
    Position goal_pos = board.get_goal_pos();
    auto primary_iter = board.get_pieces().find("P");
    if (primary_iter == board.get_pieces().end()) {
        return false;
    }
    auto primary_car = primary_iter->second;
    std::vector<Position> primary_positions = primary_car->get_positions();
    for (const auto& pos : primary_positions) {
        if (pos == goal_pos) {
            return true;
        }
    }
    return false;
}

std::string Steps::to_string() const {
    std::ostringstream oss;
    oss << board.to_string();
    return oss.str();
}

namespace std {
    template<>
    struct hash<Steps> {
        size_t operator()(const Steps& s) const {
            return s.hash();
        }
    };
}

std::vector<Steps> Algorithms::get_path(std::unordered_map<Steps, Steps>& from, Steps& goal, Steps& start) {
    std::vector<Steps> path;
    Steps current = goal;
    path.push_back(current);
    while (from.find(current) != from.end()) {
        current = from[current];
        path.insert(path.begin(), current);
    }
    if (path.empty() || !(path.front() == start)) {
        path.insert(path.begin(), start);
    }
    
    return path;
}

double Algorithms::blocked_car_heuristic(const Steps& state) {
    const Board& board = state.get_board();
    auto primary = board.get_piece("P");
    
    if (!primary) return 0.0;
    auto orientation = primary->get_orientation();
    auto primary_positions = primary->get_positions();
    Position goal_pos = board.get_goal_pos();
    if (orientation == Orientation::VERTICAL) {
        return 100.0;
    }
    int primary_rightmost = -1;
    for (const auto& pos : primary_positions) {
        if (pos.col > primary_rightmost) {
            primary_rightmost = pos.col;
        }
    }
    int primary_row = primary_positions[0].row;
    double blocking_count = 0;
    int goal_col = (goal_pos.col == board.get_cols() - 1) ? goal_pos.col : goal_pos.col;
    for (int col = primary_rightmost + 1; col <= goal_col; col++) {
        Position check_pos(primary_row, col);
        if (col < board.get_cols() && !board.get_cell(check_pos).get_is_empty()) {
            auto blocking_piece = board.get_cell(check_pos).get_piece();
            if (blocking_piece->get_orientation() == Orientation::VERTICAL) {
                blocking_count += 2.0;
            } else {
                blocking_count += 1.0;
            }
        }
    }
    double distance = goal_col - primary_rightmost;
    double indirect_blocking = 0.0;
    for (const auto& [id, piece] : board.get_pieces()) {
        if (id == "P") continue;
        bool is_direct_blocker = false;
        for (const auto& pos : piece->get_positions()) {
            if (pos.row == primary_row && pos.col > primary_rightmost && pos.col <= goal_col) {
                is_direct_blocker = true;
                break;
            }
        }
        
        if (is_direct_blocker) {
            auto orientation = piece->get_orientation();
            
            if (orientation == Orientation::VERTICAL) {
                bool can_move = false;
                std::vector<Position> possible_positions = board.get_possible_positions(piece);
                if (possible_positions.size() <= 1) {
                    indirect_blocking += 1.0;
                }
            } else {
                std::vector<Position> possible_positions = board.get_possible_positions(piece);
                if (possible_positions.size() <= 1) {
                    indirect_blocking += 1.0;
                }
            }
        }
    }
    double heuristic = distance + (blocking_count * 1.5) + (indirect_blocking * 0.5);
    
    return heuristic;
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
    if (start.is_goal()) {
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end_time - start_time;
        std::vector<Steps> path = {start};
        return {path, 0, 1, elapsed_time.count()};
    }
    std::priority_queue<std::pair<double, Steps>, std::vector<std::pair<double, Steps>>,PQCompare<double>> frontier;
    std::unordered_set<Steps> closed_set;
    std::unordered_map<Steps, Steps> from;
    std::unordered_map<Steps, double> g_score;
    g_score[start] = 0.0;
    std::unordered_map<Steps, double> f_score;
    if (heuristics == "blockedcarheuristic") {
        f_score[start] = blocked_car_heuristic(start);
    } else if (heuristics == "manhattandistance") {
        f_score[start] = manhattan_distance(start);
    }
    frontier.push({f_score[start], start});
    while (!frontier.empty()) {
        auto current = frontier.top().second;
        frontier.pop();
        states_explored++;
        std::system("clear");
        std::cout << current.get_board().to_string() << std::endl;
        if(current.is_goal()){
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_time = end_time - start_time;
            std::vector<Steps> path = get_path(from, current, start);
            return {path, static_cast<int>(path.size())-1, states_explored, elapsed_time.count()};
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
                if (heuristics == "blockedcarheuristic") {
                    f_score[next] = tentative_g_score + blocked_car_heuristic(next);
                } else if (heuristics == "manhattandistance") {
                    f_score[next] = tentative_g_score + manhattan_distance(next);
                }
                
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
    if (start.is_goal()) {
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end_time - start_time;
        std::vector<Steps> path = {start};
        return {path, 0, 1, elapsed_time.count()};
    }
    std::priority_queue<std::pair<double, Steps>, std::vector<std::pair<double, Steps>>, PQCompare<double>> frontier;
    std::unordered_set<Steps> visited;
    std::unordered_map<Steps, Steps> from;
    if (heuristics == "blockedcarheuristic") {
        frontier.push({blocked_car_heuristic(start), start});
    } else if (heuristics == "manhattandistance") {
        frontier.push({manhattan_distance(start), start});
    }
    while (!frontier.empty()) {
        auto current = frontier.top().second;
        frontier.pop();
        states_explored++;
        std::system("clear");
        std::cout << current.get_board().to_string() << std::endl;
        if (current.is_goal()) {
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_time = end_time - start_time;
            std::vector<Steps> path = get_path(from, current, start);
            return {path, static_cast<int>(path.size())-1, states_explored, elapsed_time.count()};
        }
        if (visited.find(current) != visited.end()) {
            continue;
        }
        visited.insert(current);
        for (const auto& next : current.get_possible_steps()) {
            if (visited.find(next) != visited.end()) {
                continue;
            }
            if (heuristics == "blockedcarheuristic") {
                frontier.push({blocked_car_heuristic(next), next});
            } else if (heuristics == "manhattandistance") {
                frontier.push({manhattan_distance(next), next});
            }
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
        std::system("clear");
        std::cout << current.get_board().to_string() << std::endl;
        if (current.is_goal()) {
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_time = end_time - start_time;
            std::vector<Steps> path = get_path(from, current, start);
            return {path, static_cast<int>(path.size())-1, states_explored, elapsed_time.count()};
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

Algorithms::Result IDA::solve(const Board& board) {
    auto start_time = std::chrono::high_resolution_clock::now();
    int states_explored = 0;
    Steps start(board);
    std::unordered_map<Steps, Steps> from;
    if (start.is_goal()) {
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end_time - start_time;
        return {{start}, 0, 1, elapsed_time.count()};
    }
    double bound = heuristic(start);
    
    while (true) {
        double next_bound = std::numeric_limits<double>::infinity();
        std::stack<std::pair<Steps, double>> dfs_stack;
        std::unordered_map<Steps, double> g_score;
        dfs_stack.push({start, 0.0});
        g_score[start] = 0.0;
        std::unordered_set<Steps> visited;
        while (!dfs_stack.empty()) {
            auto [current, cost] = dfs_stack.top();
            dfs_stack.pop();
            states_explored++;
            if (visited.find(current) != visited.end()) {
                continue;
            }
            visited.insert(current);
            if (current.is_goal()) {
                auto end_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed_time = end_time - start_time;
                std::vector<Steps> path;
                path.push_back(current);
                Steps trace = current;
                while (from.find(trace) != from.end()) {
                    trace = from[trace];
                    path.insert(path.begin(), trace);
                }
                return {path, path.size() - 1, states_explored, elapsed_time.count()};
            }
            double f_score = g_score[current] + heuristic(current);
            if (f_score > bound) {
                next_bound = std::min(next_bound, f_score);
                continue;
            }
            for (const auto& next : current.get_possible_steps()) {
                double new_cost = g_score[current] + 1.0;
                
                if (g_score.find(next) == g_score.end() || new_cost < g_score[next]) {
                    g_score[next] = new_cost;
                    from[next] = current;
                    dfs_stack.push({next, new_cost});
                }
            }
        }
        if (next_bound == std::numeric_limits<double>::infinity()) {
            // No solution exists
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_time = end_time - start_time;
            return {{}, 0, states_explored, elapsed_time.count()};
        }
        bound = next_bound;
    }
}