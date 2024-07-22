#ifndef MINIGAME_COFFE_1_H
#define MINIGAME_COFFE_1_H

#include "bn_keypad.h"
#include "bn_memory.h"
#include "bn_span.h"
#include "bn_math.h"
#include "bn_string.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_item.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_tiles_item.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_palette_item.h"
#include "bn_sprite_items_coffee_mug.h"
#include "bn_sprite_items_milk_jug.h"
#include "bn_sprite_items_milk_jug_hand.h"
#include "bn_sprite_items_coffee_figures.h"
#include "bn_sprite_items_coffee_bar_fig.h"
#include "bn_sprite_items_coffee_bar_cream.h"
#include "bn_sprite_items_arrow_8x8.h"
#include "bn_sprite_items_coffee_mug_handle.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_coffee_table.h"

#include "bn_sound_items.h"
#include "bn_sound_handle.h"

#include "Scene.h"
#include "../Utils.h"
#include "../Definitions.h"
#include "../Global_variables.h"

class Minigame_coffee_1 : public Scene{
private:
    static constexpr int COFFEE_DECORATIONS_PIXELS[5] = {
        839, // Triangle
        910, // Heart
        925, // Milky way
        717, // Flower
        1379 // Swan 
    };

    Global_variables &global;

    bn::regular_bg_ptr coffee_bg;

    bn::sprite_ptr coffee_mug;
    bn::sprite_ptr coffee_mug_handle;
    bn::sprite_ptr expected_coffee_decoration;
    bn::span<const bn::tile> expected_decorations_tiles;
    
    bn::array<bn::color, 16> coffee_decoration_colors;
    bn::sprite_tiles_ptr coffee_decoration_tiles_ptr;
    bn::span<bn::tile> coffee_decoration_tiles_vram;
    bn::sprite_ptr coffee_decoration;
    int pixels_painted_correctly;
    int pixels_painted;

    bn::sprite_tiles_ptr coffee_pointer_tiles_ptr;
    bn::span<bn::tile> coffee_pointer_tiles_vram;
    bn::sprite_ptr coffee_pointer;

    bn::sprite_ptr bar_fig;
    bn::fixed bar_fig_ratio;
    bn::sprite_ptr bar_fig_arrow;
    bn::sprite_ptr bar_cream;
    bn::fixed bar_cream_ratio;
    bn::sprite_ptr bar_cream_arrow;
    int fig_state; // 0 not enough, 1 correct, 2 too much
    bool sending;
    bool starting_over;
    bool moving_mug;
    int op_frames;

    bn::sprite_ptr milk_jug;
    bn::sprite_ptr milk_jug_hand;
    bool pouring_milk;

    bn::optional<bn::sound_handle> pouring_sound;
    
    bn::fixed_point circle_center = bn::fixed_point(32,32);
    bn::fixed circle_radius = 2.5;
    bn::fixed_point dir;

    int state; // 0: playing, 1: win, 2: lose
    int frame_counter;
    static constexpr int MAX_FRAMES = 1200; // 15 seconds (60fps)

    bn::vector<bn::sprite_ptr, 10> aux_label;
    bn::vector<bn::sprite_ptr, 10> perfect_label;
    bn::vector<bn::sprite_ptr,10> timer_label;
    bn::vector<bn::sprite_ptr,10> win_lose_label;
public:
    Minigame_coffee_1(Global_variables &_global);
    ~Minigame_coffee_1() = default;

    bn::optional<SceneType> update() final;

    void init_mug();   
    void toggle_HUD(bool show);
    void move_cursor();
    void rotate_milk_jug(bn::fixed degrees);
    void fill_circle(bn::span<bn::tile> &destination,bn::fixed c_x,bn::fixed c_y, bn::fixed radius,int color,bool pixel_count);
    void update_bars();
};
#endif