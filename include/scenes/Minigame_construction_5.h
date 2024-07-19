#ifndef MINIGAME_CONSTRUCTION_5_H
#define MINIGAME_CONSTRUCTION_5_H

#include "bn_keypad.h"
#include "bn_memory.h"
#include "bn_string.h"
#include "bn_vector.h"
#include "bn_forward_list.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_items_digger.h"
#include "bn_sprite_items_digger_dirt.h"
#include "bn_sprite_items_construction_5_border.h"
#include "bn_sprite_items_axolotcoin.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_regular_bg_tiles_items_bg_construction_fd_1_tiles.h"
#include "bn_regular_bg_tiles_items_bg_construction_fd_2_tiles.h"

#include "bn_sound_items.h"

#include "Scene.h"
#include "../Global_variables.h"
#include "../Definitions.h"

enum class DIRT_TYPE{
    CLEAN,
    DOT_1,
    DOT_2,
    DOT_3,
    DOT_4,
    DOT_5,
    DOT_6,
    DOT_7,
    DOT_8
};

enum class HOLE_TYPE : int{
    HOLE = 0,
    FLAT = 1,
    HEAP = 2
};

class Minigame_construction_5 : public Scene{
private:
    static constexpr int columns = 32;
    static constexpr int rows = 32;
    static constexpr int n_cells = columns * rows;
    static constexpr int v_16x16_columns = 15;
    static constexpr int v_16x16_rows = 10;
    static constexpr int holes_map_columns = 15;
    static constexpr int holes_map_rows = 10;
    static constexpr int holes_map_len = holes_map_columns * holes_map_rows;

    Global_variables& global;

    alignas(int) bn::regular_bg_map_cell dirt_cells[n_cells];
    bn::regular_bg_ptr dirt_bg_ptr;
    bn::regular_bg_map_ptr dirt_map_ptr; 
    
    alignas(int) bn::regular_bg_map_cell holes_cells[n_cells];
    bn::regular_bg_ptr holes_bg_ptr;
    bn::regular_bg_map_ptr holes_map_ptr;

    HOLE_TYPE holes_map[holes_map_len];
    int coins_buried_map[holes_map_len];
    int num_holes;
    int num_heaps;

    bn::vector<bn::pair<bn::sprite_ptr,int>, 8> coins;

    bn::point tile_selected;
    bn::sprite_ptr tile_selected_border;
    bn::sprite_ptr digger_sprite;
    bn::sprite_ptr digger_dirt_sprite;
    bn::point digger_direction;
    bool digger_has_dirt;

    int frame_counter;
    static constexpr int MAX_FRAMES = 1200; // 20 seconds (60fps)
    int state = 0; // 0 = playing, 1 = win, 2 = lose

    bn::vector<bn::sprite_ptr, 10> win_lose_label;    
    bn::vector<bn::sprite_ptr,10> timer_label;
public:
    Minigame_construction_5(Global_variables& _global);
    ~Minigame_construction_5() = default;

    bn::optional<SceneType> update() final;

private:
    bn::regular_bg_map_cell& get_dirt_cell(int x,int y);
    bn::regular_bg_map_cell& get_holes_cell(int x,int y);
    HOLE_TYPE& get_hole_map(int x, int y);

    void make_n_random_holes(bn::forward_list<bn::point,150> &pos_candidates,int n,HOLE_TYPE type);

    inline bool is_all_nivelated();

    void set_dirt_cells(int x, int y, DIRT_TYPE type);
    void set_holes_cells(int x, int y, HOLE_TYPE type);

    void fill_dirt();
};

#endif