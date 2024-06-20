#include "scenes/House.h"

House::House() :
    steve_spr(bn::sprite_items::character.create_sprite(0, 0)),
    house_bg(bn::regular_bg_items::bg_house_1.create_bg(-8, 0)),
    house_palette(house_bg.palette()),
    text_generator(bn::sprite_font(bn::sprite_items::common_variable_8x8_font)),
    objects_map{
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,5,5,4,4,3,3,2,2,0,0,1,1,0},
        {0,0,0,5,5,0,0,0,0,0,0,0,0,1,1,0},
        {0,0,0,6,0,7,7,7,7,7,0,0,0,1,1,0},
        {0,0,0,6,0,7,7,7,7,7,0,0,0,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
    prev_object(0),
    object(0)
    {

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

    int map_x = int((steve_spr.x() + 136) / 16);
    int map_y = int((steve_spr.y() + 80) / 16);

    if(map_x < 0 || map_x >= 16 || map_y < 0 || map_y >= 10){
        BN_LOG("Out of bounds");
    }

    object = objects_map[bn::abs(map_y%10)][bn::abs(map_x%16)];

    switch (object)
    {
    case 1: // Job door
        if(prev_object != 1){
            restore_bg_palette();
            house_palette.set_color(7, bn::color(31, 0, 0));

            show_text("Go to work");
        }
        if(bn::keypad::a_pressed()){
            return SceneType::MINIGAMES_SELECTOR;
        }
        break;
    case 2: // Window
        if(prev_object != 2){
            restore_bg_palette();
            house_palette.set_color(12, bn::color(31, 0, 0));

            show_text("Look outside");
        }
        if(bn::keypad::a_pressed()){
            return SceneType::WINDOW;
        }
        break;
    case 3: // Leaderboard
        if(prev_object != 3){
            restore_bg_palette();
            house_palette.set_color(10, bn::color(31, 0, 0));

            show_text("Leaderboard");
        }
        if(bn::keypad::a_pressed()){
            return SceneType::LEADERBOARD;
        }
        break;
    case 4: // Wardrobe
        if(prev_object != 4){
            restore_bg_palette();
            house_palette.set_color(13, bn::color(31, 0, 0));

            show_text("Change outfit");
        }
        if(bn::keypad::a_pressed()){
            return SceneType::WARDROBE;
        }
        break;
    case 5: // Computer
        if(prev_object != 5){
            restore_bg_palette();
            house_palette.set_color(23, bn::color(31, 0, 0));

            show_text("Go online");
        }
        if(bn::keypad::a_pressed()){
            return SceneType::COMPUTER;
        }
        break;
    case 6: // Toolbox
        if(prev_object != 6){
            restore_bg_palette();
            house_palette.set_color(21, bn::color(31, 0, 0));

            show_text("Toolbox");
        }
        break;
    case 7: // Bed
        if(prev_object != 7){
            restore_bg_palette();
            house_palette.set_color(20, bn::color(31, 0, 0));

            show_text("Sleep");
        }
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