#include "piece.h"
#include "board.h"

namespace tetris
{

piece :: piece()
{
    this->x = 0;
    this->y = 0;
    this->type = TYPE_NULL;
    this->rotation = ROTATION_UP;
};

bool piece :: is_colliding(board& board)
{
    for (int i = 0; i < 4; ++i) {
        int cell_x = PIECE_LUT[this->type][this->rotation][i][0] + this->x;
        int cell_y = PIECE_LUT[this->type][this->rotation][i][1] + this->y;
        if (board.is_occupied(cell_x, cell_y)) {
            return true;
        }
    }
    return false;
};

bool piece :: move_right(board& board)
{
    if (!board.is_colliding(this->x + 1, this->y, this->type, this->rotation)) {
        this->x += 1;
        return true;
    }
    return false;
};

bool piece :: move_left(board& board)
{
    if (!board.is_colliding(this->x - 1, this->y, this->type, this->rotation)) {
        this->x -= 1;
        return true;
    }
    return false;
};

bool piece :: move_cw(board& board, int& index)
{
    if (this->type == TYPE_O) {
        return true;
    }
    int srs_index = std::min(this->type, 1);
    int next_rotation = this->rotation + 1;
    if (next_rotation == 4) {
        next_rotation = 0;
    }
    for (int i = 0; i < 5; ++i) {
        int8_t offset_x = SRS_LUT[srs_index][this->rotation][i][0] - SRS_LUT[srs_index][next_rotation][i][0];
        int8_t offset_y = SRS_LUT[srs_index][this->rotation][i][1] - SRS_LUT[srs_index][next_rotation][i][1];
        if (!board.is_colliding(this->x + offset_x, this->y + offset_y, this->type, next_rotation)) {
            this->x += offset_x;
            this->y += offset_y;
            this->rotation = next_rotation;
            index = i;
            return true;
        }
    }
    return false;
};

bool piece :: move_ccw(board& board, int& index)
{
    if (this->type == TYPE_O) {
        return true;
    }
    int srs_index = std::min(this->type, 1);
    int next_rotation = this->rotation - 1;
    if (next_rotation == -1) {
        next_rotation = 3;
    }
    for (int i = 0; i < 5; ++i) {
        int8_t offset_x = SRS_LUT[srs_index][this->rotation][i][0] - SRS_LUT[srs_index][next_rotation][i][0];
        int8_t offset_y = SRS_LUT[srs_index][this->rotation][i][1] - SRS_LUT[srs_index][next_rotation][i][1];
        if (!board.is_colliding(this->x + offset_x, this->y + offset_y, this->type, next_rotation)) {
            this->x += offset_x;
            this->y += offset_y;
            this->rotation = next_rotation;
            index = i;
            return true;
        }
    }
    return false;
};

bool piece :: move_down(board& board)
{
    if (!board.is_colliding(this->x, this->y - 1, this->type, this->rotation)) {
        this->y -= 1;
        return true;
    }
    return false;
};

bool piece :: move_drop(board& board)
{
    if (!this->move_down(board)) {
        return false;
    }
    while (true)
    {
        bool moved_down = this->move_down(board);
        if (!moved_down) {
            break; 
        }
    }
    return true;
};

bool piece :: place(board& board)
{
    if (this->is_colliding(board)) {
        return false;
    }
    for (int i = 0; i < 4; ++i) {
        int cell_x = PIECE_LUT[this->type][this->rotation][i][0] + this->x;
        int cell_y = PIECE_LUT[this->type][this->rotation][i][1] + this->y;
        board.set(cell_x, cell_y, this->type);
    }
    return true;
};

int piece :: get_tspin(board& board, int index)
{
    if (this->type != TYPE_T) {
        return TSPIN_NULL;
    }

    int corner = 0;
    corner += board.is_occupied(this->x + 1, this->y + 1);
    corner += board.is_occupied(this->x - 1, this->y - 1);
    corner += board.is_occupied(this->x + 1, this->y - 1);
    corner += board.is_occupied(this->x - 1, this->y + 1);
    if (corner < 3) {
        return TSPIN_NULL;
    }

    int corner_point = 0;
    switch (this->rotation)
    {
    case ROTATION_UP:
        corner_point += board.is_occupied(this->x + 1, this->y + 1);
        corner_point += board.is_occupied(this->x - 1, this->y + 1);
        break;
    case ROTATION_RIGHT:
        corner_point += board.is_occupied(this->x + 1, this->y + 1);
        corner_point += board.is_occupied(this->x + 1, this->y - 1);
        break;
    case ROTATION_DOWN:
        corner_point += board.is_occupied(this->x + 1, this->y - 1);
        corner_point += board.is_occupied(this->x - 1, this->y - 1);
        break;
    case ROTATION_LEFT:
        corner_point += board.is_occupied(this->x - 1, this->y + 1);
        corner_point += board.is_occupied(this->x - 1, this->y - 1);
        break;
    }

    if (corner_point == 2) {
        return TSPIN_FULL;
    }
    
    if (index == 4) {
        return TSPIN_FULL;
    }

    return TSPIN_MINI;
};

bool piece :: operator == (piece& other)
{
    int equal_sum = 0;
    equal_sum += this->x == other.x;
    equal_sum += this->y == other.y;
    equal_sum += this->type == other.type;
    equal_sum += this->rotation == other.rotation;
    return equal_sum == 4;
};

};