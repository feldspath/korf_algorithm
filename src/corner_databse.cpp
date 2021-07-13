#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

#include "corner_configuration.hpp"
#include "configuration.hpp"

#define SIZE 88179840

std::vector<unsigned char> move_counts(SIZE / 2, 0xbb);

unsigned char get_move_count(unsigned int index) {
    int offset = index % 2;
    unsigned char value = move_counts[index / 2];
    return (value >> offset * 4) & 0xF;
}

void set_move_count(unsigned char value, unsigned int index) {
    int offset = index % 2;
    unsigned char old_value = move_counts[index / 2];
    move_counts[index/2] = (old_value & ~(0xF << offset * 4)) | ((value & 0xF) << offset * 4);
}

struct Corner {
    CornerConfiguration conf;
    unsigned char move_count;
};

int main() {
    Corner start;
    start.conf = CornerConfiguration(0x76543210);
    start.move_count = 0;
    std::queue<Corner> to_explore;  
    to_explore.push(start);
    set_move_count(0, 0);

    unsigned int count = 0;
    unsigned int print_count = 0;
    while (to_explore.size() > 0) {
        Corner current = to_explore.front();
        to_explore.pop();
        for (auto move: Configuration::possible_moves) {
            CornerConfiguration next_conf(current.conf);
            next_conf.make_move(move);
            unsigned int hash = next_conf.hash();
            if (get_move_count(hash) == 0xb) {
                set_move_count(current.move_count+1, hash);
                count++;
                if (current.move_count == 9) continue;
                Corner next;
                next.conf = next_conf;
                next.move_count = current.move_count+1;
                to_explore.push(next);
            }
        }
        if (print_count++ % 2000 == 0) {
            std::cout << "Move:" << (int)current.move_count << ' ' << (float)count / SIZE * 100 << "%\n";
        }
            
    }

    std::ofstream file("corners.bin", std::ios::out | std::ios::binary);
    if(!file) {
      std::cout << "Cannot open file!" << '\n';
      return 1;
    }

    file.write((char*)move_counts.data(), SIZE);
    file.close();
    return 0;
}