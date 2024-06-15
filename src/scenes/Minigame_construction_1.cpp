#include "scenes/Minigame_construction_1.h"

Minigame_construction_1::Minigame_construction_1() : 
    player_spr(bn::sprite_items::character.create_sprite(0, 60)),
    player_palette(player_spr.palette()),
    velocity(0),
    stunned_frames(0),
    player_animation(bn::create_sprite_animate_action_forever(player_spr, 16, bn::sprite_items::character.tiles_item(), 0, 1, 2, 1)),
    background(bn::regular_bg_items::bg_construction.create_bg(0, -48)),
    objects_falling(0),
    rand_num_gen()
    {
    for(int i = 0; i < 5; i++){
        coins.push_back(bn::sprite_items::axolotcoin.create_sprite(0, -140));
        coins_falling[i] = false;
    }

    for(int i = 0; i < BRICK_COUNT; i++){
        bricks.push_back(bn::sprite_items::brick.create_sprite(0, -140));
        bricks_falling[i] = false;
    }
}

bn::optional<SceneType> Minigame_construction_1::update(){
    if(bn::keypad::start_pressed()){
        return SceneType::MINIGAMES_SELECTOR;
    }

    if(objects_falling < 10){
        if(rand_num_gen.get_unbiased_int(10) == 0){
            // 10% chance of a coin falling
            for(int i = 0; i < 5; i++){
                if(!coins_falling[i]){
                    coins_falling[i] = true;
                    coins[i].set_x(rand_num_gen.get_unbiased_int(15)*16 - 112);
                    coins[i].set_y(-rand_num_gen.get_unbiased_int(3)*16 - 128);
                    objects_falling++;
                    break;
                }
            }
        }else{
            // 90% chance of a brick falling
            if(rand_num_gen.get_unbiased_int(4) == 0){
                for(int i = 0; i < BRICK_COUNT; i++){
                    if(!bricks_falling[i]){
                        bricks_falling[i] = true;
                        bricks[i].set_x(rand_num_gen.get_unbiased_int(15)*16 - 112);
                        bricks[i].set_y(-rand_num_gen.get_unbiased_int(3)*16 - 128);
                        objects_falling++;
                        break;
                    }
                }
            }
        }
    }

    // Coins fall
    for(int i = 0; i < coins.size(); i++){
        if(coins_falling[i]){
            coins[i].set_y(coins[i].y() + bn::fixed(1));
            if(coins[i].y() > 140){
                coins[i].set_y(-140);
                coins_falling[i] = false;
                objects_falling--;
            }
        }
    }

    // Brick fall
    for(int i = 0; i < bricks.size(); i++){
        if(bricks_falling[i]){
            bricks[i].set_y(bricks[i].y() + bn::fixed(1));
            if(bricks[i].y() > 140){
                bricks[i].set_y(-140);
                bricks_falling[i] = false;
                objects_falling--;
            }
        }
    }

    // Player movement
    if(stunned_frames > 0){
        stunned_frames--;
        if(stunned_frames == 0){
            player_palette.set_color(1, bn::color(4, 4, 6));
        }
    }else{
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
    }

    player_spr.set_x(player_spr.x() + velocity);

    if(player_spr.x() < -112){
        player_spr.set_x(-112);
        if(velocity <= -3){ 
            stunned_frames = 60;
            player_palette.set_color(1, bn::color(31, 0, 0));
        }
        velocity = 0;
    }else if(player_spr.x() > 112){ 
        player_spr.set_x(112);

        if(velocity >= 3){ 
            stunned_frames = 60;
            player_palette.set_color(1, bn::color(31, 0, 0));
        }
        velocity = 0;
    }

    // Check player_spr and coins collision
    for(int i = 0; i < coins.size(); i++){
        if(bn::abs(player_spr.x() - coins[i].x()) < 8 && bn::abs(player_spr.y() - coins[i].y()) < 8){
            coins[i].set_y(-140);
            bn::sound_items::coin_sound.play();
            coins_falling[i] = false;
            objects_falling--;
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
    rand_num_gen.update();
    return bn::nullopt;
}