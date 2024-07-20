#ifndef MINIGAME_BAR_1_H
#define MINIGAME_BAR_1_H

#include "bn_array.h"
#include "bn_color.h"
#include "bn_keypad.h"
#include "bn_memory.h"
#include "bn_vector.h"
#include "bn_string.h"
#include "bn_optional.h"
#include "bn_string_view.h"

#include "bn_sprites.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_item.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_tiles_item.h"
#include "bn_sprite_shape_size.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_items_liquid.h"
#include "bn_sprite_items_beer_mug.h"
#include "bn_sprite_items_mug_hand.h"
#include "bn_sprite_items_arrow_8x8.h"
#include "bn_sprite_tiles_items_beer_mug_liquid_a_tiles.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_regular_bg_items_bg_bar_behind.h"

#include "bn_sound_items.h"
#include "bn_sound_handle.h"

#include "Scene.h"
#include "../Definitions.h"
#include "../Global_variables.h"

class Minigame_bar_1 : public Scene{
private:
    static constexpr bn::color DRINKS_COLOR[3] = {
        bn::color(6,5,20),
        bn::color(3,12,4),
        bn::color(28,8,1)
    };

    static constexpr bn::string_view DRINKS_NAMES[3] = {
        "Ablue",
        "Breen",
        "Coranje"
    };

    Global_variables &global;

    alignas(int) bn::regular_bg_map_cell bar_behind_cells[32*32];
    bn::regular_bg_ptr bar_behind_bg;
    bn::regular_bg_map_ptr bar_behind_map;

    bn::sprite_ptr liquid_spr;
    alignas(int) bn::array<bn::tile, 8*8> beer_mug_tiles;
    bn::sprite_ptr beer_mug;
    bn::sprite_tiles_ptr beer_mug_tiles_ptr;
    bn::sprite_ptr mug_hand;
    int hand_position;  // 0,1,2
    bn::fixed curr_level;
    bool filling;
    int frames_held;
    int drinks_served;
    int drinks_to_serve;

    bn::sprite_ptr level_arrow;
    int min_level_needed; //[340,460]
    int type_of_drink; // 0,1,2

    int state; // 0->playing, 1->win, 2->lose
    int frame_counter;
    static constexpr int MAX_FRAMES = 900; // 15 seconds (60fps)

    bn::optional<bn::sound_handle> fill_sound;

    bn::vector<bn::sprite_ptr, 10> overflow_label;
    int overflow_label_lifespan;

    bn::vector<bn::sprite_ptr,5> tip_label;
    int tip_label_lifespan;

    bn::vector<bn::sprite_ptr, 6> type_drink_label;
    bn::vector<bn::sprite_ptr, 6> sent_mug_label;
    bn::vector<bn::sprite_ptr, 6> win_label;
    bn::vector<bn::sprite_ptr,10> timer_label;
public:

    Minigame_bar_1(Global_variables &_global);
    ~Minigame_bar_1() = default;

    bn::optional<SceneType> update() final;

    void set_hand_in_position();

    void start_filling();
    void update_filling();
    void stop_filling();

    void next_drink();
    void load_empty_mug();
    void set_liquid_level();
    void set_level_arrow();

    void start_bar_begin_bg();
    void add_drink_to_finished(int type);

    void show_and_add_tip();
};

#endif