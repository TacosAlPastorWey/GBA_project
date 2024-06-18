#include "scenes/House.h"

House::House() :
    steve_spr(bn::sprite_items::character.create_sprite(0, 0)),
    house_bg(bn::regular_bg_items::bg_house_1.create_bg(-8, 0))
    {
}

bn::optional<SceneType> House::update() {
    if(bn::keypad::a_pressed()){
        return SceneType::MINIGAMES_SELECTOR;
    }

    // Move Steve
    if(bn::keypad::left_held()){
        steve_spr.set_x(steve_spr.x() - 1);
    }
    if(bn::keypad::right_held()){
        steve_spr.set_x(steve_spr.x() + 1);
    }
    if(bn::keypad::up_held()){
        steve_spr.set_y(steve_spr.y() - 1);
    }
    if(bn::keypad::down_held()){
        steve_spr.set_y(steve_spr.y() + 1);
    }

    return bn::nullopt;
}