#pragma once

#include "piece_configuration.hpp"

class EdgeConfiguration : public PieceConfiguration {

public:
    EdgeConfiguration() {}
    EdgeConfiguration(unsigned long _edges);
    void make_move(unsigned char move);
    void update_orientation(unsigned short indices);
    bool eq(const EdgeConfiguration& other) const;
};