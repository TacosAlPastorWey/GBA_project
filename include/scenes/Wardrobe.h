#ifndef WARDROBE_H
#define WARDROBE_H

#include "bn_keypad.h"
#include "bn_vector.h"
#include "bn_string.h"

#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_items_arrow_wardrobe.h"
#include "bn_sprite_items_object.h"
#include "bn_sprite_items_locked.h"

#include "bn_sprite_text_generator.h"
#include "bn_sprite_items_fixed_8x16_font.h"

#include "bn_regular_bg_ptr.h"
#include "bn_bg_palette_ptr.h"
#include "bn_regular_bg_items_bg_wardrobe.h"

#include "Scene.h"
#include "../Save_game.h"

class Wardrobe : public Scene {
    private:
        Save_game& save;
        bn::regular_bg_ptr background;
        bn::bg_palette_ptr bg_palette;

        bn::sprite_ptr left_arrow;
        bn::sprite_palette_ptr left_arrow_palette;
        bn::sprite_ptr right_arrow;
        bn::sprite_palette_ptr right_arrow_palette;

        bn::sprite_text_generator text_generator;
        bn::string<8> index_text;
        bn::vector<bn::sprite_ptr, 8> text_sprites;

        bn::sprite_ptr curr_item;

        int prev_option_selected;
        int option_selected;
        int prev_item_index;
        int item_index;
    public:
        Wardrobe(Save_game& _save);
        ~Wardrobe() = default;
        
        bn::optional<SceneType> update() final;

    private:
        void set_index_text();
        void load_curr_item();
};

#endif