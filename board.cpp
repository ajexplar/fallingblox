#include "board.hpp"

#include <algorithm>

Board::Board(int w, int h) {
    board.resize(h, std::vector<int>(w, 0));
}

void Board::clearRow(size_t i) {
    for(size_t j = 0; j < board.at(i).size(); ++j) {
        board.at(i).at(j) = 0;
    }
}

void Board::swapRow(size_t i, size_t ix) {
    for(size_t j = 0; j < board.at(i).size(); ++j) {
        int temp = board.at(i).at(j);
        board.at(i).at(j) = board.at(ix).at(j);
        board.at(ix).at(j) = temp;
    }
}
