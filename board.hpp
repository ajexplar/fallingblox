#include <iostream>
#include <vector>

class Board {
    private:
        std::vector< std::vector<int> > board;

    public:
        Board(int w = 12, int h = 20);

        /* Altering the board */
        void clearRow(size_t i);
        void swapRow(size_t i, size_t ix);

        /* Mutators & Accessors */
        size_t getHeight() { return board.size(); }
        size_t getWidth(size_t i) { return board.at(i).size(); }
        int atIndex(size_t i, size_t j) { return board.at(i).at(j); }
        void setIndex(size_t i, size_t j, int x) { board.at(i).at(j) = x; }

};
