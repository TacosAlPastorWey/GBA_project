#include "scenes/Minigame_construction_4.h"

#include "bn_log.h"
#include "bn_core.h"

Stone::Stone(int _life) : 
    sprite(bn::sprite_items::stone.create_sprite(-40, 48)),
    max_life(_life),
    life(_life)
    {
}

void Stone::break_stone(int damage){
    life -= damage;
    if(life < 0) life = 0;
    int frame = 4 - (life*4)/max_life;
    if(life > 0) frame = bn::min(3, frame);
    sprite.set_tiles(bn::sprite_items::stone.tiles_item().create_tiles(frame));
}

bool Stone::is_breaked(){
    return life <= 0;
}

Hit_bar::Hit_bar(Global_variables& _global) : 
    global(_global),
    floor_tiles_ptrs{bn::sprite_tiles_ptr::allocate(4, bn::bpp_mode::BPP_4), bn::sprite_tiles_ptr::allocate(4, bn::bpp_mode::BPP_4)},
    vram_tiles_ptrs{floor_tiles_ptrs[0].vram().value(), floor_tiles_ptrs[1].vram().value()},
    sprites{
        bn::sprite_ptr::create(POS.x()-16, POS.y(), bn::sprite_shape_size(32,8),floor_tiles_ptrs[0],bn::sprite_palette_ptr::create(bn::sprite_items::hit_bar.palette_item())),
        bn::sprite_ptr::create(POS.x()+16, POS.y(), bn::sprite_shape_size(32,8),floor_tiles_ptrs[1],bn::sprite_palette_ptr::create(bn::sprite_items::hit_bar.palette_item()))}    ,
    arrow(bn::sprite_items::arrow_8x8.create_sprite(POS.x()-80, POS.y()-10))
    {
    sprites[1].set_horizontal_flip(true);

    activate();
}

void Hit_bar::activate(){
    sprites[0].set_visible(true);
    sprites[1].set_visible(true);
    arrow.set_visible(true);

    crit_center = global.rng().get_unbiased_int(5);

    bn::memory::copy(hit_bar_bn_gfxTiles[crit_center*4], 4, vram_tiles_ptrs[0].data()[0]);
    bn::memory::copy(hit_bar_bn_gfxTiles[(4-crit_center)*4], 4, vram_tiles_ptrs[1].data()[0]);

    active = true;
}

void Hit_bar::update(){
    if(arrow_move_left){
        arrow.set_x(arrow.x() - 1.02f);
    }else{
        arrow.set_x(arrow.x() + 1.02f);
    }

    if(arrow.x() < POS.x()-30){
        arrow.set_x(POS.x()-30);
        arrow_move_left = false;
    }else if(arrow.x() > POS.x()+30){
        arrow.set_x(POS.x()+30);
        arrow_move_left = true;
    }
}

int Hit_bar::hit(){
    active = false;

    bn::fixed center = POS.x() - 16 + (8*crit_center);
    bn::fixed dist = bn::abs(arrow.x() - center);
    // 2, 6, 14
    if(dist <= 2){
        return 8;
    }else if(dist <= 6){
        return 4;
    }else if(dist <= 14){
        return 2;
    }
    return 1;
}

void Hit_bar::deactivate(){
    sprites[0].set_visible(false);
    sprites[1].set_visible(false);
    arrow.set_visible(false);
    active = false;
}

bool Hit_bar::is_active(){
    return active;
}

Minigame_construction_4::Minigame_construction_4(Global_variables& _global) : 
    global(_global),
    camera(bn::camera_ptr::create(0, 0)),
    camera_shake_frames(0),
    camera_shake_counter(0),
    shaking_camera(false),
    theme_bg(bn::regular_bg_items::daytime_theme.create_bg(-8, 0)),
    spr_steve(bn::sprite_items::steve_lateral.create_sprite(56, 16)),
    spr_pickaxe(bn::sprite_items::pickaxe.create_sprite(64, -16)),
    spr_steve_arm(bn::sprite_items::steve_arm.create_sprite(64, 16)),
    rotation_angle(5),
    rotation_speed(0),
    arm_moving(false),
    stones_breaked(0),
    stone(bn::make_unique<Stone>(8)),
    replacing_stone(false),
    hit_bar(global),
    frame_counter(0)
    {
    stones_to_break = global.game_difficulty();
    update_stones_breaked_label();
    
    stone->sprite.set_camera(camera);
    spr_steve.set_camera(camera);
    spr_pickaxe.set_camera(camera);
    spr_steve_arm.set_camera(camera);

    spr_steve.set_scale(2);
    rotate_steve_arm(5);
}

bn::optional<SceneType> Minigame_construction_4::update(){
    #ifdef MINIGAMES_TEST
    if(bn::keypad::start_pressed()){
        return SceneType::MINIGAMES_SELECTOR;
    }
    #endif

    frame_counter++;
    if(frame_counter <= MAX_FRAMES && frame_counter >= 300){  // Display after 5 seconds
        timer_label.clear();
        bn::fixed act_sec(MAX_FRAMES-frame_counter);
        act_sec /= 60;
        global.text_gen_fix_8x16().generate(80,-60,bn::to_string<3,int>(act_sec.ceil_integer()), timer_label);
    }
    if(frame_counter == MAX_FRAMES){
        frame_counter = 0;
        state = 2;
        global.text_gen_fix_8x16().generate(0,0, "TIME'S UP!", timer_label);
    }

    if(state == 1){
        if(frame_counter == 60){
            return SceneType::MINIGAMES_SELECTOR;
        }
        return bn::nullopt;
    }
    if(state == 2){
        if(frame_counter == 60){
            return SceneType::HOUSE;
        }
        return bn::nullopt;
    }

    if(hit_bar.is_active()){
        hit_bar.update();
    }

    if(bn::keypad::a_pressed()){
        if(hit_bar.is_active()){
            hit_bar.deactivate();
            arm_moving = true;
            rotation_speed = 4;
            rotation_angle = 5;
        }
    }

    if(arm_moving){
        rotation_angle += rotation_speed;
        if(rotation_angle >= 90){  // Hit the stone
            rotation_angle = 90;
            rotation_speed = -rotation_speed;

            int damage = hit_bar.hit();
            shake_camera(damage*3);
            stone->break_stone(damage);
            global.text_gen_fix_8x16().generate(-40, -8, "Hit", hit_label);
            global.text_gen_fix_8x16().generate(-40, 8, bn::to_string<3>(damage), hit_label);
        
            bn::sound_items::stone_hit.play();

            if(stone->is_breaked()){
                stones_breaked++;
                update_stones_breaked_label();
                replacing_stone = true;
            }
        }
        if(replacing_stone) stone->sprite.set_y(stone->sprite.y() + 3);
    
        if(rotation_angle <= 5){  // Return to initial position
            hit_label.clear();
            rotation_angle = 5;
            arm_moving = false;

            if(stones_breaked >= stones_to_break){
                state = 1;
                frame_counter = 0;

                // Calculate the tip for finish early
                int coins_tip = (bn::fixed(MAX_FRAMES-frame_counter)/60).ceil_integer();
                coins_tip *= global.game_difficulty();

                global.add_game_money(coins_tip);
                global.text_gen_fix_8x16().generate(0,0, "+" + bn::to_string<10,int>(coins_tip), timer_label);         
            }else{
                if(replacing_stone){
                    stone = bn::make_unique<Stone>(8);
                    stone->sprite.set_camera(camera);
                }
                hit_bar.activate();
            }

            replacing_stone = false;
        }
        rotate_steve_arm(rotation_angle);
    }

    update_camera();

    return bn::nullopt;
}

void Minigame_construction_4::rotate_steve_arm(int angle){
    if(angle < 0) angle = 360 + (angle % 360);
    angle = angle % 360;

    spr_steve_arm.set_x(steve_arm_joint.x() - (bn::degrees_sin(angle)*32));
    spr_steve_arm.set_y(steve_arm_joint.y() - (bn::degrees_cos(angle)*32));

    spr_steve_arm.set_rotation_angle(angle);

    spr_pickaxe.set_x(steve_arm_joint.x() - (bn::degrees_sin(angle)*64));
    spr_pickaxe.set_y(steve_arm_joint.y() - (bn::degrees_cos(angle)*64));

    angle = (angle*2) % 360;
    angle = (angle+270) % 360;

    spr_pickaxe.set_rotation_angle(angle);
}

void Minigame_construction_4::shake_camera(int frames){
    camera_shake_frames = frames;
    camera_shake_counter = 0;
    shaking_camera = true;
}

void Minigame_construction_4::update_camera(){
    if(shaking_camera){
        if(camera_shake_counter < camera_shake_frames){
            camera_shake_counter++;
            camera.set_x(global.rng().get_int(3)-1);
            camera.set_y(global.rng().get_int(3)-1);
        }else{
            camera_shake_counter = 0;
            shaking_camera = false;
            camera.set_x(0);
            camera.set_y(0);
        }
    }
}

void Minigame_construction_4::update_stones_breaked_label(){
    stones_breaked_label.clear();
    bn::string<6> label;
    label += bn::to_string<3>(stones_breaked);
    label += "/";
    label += bn::to_string<3>(stones_to_break);
    global.text_gen_fix_8x16().generate(-104, -64, label, stones_breaked_label);
}