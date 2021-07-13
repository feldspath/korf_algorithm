#include <vector>
#include <iostream>
#include <fstream>

#include "configuration.hpp"
#include "corner_configuration.hpp"
#include "edge_configuration.hpp"

static std::vector<unsigned char> corners_move_counts(88179840);

static unsigned char get_move_count(unsigned int index) {
    int offset = index % 2;
    unsigned char value = corners_move_counts[index / 2];
    return (value >> offset * 4) & 0xF;
}

void initialize_move_counts_vectors() {
    std::ifstream file("../corners.bin", std::ios::out | std::ios::binary);
    if(!file) {
      std::cout << "Cannot open file!" << '\n';
      return;
    }
    file.read((char *)corners_move_counts.data(), corners_move_counts.size());
    file.close();
}

Configuration::Configuration(unsigned long _corners, unsigned long _edges, const std::vector<unsigned char>& _moves) {
    corners = CornerConfiguration(_corners);
    edges = EdgeConfiguration(_edges);
    moves = _moves;
}

unsigned char Configuration::estimated_moves_number() const {
    return moves.size() + get_move_count(corners.hash());
}

const std::vector<unsigned char>& Configuration::get_moves() const {
    return moves;
}

void Configuration::reset_moves() {
    moves.resize(0);
}

const Configuration Configuration::solved(0x0000000076543210UL, 0x0000BA9876543210UL, std::vector<unsigned char>());

const std::vector<unsigned char> Configuration::possible_moves = { 
    // U face
    0x00, 0x10, 0x20,
    // R face
    0x01, 0x11, 0x21,
    // F face
    0x02, 0x12, 0x22,
    // D face
    0x03, 0x13, 0x23,
    // L face
    0x04, 0x14, 0x24,
    // B face
    0x05, 0x15, 0x25,
};

bool Configuration::eq(const Configuration& other) const {
    return corners.eq(other.corners) && edges.eq(other.edges);
}

Configuration Configuration::generate_from_move(unsigned char move) const {
    Configuration new_configuration(*this);
    new_configuration.corners.make_move(move);
    new_configuration.edges.make_move(move);
    new_configuration.moves.push_back(move);
    return new_configuration;
}

void Configuration::print() const {
    std::cout << "Corners: ";
    corners.print();
    std::cout << '\n';
    std::cout << "Edges: ";
    edges.print();
    std::cout << '\n';
    std::cout << "Moves: ";
    for (size_t i = 0; i < moves.size(); ++i) {
        std::cout << read_move(moves[i]) << ' ';
    }
    std::cout << '\n';
    std::cout << "Corner Hash: " << std::dec << corners.hash() << '\n';
    std::cout << "Edge Hash: " << std::dec << edges.hash() << '\n';
}

bool operator==(const Configuration& a, const Configuration& b) {
    return a.eq(b);
}

bool operator>(const Configuration& a, const Configuration& b) {
    return a.estimated_moves_number() > b.estimated_moves_number();
}

bool is_same_face(const unsigned char& current, const unsigned char& previous) {
    unsigned char face_mask = 0x0F;
    return (current & face_mask) == (previous & face_mask);
}

bool is_opposite_face(const unsigned char& current, const unsigned char& previous) {
    unsigned char face_mask = 0x0F;
    unsigned char current_value = current & face_mask;
    unsigned char previous_value = previous & face_mask;
    return (previous_value > current_value) && (previous_value - current_value == 3);
}

static std::vector<char> face_chars = { 'U', 'R', 'F', 'D', 'L', 'B' };
static std::vector<char> direction_chars = { '\0' , '\'', '2' };

unsigned char parse_move(std::string str) {
    unsigned char move = 0xFF;
    for (size_t i = 0; i < face_chars.size(); ++i) {
        if (face_chars[i] == str[0]) {
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
    str += face_chars[move & 0x0F];
    unsigned char direction = move >> 4;
    return str + direction_chars[direction];
}