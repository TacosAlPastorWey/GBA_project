#include "scenes/Minigame_construction_1.h"

Minigame_construction_1::Minigame_construction_1() : 
    player_spr(bn::sprite_items::character.create_sprite(0, 60)),
    velocity(0),
    player_animation(bn::create_sprite_animate_action_forever(player_spr, 16, bn::sprite_items::character.tiles_item(), 0, 1, 2, 1)),
    background(bn::regular_bg_items::bg_construction.create_bg(0, -48))
    {
    coins.push_back(bn::sprite_items::axolotcoin.create_sprite(-40, -140));
    coins.push_back(bn::sprite_items::axolotcoin.create_sprite(-20, -150));
    coins.push_back(bn::sprite_items::axolotcoin.create_sprite(0, -145));
    coins.push_back(bn::sprite_items::axolotcoin.create_sprite(20, -156));
    coins.push_back(bn::sprite_items::axolotcoin.create_sprite(40, -136));

    for(int i = 0; i < 5; i++){
        bricks.push_back(bn::sprite_items::brick.create_sprite(-60 + i * 20, -140));
    }
}

bn::optional<SceneType> Minigame_construction_1::update(){
    if(bn::keypad::start_pressed()){
        return SceneType::MINIGAMES_SELECTOR;
    }

    // Coins fall
    for(int i = 0; i < coins.size(); i++){
        coins[i].set_y(coins[i].y() + bn::fixed(0.5));
        if(coins[i].y() > 140){
            coins[i].set_y(-140);
        }
    }

    // Brick fall
    for(int i = 0; i < bricks.size(); i++){
        bricks[i].set_y(bricks[i].y() + bn::fixed(0.5));
        if(bricks[i].y() > 140){
            bricks[i].set_y(-140);
        }
    }

    // Player movement
    if(bn::keypad::left_pressed()){
        velocity = bn::fixed(-0.5);
    }else if(bn::keypad::left_held()){
        velocity = bn::max(velocity - bn::fixed(0.1), bn::fixed(-5.0));
    }

    if(bn::keypad::right_pressed()){
        velocity = bn::fixed(0.5);
    }else if(bn::keypad::right_held()){
        velocity = bn::min(velocity + bn::fixed(0.1), bn::fixed(5.0));
    }

    if(!bn::keypad::left_held() && !bn::keypad::right_held()){
        if(velocity > 0){
            velocity = bn::max(velocity - bn::fixed(0.15), bn::fixed(0));
        }else if(velocity < 0){
            velocity = bn::min(velocity + bn::fixed(0.15), bn::fixed(0));
        }
    }

    player_spr.set_x(player_spr.x() + velocity);

    if(player_spr.x() < -112){
        player_spr.set_x(-112);
    }else if(player_spr.x() > 112){ 
        player_spr.set_x(112);
    }

    // Check player_spr and coins collision
    for(int i = 0; i < coins.size(); i++){
        if(bn::abs(player_spr.x() - coins[i].x()) < 8 && bn::abs(player_spr.y() - coins[i].y()) < 8){
            coins[i].set_y(-140);
            bn::sound_items::coin_sound.play();
        }
    }

    // Check player_spr and bricks collision
    for(int i = 0; i < bricks.size(); i++){
        if(bn::abs(player_spr.x() - bricks[i].x()) < 8 && bn::abs(player_spr.y() - bricks[i].y()) < 8){
            bricks[i].set_y(-140);
            bn::sound_items::hit.play();
            return SceneType::MINIGAMES_SELECTOR;
        }
    }

    player_animation.update();
    return bn::nullopt;
}