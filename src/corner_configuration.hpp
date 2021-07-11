#pragma once

#include "piece_configuration.hpp"

class CornerConfiguration : public PieceConfiguration {

public:
    CornerConfiguration() {}
    CornerConfiguration(unsigned long _corners);
    void make_move(unsigned char move);
    void update_orientation(unsigned short indices);
    bool eq(const CornerConfiguration& other) const;
};