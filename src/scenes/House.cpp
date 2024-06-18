#include "scenes/House.h"

House::House() :
    steve_spr(bn::sprite_items::character.create_sprite(0, 0)),
    house_bg(bn::regular_bg_items::bg_house_1.create_bg(-8, 0)),
    house_palette(house_bg.palette()),
    hitbox_spr(bn::sprite_items::hitbox.create_sprite(88, 40)),
    text_generator(bn::sprite_font(bn::sprite_items::common_variable_8x8_font))
    {
}

bn::optional<SceneType> House::update() {
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

    // Check if Steve is near the right door
    if(steve_spr.x() >= 72 && steve_spr.x() <= 104 && steve_spr.y() >= 16 && steve_spr.y() <= 64){
        house_palette.set_color(7, bn::color(31, 0, 0));

        if(text_sprites.empty()){
            text_generator.generate(0, 0, "Play", text_sprites);
        }

        if(bn::keypad::a_pressed()){
            return SceneType::MINIGAMES_SELECTOR;
        }
    }else{
        house_palette.set_color(7, bn::color(17, 18, 9));

        if(!text_sprites.empty()){
            text_sprites.clear();
        }
    }

    return bn::nullopt;
}