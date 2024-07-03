#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include "bn_random.h"

#include "bn_sprite_text_generator.h"
#include "bn_sprite_font.h"
#include "bn_sprite_items_common_variable_8x8_font.h"
#include "bn_sprite_items_fixed_8x16_font.h"

#include "Save_game.h"

class Global_variables{
    private:
        Save_game _save;
        int _game_difficulty;
        int _game_money;
        bn::random _rng;
        bn::sprite_text_generator _text_generator_var_8x8;
        bn::sprite_text_generator _text_generator_fix_8x16;
    public:
        Global_variables();
        ~Global_variables() = default;

        Save_game& save();
        int game_difficulty();
        void set_game_difficulty(int difficulty);
        int game_money();
        void add_game_money(int amount);
        bn::random& rng();
        bn::sprite_text_generator& text_gen_var_8x8();
        bn::sprite_text_generator& text_gen_fix_8x16();
};

#endif