#include "piece.h"
#include "board.h"

namespace tetris
{

board :: board()
{
    this->reset();
};

void board :: reset()
{
    for (int y = 0; y < 40; ++y) {
        for (int x = 0; x < 10; ++x) {
            this->set(x, y, TYPE_NULL);
        }
    }
};

int board :: get(int x, int y)
{
    return this->data[y][x];
};

int board :: get_clear()
{
    int clear = 0;
    for (int y = 0; y < 40; ++y) {
        int row_sum = 0;
        for (int x = 0; x < 10; ++x) {
            row_sum += this->is_occupied(x, y);
        }
        clear += row_sum == 10;
    }
    return clear;
};

int board :: get_clear(piece& piece)
{
    board copy = *this;
    piece.place(copy);
    return copy.get_clear();
};

int board :: get_tspin(piece& piece, int index)
{
    return piece.get_tspin(*this, index);
};

int board :: set(int x, int y, int type)
{
    this->data[y][x] = type;
    return type;
};

bool board :: is_occupied(int x, int y)
{
    if (x < 0 || x > 9 || y < 0 || y > 39) {
        return true;
    }
    return this->get(x, y) != TYPE_NULL;
};

bool board :: is_colliding(piece& piece)
{
    return this->is_colliding(piece.x, piece.y, piece.type, piece.rotation);
};

bool board :: is_colliding(int x, int y, int type, int rotation)
{
    for (int i = 0; i < 4; ++i) {
        int cell_x = PIECE_LUT[type][rotation][i][0] + x;
        int cell_y = PIECE_LUT[type][rotation][i][1] + y;
        if (this->is_occupied(cell_x, cell_y)) {
            return true;
        }
    }
    return false;
};

bool board :: is_perfect()
{
    for (int y = 0; y < 40; ++y) {
        for (int x = 0; x < 10; ++x) {
            if (this->is_occupied(x, y)) {
                return false;
            }
        }
    }
    return true;
};

bool board :: clear()
{
    std::vector<int> full;
    full.clear();
    for (int y = 0; y < 40; ++y) {
        int row_sum = 0;
        for (int x = 0; x < 10; ++x) {
            row_sum += this->is_occupied(x, y);
        }
        if (row_sum == 10) {
            full.push_back(y);
        }
    }
    if (full.empty()) {
        return false;
    }
    std::sort(full.begin(), full.end(), std::greater<int>());
    for (int i = 0; i < int(full.size()); ++i) {
        for (int y = full[i]; y < 39; ++y) {
            for (int x = 0; x < 10; ++x) {
                this->set(x, y, this->get(x, y + 1));
            }
        }
        for (int x = 0; x < 10; ++x) {
            this->set(x, 39, TYPE_NULL);
        }
    }
    return true;
};

};