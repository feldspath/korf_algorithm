#pragma once

#include <vector>

#include "corner_configuration.hpp"
#include "edge_configuration.hpp"

class Configuration {

    CornerConfiguration corners;
    EdgeConfiguration edges;

    std::vector<unsigned char> moves;

public:

    static const Configuration solved;
    static const std::vector<unsigned char> possible_moves;

    Configuration(unsigned long _corners, unsigned long _edges, const std::vector<unsigned char>& _moves);
    Configuration generate_from_move(unsigned char move) const;
    unsigned char estimated_moves_number() const;

    const std::vector<unsigned char>& get_moves() const;
    void reset_moves();
    bool eq(const Configuration& other) const;
    void print() const;
};

bool operator==(const Configuration& a, const Configuration& b);
bool operator>(const Configuration& a, const Configuration& b);

bool is_same_face(const unsigned char& current, const unsigned char& previous);
bool is_opposite_face(const unsigned char& current, const unsigned char& previous);

unsigned char parse_move(std::string str);
std::string read_move(unsigned char move);

void initialize_move_counts_vectors();