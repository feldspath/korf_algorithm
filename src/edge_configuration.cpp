#include <vector>

#include "edge_configuration.hpp"
#include "piece_configuration.hpp"

static const std::vector<unsigned short> edge_moves = {
    0x0123, 0x37B4, 0x0485,
    0x8BA9, 0x1596, 0x26A7
};

EdgeConfiguration::EdgeConfiguration(unsigned long _edges) : PieceConfiguration(_edges) {}

void EdgeConfiguration::make_move(unsigned char move) {
    unsigned char face = move & 0x0F;
    unsigned char direction = move >> 4;
    unsigned short indices = edge_moves[face];

    if (direction < 2 && (face == 2 || face == 5)) {
        update_orientation(indices);
    }

    permut_pieces(indices, direction);
}

void EdgeConfiguration::update_orientation(unsigned short indices) {
    unsigned long new_edges = pieces;
    for (int i = 0; i < 4; ++i) {
        unsigned int index = get_piece_index(indices, i);
        unsigned int piece = get_piece_value(pieces, index);
        new_edges ^= (1UL << (48 + piece)); 
    }
    pieces = new_edges;
}

bool EdgeConfiguration::eq(const EdgeConfiguration& other) const {
    return pieces == other.pieces;
}

unsigned int EdgeConfiguration::hash() const {
    std::vector<unsigned char> values(12);
    for (size_t i = 0; i < values.size(); ++i) {
        values[i] = get_piece_value(pieces, i);
    }
    int position_hash = 0;
    int fact = 1;
    for (size_t i = 1; i < values.size(); ++i) {
        fact *= i;
        position_hash += fact * order_index(values, i);
    }

    int rotation_hash = pieces >> 48;

    return rotation_hash * fact * values.size() + position_hash;
}