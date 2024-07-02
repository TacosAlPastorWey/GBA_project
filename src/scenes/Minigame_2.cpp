#include "scenes/Minigame_2.h"

Minigame_2::Minigame_2(Global_variables& _global) : global(_global),
    pizza_spr(bn::sprite_items::pizza_base.create_sprite(0,0)),
    background(bn::regular_bg_items::bg_pizzeria.create_bg(-8,0)),
    select_spr(bn::sprite_items::ph_border.create_sprite(-96,-24)),
    pos(2,0)
    {
        ingredients_spr.push_back(bn::sprite_items::ingredients.create_sprite(-96,-24,0));
        ingredients_spr.push_back(bn::sprite_items::ingredients.create_sprite(-56,-24,1));
        ingredients_spr.push_back(bn::sprite_items::ingredients.create_sprite(56,-24,2));
        ingredients_spr.push_back(bn::sprite_items::ingredients.create_sprite(96,-24,3));
        ingredients_spr.push_back(bn::sprite_items::ingredients.create_sprite(-96,24,4));
        ingredients_spr.push_back(bn::sprite_items::ingredients.create_sprite(-56,24,5));
        ingredients_spr.push_back(bn::sprite_items::ingredients.create_sprite(56,24,6));
        ingredients_spr.push_back(bn::sprite_items::ingredients.create_sprite(96,24,7));

        for(int i = 0; i < 16; i++) {
            recipe_val.push_back(global.rng().get_int(8));
            recipe_spr.push_back(bn::sprite_items::ingredients.create_sprite(-100+(32*i), -64, recipe_val[i]));
            if (i > 6) {
                recipe_spr[i].set_visible(false);
            }
        }
        
    }
    
    
bn::optional<SceneType> Minigame_2::update(){
    if (bn::keypad::start_pressed()) {
        return SceneType::MINIGAMES_SELECTOR;
    }

    if (bn::keypad::right_pressed() && pos[1] < 3){
        pos[1] += 1;
        select_spr.set_position(ingredient_pos[pos[0]][pos[1]]);
    }

    if (bn::keypad::left_pressed() && pos[1] > 0){
        pos[1] -= 1;
        select_spr.set_position(ingredient_pos[pos[0]][pos[1]]);
    }

    if (bn::keypad::up_pressed() && pos[0]){
        pos[0] = 0;
        select_spr.set_position(ingredient_pos[pos[0]][pos[1]]);
    }

    if (bn::keypad::down_pressed() && !pos[0]){
        pos[0] = 1;
        select_spr.set_position(ingredient_pos[pos[0]][pos[1]]);
    }

    if (bn::keypad::a_pressed()){ // Main part of game, poggers
        if ((pos[0]*4)+pos[1] == recipe_val[0]) {   // Chose right
            recipe_val.erase(recipe_val.begin());
            recipe_spr.erase(recipe_spr.begin());
            if (!recipe_val.size()) {
                return SceneType::MINIGAMES_SELECTOR;
            }
            for (int i = 0; i < recipe_val.size(); i++) {
                recipe_spr[i].set_x(recipe_spr[i].x() - 32);
                if (i == 6) {
                    recipe_spr[i].set_visible(true);
                }
            }
        } else {                                    // Chose wrong!
            return SceneType::MINIGAMES_SELECTOR; 
        }
    }

    return bn::nullopt;
}

