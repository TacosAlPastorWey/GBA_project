#include "Global_variables.h"

Global_variables::Global_variables() : 
    _save(),
    _game_difficulty(1),
    _game_money(0),
    _rng(),
    _text_generator_var_8x8(bn::sprite_font(bn::sprite_items::common_variable_8x8_font)),
    _text_generator_fix_8x16(bn::sprite_font(bn::sprite_items::fixed_8x16_font))
    {
    _save.load();

    _text_generator_fix_8x16.set_center_alignment();
    _text_generator_var_8x8.set_center_alignment();
}

Save_game& Global_variables::save(){
    return _save;
}

int Global_variables::game_difficulty(){
    return _game_difficulty;
}

void Global_variables::set_game_difficulty(int difficulty){
    _game_difficulty = difficulty;
}

int Global_variables::game_money(){
    return _game_money;
}

void Global_variables::add_game_money(int amount){
    _game_money += amount;
}

bn::random& Global_variables::rng(){
    return _rng;
}

bn::sprite_text_generator& Global_variables::text_gen_var_8x8(){
    return _text_generator_var_8x8;
}

bn::sprite_text_generator& Global_variables::text_gen_fix_8x16(){
    return _text_generator_fix_8x16;
}