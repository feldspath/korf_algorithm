#include <vector>
#include <iostream>

#include "piece_configuration.hpp"

PieceConfiguration::PieceConfiguration(unsigned long _pieces) : pieces(_pieces) {}

void PieceConfiguration::permut_pieces(unsigned short indices, unsigned char direction) {
    std::vector<unsigned short> start;
    std::vector<unsigned short> end;
    char increment = direction;
    if (direction < 2) {
        increment = (direction == 0 ? 1: -1);
    }
    for (int i = 0; i < 4; ++i) {
        start.push_back(get_piece_index(indices, i));
        end.push_back(get_piece_index(indices, (i+increment+4)%4));
    }
    
    unsigned long new_pieces = pieces;
    for (int i = 0; i < 4; ++i) {
        unsigned int value = get_piece_value(pieces, start[i]);
        new_pieces = set_piece_value(new_pieces, end[i], value);
    }
    pieces = new_pieces;
}

void PieceConfiguration::print() const {
    std::cout << std::hex << pieces;
}