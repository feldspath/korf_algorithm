#pragma once

#include <vector>

class PieceConfiguration {
protected:
    // From right to left:
    // 6 bytes: position
    // 2 bytes: orientation
    unsigned long pieces;

public:
    PieceConfiguration() {}
    PieceConfiguration(unsigned long _pieces);
    // Move functions
    virtual void make_move(unsigned char move) = 0;
    void permut_pieces(unsigned short indices, unsigned char direction);
    virtual void update_orientation(unsigned short indices) = 0;

    // Utility
    void print() const;
};

inline unsigned int get_piece_index(unsigned short indices, int count) {
    return (indices >> (3 - count) * 4) & 0x0F;
}

inline unsigned int get_piece_value(unsigned long pieces, int index) {
    return (pieces >> index * 4) & 0x0F;
}

inline unsigned long set_piece_value(unsigned long pieces, int index, unsigned int value) {
    unsigned long clear_mask = ~(0xFUL << index * 4);
    return (pieces & clear_mask) | ((value & 0xFUL) << index * 4);
}