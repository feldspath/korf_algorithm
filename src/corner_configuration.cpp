#include <vector>
#include <iostream>

#include "corner_configuration.hpp"

static const std::vector<unsigned short> corner_moves = {
    0x0123, 0x3740, 0x0451,
    0x4765, 0x1562, 0x2673
};

CornerConfiguration::CornerConfiguration(unsigned long _corners) : PieceConfiguration(_corners) {}

void CornerConfiguration::make_move(unsigned char move) {
    unsigned char face = move & 0x0F;
    unsigned char direction = move >> 4;
    unsigned short indices = corner_moves[face];

    if (direction < 2 && face != 0 && face != 3) {
        update_orientation(indices);
    }

    permut_pieces(indices, direction);
}

inline unsigned long increment_corner_rotation(unsigned long corners, unsigned char index, char increment) {
    int bit_position = 48 + index * 2;
    char value = (corners >> bit_position) & 3;
    unsigned long new_value = (value + 3 + increment) % 3;
    unsigned long mask = ~(3UL << bit_position);
    return (corners & mask) | (new_value << bit_position);
}

void CornerConfiguration::update_orientation(unsigned short indices) {
    unsigned long new_corners = pieces;
    char increment = 1;
    for (int i = 0; i < 4; ++i) {
        unsigned int index = get_piece_index(indices, i);
        unsigned int piece = get_piece_value(pieces, index);
        new_corners = increment_corner_rotation(new_corners, piece, increment);
        increment *= -1;
    }
    pieces = new_corners;
}

bool CornerConfiguration::eq(const CornerConfiguration& other) const {
    return pieces == other.pieces;
}

unsigned int CornerConfiguration::hash() const {
    std::vector<unsigned char> values(8);
    for (size_t i = 0; i < values.size(); ++i) {
        values[i] = get_piece_value(pieces, i);
    }
    int position_hash = 0;
    int fact = 1;
    for (size_t i = 1; i < values.size(); ++i) {
        fact *= i;
        position_hash += fact * order_index(values, i);
    } 

    int rotation_hash = 0;
    int power = 1;
    for (size_t i = 0; i < values.size()-1; ++i) {
        rotation_hash += power * ((pieces >> (48 + i * 2)) & 3);
        power *= 3;
    }

    return rotation_hash * fact * values.size() + position_hash;
}