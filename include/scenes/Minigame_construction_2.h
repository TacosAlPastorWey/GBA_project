#ifndef MINIGAME_CONSTRUCTION_2_H
#define MINIGAME_CONSTRUCTION_2_H

#include "bn_memory.h"
#include "bn_keypad.h"
#include "bn_vector.h"
#include "bn_math.h"
#include "bn_array.h"
#include "bn_string.h"
#include "bn_unique_ptr.h"

#include "bn_sound_items.h"

#include "bn_regular_bg_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_cell.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_regular_bg_tiles_items_bg_pipes_tiles.h"
#include "bn_regular_bg_items_bg_pipes.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_items_border_16.h"

#include "Scene.h"
#include "../Definitions.h"
#include "../Global_variables.h"


namespace PIPE_TYPE{
enum PIPE_TYPE{
    STRAIGHT_1 = 0, // ||
    STRAIGHT_2 = 1, // ==
    CURVE_1 = 2,   // |_
    CURVE_2 = 3,   // |~
    CURVE_3 = 4,   // ~|
    CURVE_4 = 5    // _|
};
}

struct State{
    int x;
    int y;
    int dist;

    State(int _x, int _y, int _dist) : x(_x), y(_y), dist(_dist){}

    bool operator<(const State& other) const{
        return dist > other.dist;
    }
};

struct Pipes_bg{    
    static constexpr int columns = 32;
    static constexpr int rows = 32;
    static constexpr int n_cells = columns * rows;
    static constexpr int x_offset = 9;
    static constexpr int y_offset = 10;
    static constexpr int mov[4][2] = {
        {1,0},
        {-1,0},
        {0,1},
        {0,-1}
    };

    Global_variables& global;

    int pipes_map[7][7];
    alignas(int) bn::regular_bg_map_cell cells[n_cells];
    bn::regular_bg_ptr bg_ptr;
    bn::regular_bg_map_ptr map_ptr;
    int size; // 3,4,5,6,7
    int real_size;

    Pipes_bg(int _size,Global_variables& _global); // 3,4,5,6,7

    void rotate_pipe(int x, int y);
    bool is_solved();

private:
    void make_puzzle();

    void build_pipe(int x,int y);

    bn::regular_bg_map_cell& get_cell(int x,int y);
 };

class Minigame_construction_2 : public Scene{
    private:
        Global_variables& global;

        alignas(int) bn::array<bn::regular_bg_map_cell,32*32> bg_cells;
        bn::regular_bg_map_item bg_map;
        bn::regular_bg_ptr background;
        bn::unique_ptr<Pipes_bg> pipes_bg;
        bn::point cursor_limit;
        bn::point selected_pipe;
        bn::sprite_ptr cursor;
        bn::sprite_palette_ptr cursor_palette;

        bool finished;
        int frame_counter;
        static constexpr int MAX_FRAMES = 600; // 10 seconds (60fps)

        bn::vector<bn::sprite_ptr, 10> timer_label;
    public:
        Minigame_construction_2(Global_variables& _global);
        ~Minigame_construction_2() = default;

        bn::optional<SceneType> update() final;
};

#endif