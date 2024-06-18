#ifndef HOUSE_H
#define HOUSE_H

#include "bn_keypad.h"
#include "bn_vector.h"
#include "bn_fixed_rect.h"
#include "bn_core.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_items_character.h"

#include "bn_sprite_text_generator.h"
#include "bn_sprite_items_common_variable_8x8_font.h"

#include "bn_regular_bg_ptr.h"
#include "bn_bg_palette_ptr.h"
#include "bn_regular_bg_items_bg_house_1.h"

#include "Scene.h"
#include "../Utils.h"

class House : public Scene {
    private:
        bn::sprite_ptr steve_spr;
        bn::regular_bg_ptr house_bg;
        bn::bg_palette_ptr house_palette;
        bn::sprite_text_generator text_generator;
        bn::vector<bn::sprite_ptr, 10> text_sprites;
        bn::fixed_rect objects_hitbox[7];
        int prev_object;
        int object;
    public:
        House();
        ~House() = default;

        bn::optional<SceneType> update() final;

    private:
        void restore_bg_palette();
        void show_text(const bn::string_view& text);
};


#endif