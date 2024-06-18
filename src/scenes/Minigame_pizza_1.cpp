#include "scenes/Minigame_pizza_1.h"

Minigame_pizza_1::Minigame_pizza_1() :
    player_spr(bn::sprite_items::pizza_paddle.create_sprite(0,60)),
    oven_spr(bn::sprite_items::pizza_oven.create_sprite(0,-17)),
    background(bn::regular_bg_items::bg_empty_pizzeria.create_bg(-8,0)),
    velocity(3),
    oven_animation(bn::create_sprite_animate_action_forever(oven_spr, 12, bn::sprite_items::pizza_oven.tiles_item(), 0, 2, 1, 2))
    {
}

bn::optional<SceneType> Minigame_pizza_1::update(){
    if(bn::keypad::start_pressed()){
        return SceneType::MINIGAMES_SELECTOR;
    }

    if(bn::keypad::a_pressed()){
        BN_LOG("X coord hit: ", player_spr.x());
        if ((player_spr.x() > -11) && (player_spr.x() < 11)) {
            return SceneType::HOUSE;
        } else {
            return SceneType::MINIGAMES_SELECTOR;
        }
    }


    player_spr.set_x(player_spr.x() + velocity);

    // Border paddle collision
    if((player_spr.x() > 104) || (player_spr.x() < -104)) {
        velocity *= -1;
    }

    oven_animation.update();
    return bn::nullopt;
}