#include "scenes/Leaderboard.h"

Leaderboard::Leaderboard (Save_game &Save) :
    text_generator(bn::sprite_font(bn::sprite_items::fixed_8x16_font))
    {
    text_generator.set_center_alignment();
    text_generator.generate(0, -64, "Leaderboard", text_sprites);
    text_generator.set_left_alignment();
    for (int i = 0; i < 10; i++) {
        bn::string<20> str;
        bn::ostringstream val(str);
        val.append(Save.highscore_values(i));
        text_generator.generate(-90, -40+(12*i), Save.highscore_names(i), text_sprites);
        text_generator.generate(10, -40+(12*i), str, text_sprites);
    }
}

bn::optional<SceneType> Leaderboard::update() {
    if(bn::keypad::b_pressed()){
        return SceneType::HOUSE;
    }

    return bn::nullopt;
}
