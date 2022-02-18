#include "player.h"

player :: player()
{
    this->reset();
};

void player :: set_bag(std::vector<int> bag)
{
    this->bag = bag;
    this->bag_index = 0;
};

void player :: set_enemy(player* other)
{
    this->enemy = other;
};

void player :: reset()
{
    this->type = PLAYER_HUMAN;

    this->board = tetris::board();
    this->current = TYPE_NULL;
    this->hold = TYPE_NULL;
    this->b2b = 0;
    this->combo = 0;
    this->queue = std::vector<int>();
    this->bag = std::vector<int>();
    this->bag_index = 0;

    this->piece = tetris::piece();
    this->piece_locked = false;
    this->piece_holded = false;
    this->piece_last_move = MOVE_NULL;
    this->piece_rotate_index = -1;
    this->piece_on_floor_counter = 0;
    this->piece_drop_counter = 0;

    this->enemy = nullptr;
    this->garbage_incomming = 0;

    this->gameover = false;

    this->delay_move = delayer();
    this->delay_clear = delayer();
    this->rule = setting();
    this->stat = statistic();

    this->sprite_board = nullptr;
    this->sprite_block = nullptr;
    this->sprite_ghost = nullptr;

    this->decal_board = nullptr;
    this->decal_block = nullptr;
    this->decal_ghost = nullptr;
};

void player :: init(int type, setting setting, std::vector<int> bag, player* enemy)
{
    this->reset();
    this->set_bag(bag);
    this->set_enemy(enemy);

    this->type = type;
    this->rule = setting;

    this->load_queue();
    this->load_current();

    this->sprite_board = new olc::Sprite("./content/board.png");
    this->sprite_block = new olc::Sprite("./content/block.png");
    this->sprite_ghost = new olc::Sprite("./content/ghost.png");
    
    this->decal_board = new olc::Decal(this->sprite_board);
    this->decal_block = new olc::Decal(this->sprite_block);
    this->decal_ghost = new olc::Decal(this->sprite_ghost);
};

void player :: update(olc::PixelGameEngine* pge)
{
    if (this->gameover) {
        return;
    }

    this->delay_clear.update();
    if (this->delay_clear.is_delaying()) {
        return;
    }
    if (this->delay_clear.is_just_end()) {
        int line = this->board.get_clear(this->piece);
        int attack = this->calc_garbage();

        this->advance_state();
        this->load_current();

        if (this->rule.garbage.countering) {
            if (line == 0) {
                this->garbage_place();
            }
            else {
                if (this->garbage_incomming > attack) {
                    this->garbage_incomming = this->garbage_incomming - attack;
                    if (!this->rule.garbage.blocking) {
                        this->garbage_place();
                    }
                }
                else {
                    this->garbage_send(attack - this->garbage_incomming);
                    this->garbage_incomming = 0;
                }
            }
        }
        else {
            this->garbage_place();
            this->garbage_send(attack);
        }

        // TODO: Update statistic!
    }

    if (this->type == PLAYER_HUMAN) {
        // hold
        if (pge->GetKey((olc::Key)this->rule.input.hold).bPressed) {
            this->do_hold();
        }

        // rotate
        if (pge->GetKey((olc::Key)this->rule.input.cw).bPressed) {
            this->do_cw();
        }
        if (pge->GetKey((olc::Key)this->rule.input.ccw).bPressed) {
            this->do_ccw();
        }

        // harddrop
        if (pge->GetKey((olc::Key)this->rule.input.drop).bPressed) {
            this->do_drop();
        }

        this->delay_move.update();
        if (!this->delay_move.is_delaying()) {
            // das & arr
            if (pge->GetKey((olc::Key)this->rule.input.right).bPressed) {
                this->do_right();
                this->delay_move.delay(this->rule.delay.das);
            }
            else if (pge->GetKey((olc::Key)this->rule.input.right).bHeld) {
                this->do_right();
                this->delay_move.delay(this->rule.delay.arr);
            }
            
            if (pge->GetKey((olc::Key)this->rule.input.left).bPressed) {
                this->do_left();
                this->delay_move.delay(this->rule.delay.das);
            }
            else if (pge->GetKey((olc::Key)this->rule.input.left).bHeld) {
                this->do_left();
                this->delay_move.delay(this->rule.delay.arr);
            }

            // softdrop
            if (pge->GetKey((olc::Key)this->rule.input.down).bHeld) {
                this->do_down();
                this->delay_move.delay(this->rule.delay.softdrop);
            }
        }

        // piece falling
        if (!pge->GetKey((olc::Key)this->rule.input.down).bHeld) {
            this->piece_drop_counter = this->piece_drop_counter + 1;
            if (this->piece_drop_counter > this->rule.delay.fall) {
                this->piece_drop_counter = 0;
                this->piece.move_down(this->board);
            }
        }
        else {
            this->piece_drop_counter = 0;
        }

        // lock on floor
        if (this->board.is_colliding(this->piece.x, this->piece.y - 1, this->piece.type, this->piece.rotation)) {
            this->piece_on_floor_counter = this->piece_on_floor_counter + 1;
        }
        else {
            this->piece_on_floor_counter = 0;
        }
        if (this->piece_on_floor_counter > this->rule.delay.on_floor_lock) {
            this->do_drop();
        }
    }

    if (this->piece_locked) {
        if (this->piece.y >= 20) {
            this->gameover = true;
            return;
        }
        
        tetris::board board_copy = this->board;
        this->piece.place(board_copy);
        int line = board_copy.get_clear();
        board_copy.clear();

        int clear_delay = 0;
        if (board_copy.is_perfect()) {
            clear_delay = this->rule.delay.clear_perfect;
        }
        else {
            switch (line)
            {
            case 1:
                clear_delay = this->rule.delay.clear_1;
                break;
            case 2:
                clear_delay = this->rule.delay.clear_2;
                break;
            case 3:
                clear_delay = this->rule.delay.clear_3;
                break;
            case 4:
                clear_delay = this->rule.delay.clear_4;
                break;
            default:
                break;
            }
        }
        clear_delay = clear_delay + this->rule.delay.spawn;

        this->delay_clear.delay(clear_delay);
    }
};

void player :: render(olc::PixelGameEngine* pge, int x, int y)
{
    // Set blend mode
    pge->SetPixelMode(olc::Pixel::ALPHA);

    // Set block size
    int block_size = 40;

    // Draw garbage warning
    int garbage_length = std::min(this->garbage_incomming, 20);
    pge->FillRectDecal(
        {
            float(x - block_size),
            float(y + (20 - garbage_length) * block_size)
        },
        {
            float(block_size),
            float(garbage_length * block_size)
        },
        olc::RED
    );
    pge->FillRectDecal(
        {
            float(x + 10 * block_size),
            float(y + (20 - garbage_length) * block_size)
        },
        {
            float(block_size),
            float(garbage_length * block_size)
        },
        olc::RED
    );

    // Draw board frame
    pge->DrawDecal({float(x - block_size * 5 - 8), float(y - 8)}, this->decal_board);

    // Draw board
    for (int y_cell = 0; y_cell < 20; ++y_cell) {
        for (int x_cell = 0; x_cell < 10; ++x_cell) {
            if (!this->board.is_occupied(x_cell, y_cell)) {
                continue;
            }
            pge->DrawDecal(
                {
                    float(x + x_cell * block_size),
                    float(y + (19 - y_cell) * block_size)
                },
                this->decal_block,
                {
                    1.0f,
                    1.0f
                },
                this->rule.color.map[std::clamp(this->board.get(x_cell, y_cell), 0, TYPE_COUNT - 1)]
            );
        }
    }

    // Draw piece
    for (int i = 0; i < 4; ++i) {
        int piece_cell_x = PIECE_LUT[this->piece.type][this->piece.rotation][i][0] + this->piece.x;
        int piece_cell_y = PIECE_LUT[this->piece.type][this->piece.rotation][i][1] + this->piece.y;

        if (piece_cell_y > 19) {
            continue;
        }

        pge->DrawDecal(
            {
                float(x + piece_cell_x * block_size),
                float(y + (19 - piece_cell_y) * block_size)
            },
            this->decal_block,
            {
                1.0f,
                1.0f
            },
            this->rule.color.map[std::clamp(this->piece.type, 0, TYPE_COUNT - 1)]
        );
    }

    // Draw ghost
    tetris::piece ghost = this->piece;
    ghost.move_drop(this->board);
    for (int i = 0; i < 4; ++i) {
        int ghost_cell_x = PIECE_LUT[ghost.type][ghost.rotation][i][0] + ghost.x;
        int ghost_cell_y = PIECE_LUT[ghost.type][ghost.rotation][i][1] + ghost.y;

        if (ghost_cell_y > 19) {
            continue;
        }

        pge->DrawDecal(
            {
                float(x + ghost_cell_x * block_size),
                float(y + (19 - ghost_cell_y) * block_size)
            },
            this->decal_ghost,
            {
                1.0f,
                1.0f
            },
            {
                this->rule.color.map[std::clamp(ghost.type, 0, TYPE_COUNT - 1)].r,
                this->rule.color.map[std::clamp(ghost.type, 0, TYPE_COUNT - 1)].g,
                this->rule.color.map[std::clamp(ghost.type, 0, TYPE_COUNT - 1)].b,
                127
            }
        );
    }
};

void player :: end()
{
    delete this->decal_board;
    delete this->sprite_board;
    delete this->sprite_ghost;
};

void player :: load_queue()
{
    int queue_size = int(this->queue.size());
    for (int i = 0; i < this->rule.player.preview - queue_size; ++i) {
        this->queue.push_back(this->bag[this->bag_index]);
        this->bag_index += 1;
        this->bag_index = this->bag_index % int(this->bag.size());
    }
};

void player :: load_current()
{
    this->current = this->queue[0];
    this->queue.erase(this->queue.begin());
    this->load_queue();

    this->piece = tetris::piece();
    this->piece.x = 4;
    this->piece.y = 19;
    this->piece.type = this->current;
    this->piece.rotation = tetris::ROTATION_UP;

    if (this->board.is_colliding(4, 19, this->current, tetris::ROTATION_UP)) {
        this->piece.y = 20;
    }

    this->piece_locked = false;
    this->piece_holded = false;
    this->piece_last_move = MOVE_NULL;
    this->piece_rotate_index = -1;
};

void player :: advance_state()
{
    int line = this->board.get_clear();
    int tspin = this->piece.get_tspin(this->board, this->piece_rotate_index);

    this->piece.place(this->board);
    this->board.clear();

    if (line > 0) {
        if (tspin != TSPIN_NULL) {
            this->b2b = this->b2b + 1;
        }
        else {
            if (line == 4) {
                this->b2b = this->b2b + 1;
            }
            else {
                this->b2b = 0;
            }
        }
        this->combo = this->combo + 1;
    }
    else {
        this->combo = 0;
    }
};

int player :: calc_garbage()
{
    int result = 0;

    int line = this->board.get_clear(this->piece);
    int tspin = this->piece.get_tspin(this->board, this->piece_rotate_index);

    if (line == 0) {
        return result;
    }

    if (tspin != TSPIN_NULL) {
        if (tspin == TSPIN_FULL) {
            line = std::min(line, 3);
            switch (line)
            {
            case 1:
                result = this->rule.attack.tspin_1;
                break;
            case 2:
                result = this->rule.attack.tspin_2;
                break;
            case 3:
                result = this->rule.attack.tspin_3;
                break;
            }
        }
        else {
            line = std::min(line, 2);
            switch (line)
            {
            case 1:
                result = this->rule.attack.tspin_mini_1;
                break;
            case 2:
                result = this->rule.attack.tspin_mini_2;
                break;
            }
        }
    }
    else {
        line = std::min(line, 4);
        switch (line)
        {
        case 1:
            result = this->rule.attack.clear_1;
            break;
        case 2:
            result = this->rule.attack.clear_2;
            break;
        case 3:
            result = this->rule.attack.clear_3;
            break;
        case 4:
            result = this->rule.attack.clear_4;
            break;
        }
    }

    if (this->b2b > 1) {
        result = result + this->rule.attack.b2b_bonus;
    }

    int real_combo = std::max(this->combo - 1, 0);
    result = result + this->rule.attack.combo[std::min(real_combo, COMBO_TABLE_SIZE - 1)];

    if (this->board.is_perfect()) {
        result = result + this->rule.attack.perfect;
    }

    return result;
};

void player :: garbage_place()
{
    if (this->garbage_incomming == 0) {
        return;
    } 

    this->garbage_incomming = std::min(this->garbage_incomming, 22);

    for (int y = 39; y >= this->garbage_incomming; --y) {
        memcpy(this->board.data[y], this->board.data[y - this->garbage_incomming], 10 * sizeof(int));
    }

    for (int y = 0; y < this->garbage_incomming; ++y) {
        memset(this->board.data[y], TYPE_GARBAGE, 10 * sizeof(int));
    }

    int hole_position = rand() % 10;
    for (int y = 0; y < this->garbage_incomming; ++y) {
        this->board.set(hole_position, y, TYPE_NULL);
        if ((rand() % 100) < this->rule.garbage.messiness_percent) {
            hole_position = rand() % 10;
        }
    }

    this->garbage_incomming = 0;
}

void player :: garbage_send(int line)
{
    if (this->enemy == nullptr) {
        return;
    }
    this->enemy->garbage_incomming += line;
};

void player :: do_hold()
{
    if (this->piece_holded) {
        return;
    }

    if (this->hold == tetris::TYPE_NULL) {
        this->hold = this->current;
        this->load_current();
    }
    else {
        int value = this->hold;
        this->hold = this->current;
        this->current = value;

        this->piece.x = 4;
        this->piece.y = 19;
        this->piece.type = this->current;
        this->piece.rotation = tetris::ROTATION_UP;

        if (this->board.is_colliding(4, 19, this->current, tetris::ROTATION_UP)) {
            this->piece.y = 20;
        }

        this->piece_locked = false;
        this->piece_last_move = MOVE_NULL;
        this->piece_rotate_index = -1;
    }

    this->piece_holded = true;
};

void player :: do_right()
{
    bool success = this->piece.move_right(this->board);
    if (success) {
        this->piece_last_move = MOVE_RIGHT;
        this->piece_rotate_index = -1;
        this->piece_on_floor_counter = 0;
    }
};

void player :: do_left()
{
    bool success = this->piece.move_left(this->board);
    if (success) {
        this->piece_last_move = MOVE_LEFT;
        this->piece_rotate_index = -1;
        this->piece_on_floor_counter = 0;
    }
};

void player :: do_cw()
{
    bool success = this->piece.move_cw(this->board, this->piece_rotate_index);
    if (success) {
        this->piece_last_move = MOVE_CW;
        this->piece_on_floor_counter = 0;
    }
};

void player :: do_ccw()
{
    bool success = this->piece.move_ccw(this->board, this->piece_rotate_index);
    if (success) {
        this->piece_last_move = MOVE_CCW;
        this->piece_on_floor_counter = 0;
    }
};

void player :: do_down()
{
    this->piece.move_down(this->board);
    this->piece_last_move = MOVE_DOWN;
    this->piece_rotate_index = -1;
};

void player :: do_drop()
{
    this->piece.move_drop(this->board);
    this->piece_last_move = MOVE_NULL;
    this->piece_rotate_index = -1;
    this->piece_locked = true;
};