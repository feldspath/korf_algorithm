#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <string>

#include "configuration.hpp"

std::vector <std::string> parse_scramble(std::string str) {
    std::vector <std::string> moves;
    std::string move = "";
    for (size_t i = 0; i < str.size(); i++) {
        if (isalpha(str[i])) {
            moves.push_back(move);
            move = "";
        }
        if (str[i] != ' ') {
            move += str[i];
        }
    }
    if (!move.empty()) {
        moves.push_back(move);
        move = "";
    }
    return moves;
}

int main() {
    std::vector <std::string> starting_moves = parse_scramble("L2 D2 F2 U L2 D' L2 U F2 D'");
    Configuration start = Configuration::solved;
    for (size_t i = 0; i < starting_moves.size(); ++i) {
        start = start.generate_from_move(Configuration::possible_moves[parse_move(starting_moves[i])]);
    }
    start.reset_moves();
    start.print();
    std::priority_queue<Configuration, std::vector<Configuration>, std::greater<Configuration>> to_explore;  
    to_explore.push(start);

    initialize_move_counts_vectors();

    bool solved = false;
    int count = 0;
    while (!solved && !to_explore.empty()) {
        Configuration current = to_explore.top();
        to_explore.pop();
        if (current.get_moves().size() == 2) {
                ++count;
                std::cout << (float)count / 279 * 100 << "%\n";
        }
        for (auto move: Configuration::possible_moves) {
            // moves on the same face than the last move are discarded
            auto moves = current.get_moves();
            if (moves.size() > 0 && (is_same_face(move, moves.back()) || is_opposite_face(move, moves.back()))) {
                continue;
            }

            Configuration next_conf = current.generate_from_move(move);
            if (next_conf == Configuration::solved) {
                solved = true;
                next_conf.print();
            }
            else if (next_conf.estimated_moves_number() <= 18) {
                // moves that will take more than 20 steps are discarded
                to_explore.push(next_conf);

            }            
        }
    }

    return 0;
}