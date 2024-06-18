#include "scenes/House.h"

House::House() :
    steve_spr(bn::sprite_items::character.create_sprite(0, 0)),
    house_bg(bn::regular_bg_items::bg_house_1.create_bg(-8, 0)),
    house_palette(house_bg.palette()),
    text_generator(bn::sprite_font(bn::sprite_items::common_variable_8x8_font)),
    prev_object(0),
    object(0)
    {

    objects_hitbox[0] = bn::fixed_rect(80, 24, 16, 48);
    objects_hitbox[1] = bn::fixed_rect(24, 0, 32, 32);
    objects_hitbox[2] = bn::fixed_rect(-8, 8, 32, 16);
    objects_hitbox[3] = bn::fixed_rect(-40, 8, 32, 16);
    objects_hitbox[4] = bn::fixed_rect(-72, 16, 32, 32);    
    objects_hitbox[5] = bn::fixed_rect(-72, 40, 32, 16);
    objects_hitbox[6] = bn::fixed_rect(-24, 40, 64, 16);
}

bn::optional<SceneType> House::update() {
    // Move Steve
    if(bn::keypad::left_held()){
        steve_spr.set_x(steve_spr.x() - 1);
        if(steve_spr.x() < -88) steve_spr.set_x(-88);
    }
    if(bn::keypad::right_held()){
        steve_spr.set_x(steve_spr.x() + 1);
        if(steve_spr.x() > 88) steve_spr.set_x(88);
    }
    if(bn::keypad::up_held()){
        steve_spr.set_y(steve_spr.y() - 1);
        if(steve_spr.y() < 0) steve_spr.set_y(0);
    }
    if(bn::keypad::down_held()){
        steve_spr.set_y(steve_spr.y() + 1);
        if(steve_spr.y() > 48) steve_spr.set_y(48);
    }

    object = 0;

    // Check if Steve is near the right door
    if(Utils::is_inside(steve_spr.position(), objects_hitbox[0])){ 
        object = 1; // Right door
        if(prev_object != 1){
            restore_bg_palette();
            house_palette.set_color(7, bn::color(31, 0, 0));

            show_text("Go to work");
        }
    }
    if(Utils::is_inside(steve_spr.position(), objects_hitbox[1])){ 
        object = 2; // Window
        if(prev_object != 2){
            restore_bg_palette();
            house_palette.set_color(12, bn::color(31, 0, 0));

            show_text("Look outside");
        }
    }
    if(Utils::is_inside(steve_spr.position(), objects_hitbox[2])){ 
        object = 3; // Leaderboard
        if(prev_object != 3){
            restore_bg_palette();
            house_palette.set_color(10, bn::color(31, 0, 0));

            show_text("Leaderboard");
        }
    }
    if(Utils::is_inside(steve_spr.position(), objects_hitbox[3])){ 
        object = 4; // Wardrobe
        if(prev_object != 4){
            restore_bg_palette();
            house_palette.set_color(13, bn::color(31, 0, 0));

            show_text("Change outfit");
        }
    }
    if(Utils::is_inside(steve_spr.position(), objects_hitbox[4])){ 
        object = 5; // Computer
        if(prev_object != 5){
            restore_bg_palette();
            house_palette.set_color(23, bn::color(31, 0, 0));

            show_text("Go online");
        }
    }
    if(Utils::is_inside(steve_spr.position(), objects_hitbox[5])){ 
        object = 6; // Configurations
        if(prev_object != 6){
            restore_bg_palette();
            house_palette.set_color(21, bn::color(31, 0, 0));

            show_text("Toolbox");
        }
    }
    if(Utils::is_inside(steve_spr.position(), objects_hitbox[6])){ 
        object = 7; // Bed
        if(prev_object != 7){
            restore_bg_palette();
            house_palette.set_color(20, bn::color(31, 0, 0));

            show_text("Sleep");
        }
    }

    switch (object){
        case 1:
            if(bn::keypad::a_pressed()){
                return SceneType::MINIGAMES_SELECTOR;
            }
            break;
        case 7:
            if(bn::keypad::a_pressed()){
                bn::core::sleep(bn::keypad::key_type::START);
            }
            break;
        default:
            break;
    }

    if(object == 0 && prev_object != 0){
        restore_bg_palette();

        if(!text_sprites.empty()){
            text_sprites.clear();
        }
    }

    prev_object = object;
    return bn::nullopt;
}

void House::restore_bg_palette(){
    house_palette.set_colors(bn::regular_bg_items::bg_house_1.palette_item());
}

void House::show_text(const bn::string_view& text){
    if(!text_sprites.empty()){
        text_sprites.clear();
    }

    text_generator.generate(0, -50, text, text_sprites);
}