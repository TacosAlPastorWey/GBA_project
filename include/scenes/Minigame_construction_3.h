#ifndef MINIGAME_CONSTRUCTION_3_H
#define MINIGAME_CONSTRUCTION_3_H

#include "bn_keypad.h"
#include "bn_memory.h"
#include "bn_vector.h"
#include "bn_math.h"
#include "bn_camera_ptr.h"
#include "bn_string.h"

#include "bn_sound_items.h"

#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_items_building_tiles.h"
#include "bn_sprite_items_character.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_regular_bg_items_bg_construction_3.h"
#include "bn_regular_bg_items_daytime_theme.h"

#include "Scene.h"
#include "../Utils.h"
#include "../Definitions.h"
#include "../Global_variables.h"

struct Building_bg{
    static constexpr int n_cells = 32 * 32;
    alignas(int) bn::regular_bg_map_cell cells[n_cells];
    bn::regular_bg_ptr bg_ptr;
    bn::regular_bg_map_item map_item;
    int crane_level; 

    Building_bg();

    void raise_crane();
};

struct Floor_spr{
    bn::sprite_tiles_ptr floor_tiles_ptrs[2];
    bn::span<bn::tile> vram_tiles_ptrs[2];
    bn::sprite_ptr sprites[2];
    
    bn::fixed_point floor_pos;
    int width;
    bool left_move;

    Floor_spr(bn::fixed_point pos,int _width, bn::camera_ptr& camera);

    void update();
    Utils::Fixed_line get_x_line();

private:
    void move_x(int dx);
};

class Minigame_construction_3 : public Scene{
private:
    Global_variables& global;
    bn::camera_ptr camera;
    bn::fixed camera_speed_y;
    bn::fixed camera_delta_y;

    static constexpr int n_cells = 32 * 32;
    alignas(int) bn::regular_bg_map_cell theme_cells[n_cells];
    bn::regular_bg_map_item theme_map_item;
    bn::regular_bg_ptr theme_bg;
    
    Building_bg building_bg;
    bn::sprite_ptr steve_spr;
    bn::vector<Floor_spr, 15> floor_sprites;
    int floors_count;
    int floors_goal;
    bn::vector<bn::sprite_ptr, 4> floors_count_sprs;
    int state; // 0 = playing, 1 = win, 2 = lose

    int frame_counter;
    static constexpr int MAX_FRAMES = 600; // 10 seconds (60fps)
    bn::vector<bn::sprite_ptr, 10> timer_label;

public:
    Minigame_construction_3(Global_variables& global);
    ~Minigame_construction_3();

    bn::optional<SceneType> update() final;

private:
    void set_floors_count_text();

};

#endif