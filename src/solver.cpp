#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <string>

#include "configuration.hpp"

static std::vector<char> faces = { 'U', 'R', 'F', 'D', 'L', 'B' };

unsigned char parse_move(std::string str) {
    unsigned char move = 0xFF;
    for (size_t i = 0; i < faces.size(); ++i) {
        if (faces[i] == str[0]) {
            move = 3*i;
            break;
        }
    }
    
    if (move == 0xFF) {
        std::cerr << "Failed to parse move " << str << '\n';
        return 0xFF;
    }

    if (str.size() == 1) {
        return move;
    }
    if (str[1] == '2') {
        return move+2;
    }
    if (str[1] == '\'') {
        return move+1;
    }

    std::cerr << "Failed to parse move " << str << '\n';
    return 0xFF;
}


std::string read_move(unsigned char move) {
    std::string str;
    str += faces[move & 0x0F];
    unsigned char direction = move >> 4;
    if (direction == 2) {
        return str + '2';
    }
    if (direction == 1) {
        return str + '\'';
    }
    if (direction == 0) {
        return str;
    }
    std::cerr << "Failed to read move " << std::hex << move << '\n';
    return str;

}


std::vector <std::string> starting_moves = { "U'", "R", "F2", "B'", "U'", "D2" };


int main() {
    Configuration start = Configuration::solved;
    for (size_t i = 0; i < starting_moves.size(); ++i) {
        start = start.generate_from_move(Configuration::possible_moves[parse_move(starting_moves[i])]);
    }
    start.reset_moves();
    start.print();
    std::queue<Configuration> to_explore;  
    to_explore.push(start);

    bool solved = false;
    while (!solved) {
        Configuration current = to_explore.front();
        to_explore.pop();
        for (auto move: Configuration::possible_moves) {
            // moves on the same face than the last move are discarded
            auto moves = current.get_moves();
            if (moves.size() > 0 && (is_same_face(move, moves.back()) || is_opposite_face(move, moves.back()))) {
                continue;
            }

            Configuration next_conf = current.generate_from_move(move);
            if (next_conf == Configuration::solved) {
                solved = true;
                auto solving_moves = next_conf.get_moves();
                std::cout << "Solving moves:\n";
                for (size_t i = 0; i < solving_moves.size(); ++i) {
                    std::cout << read_move(solving_moves[i]) << ' ';
                }
                std::cout << '\n';
            }
            else if (next_conf.estimated_moves_number() <= 20) {
                // moves that will take more than 20 steps are discarded
                to_explore.push(next_conf);
            }            
        }
    }


    
    


    return 0;
}