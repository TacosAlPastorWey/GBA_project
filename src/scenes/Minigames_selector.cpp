#include "scenes/Minigames_selector.h"

const bn::string_view Minigames_selector::scene_names[SCENE_COUNT] = {
    "Construction 1"
};

Minigames_selector::Minigames_selector() : 
    arrow_sprite(bn::sprite_items::arrow.create_sprite(-112, -72)),
    text_generator(bn::sprite_font(bn::sprite_items::fixed_8x16_font))
    {
    text_generator.set_left_alignment();
}

bn::optional<SceneType> Minigames_selector::update(){
     if(bn::keypad::up_pressed()){
        pos_arrow--;
        if(pos_arrow < 0){
            pos_arrow = 0;
            top_scene--;
            if(top_scene < 0) top_scene = 0;
        }

        update_scene_list = true;
    }

    if(bn::keypad::down_pressed()){
        pos_arrow++;
        if(pos_arrow > bn::min(SCENE_COUNT-1,9)){
            pos_arrow = bn::min(SCENE_COUNT-1,9);
            top_scene++;
            if(top_scene > SCENE_COUNT - 10) top_scene = bn::max(SCENE_COUNT - 10,0);
        }

        update_scene_list = true;
    }   

    if(update_scene_list){
        scene_names_sprites.clear();
        for(int i = 0; i < 10 && i+top_scene < SCENE_COUNT; i++){
            text_generator.generate(-100,i*16-72,scene_names[i + top_scene], scene_names_sprites);
        }

        arrow_sprite.set_y(pos_arrow*16-72);
    }

    if(bn::keypad::a_pressed()){
        int scene_index = pos_arrow + top_scene;
        switch(scene_index){
            case 0:
                return SceneType::MINIGAME_CONSTRUCTION_1;
            default:
                return bn::nullopt;
        }
    }

    return bn::nullopt;
}