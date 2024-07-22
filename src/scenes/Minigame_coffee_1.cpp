#include "scenes/Minigame_coffee_1.h"

Minigame_coffee_1::Minigame_coffee_1(Global_variables &_global) :
    global(_global),
    coffee_bg(bn::regular_bg_items::coffee_table.create_bg(8,48)),
    coffee_mug(bn::sprite_items::coffee_mug.create_sprite(0,16)),
    coffee_mug_handle(bn::sprite_items::coffee_mug_handle.create_sprite(40,16)),
    expected_coffee_decoration(bn::sprite_items::coffee_figures.create_sprite(0,16)),
    expected_decorations_tiles(expected_coffee_decoration.tiles().tiles_ref().value()),
    coffee_decoration_colors{
        bn::color(31,31,31),
        bn::color(29,29,27),
        bn::color(29,25,1),
        bn::color(31,0,0),
        bn::color(25,25,25),
        bn::color(31,31,31),
        bn::color(31,31,31),
        bn::color(31,31,31),
        bn::color(31,31,31),
        bn::color(31,31,31),
        bn::color(31,31,31),
        bn::color(31,31,31),
        bn::color(31,31,31),
        bn::color(31,31,31),
        bn::color(31,31,31),
        bn::color(31,31,31)
    },
    coffee_decoration_tiles_ptr(bn::sprite_tiles_ptr::allocate(64,bn::bpp_mode::BPP_4)),
    coffee_decoration_tiles_vram(coffee_decoration_tiles_ptr.vram().value()),
    coffee_decoration(bn::sprite_ptr::create(0,16,bn::sprite_shape_size(64,64),coffee_decoration_tiles_ptr,bn::sprite_palette_ptr::create(bn::sprite_palette_item(coffee_decoration_colors,bn::bpp_mode::BPP_4)))),
    pixels_painted_correctly(0),
    pixels_painted(0),

    coffee_pointer_tiles_ptr(bn::sprite_tiles_ptr::allocate(64,bn::bpp_mode::BPP_4)),
    coffee_pointer_tiles_vram(coffee_pointer_tiles_ptr.vram().value()),
    coffee_pointer(bn::sprite_ptr::create(0,16,bn::sprite_shape_size(64,64),coffee_pointer_tiles_ptr,coffee_decoration.palette())),

    bar_fig(bn::sprite_items::coffee_bar_fig.create_sprite(0,64)), // 0,16 is the position of the sprite in the tilesheet
    bar_fig_arrow(bn::sprite_items::arrow_8x8.create_sprite(-32,56)),
    bar_cream(bn::sprite_items::coffee_bar_cream.create_sprite(-48,16)), // 0,16 is the position of the sprite in the tilesheet
    bar_cream_arrow(bn::sprite_items::arrow_8x8.create_sprite(-60,46)),
    fig_state(0), // 0 not enough, 1 correct, 2 too much
    sending(false),
    milk_jug(bn::sprite_items::milk_jug.create_sprite(64,0)),
    milk_jug_hand(bn::sprite_items::milk_jug_hand.create_sprite(96,0)),
    pouring_milk(false),
    frame_counter(0)
    {
    toggle_HUD(false);
    init_mug();
}

bn::optional<SceneType> Minigame_coffee_1::update(){
    #ifdef MINIGAMES_TEST
    if(bn::keypad::select_pressed()){
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
        global.text_gen_fix_8x16().generate(0,16, "You Lose", win_lose_label);
    }

    if(state == 2){
        if(frame_counter >= 60) return SceneType::HOUSE;
        return bn::nullopt;
    }

    if(sending){
        op_frames++;
        coffee_mug.set_x(coffee_mug.x() + 2);
        coffee_mug_handle.set_x(coffee_mug_handle.x() + 2);
        coffee_decoration.set_x(coffee_decoration.x() + 2);
        if(op_frames == 60) perfect_label.clear();
        if(op_frames == 120){
            return SceneType::MINIGAMES_SELECTOR;
        }
        return bn::nullopt;
    }
    if(starting_over){
        op_frames++;
        coffee_mug.set_y(coffee_mug.y() + 2);
        coffee_mug_handle.set_y(coffee_mug_handle.y() + 2);
        coffee_decoration.set_y(coffee_decoration.y() + 2);
        if(coffee_mug.y() >= 112){
            starting_over = false;
            init_mug();
        }
        return bn::nullopt;
    }
    if(moving_mug){
        coffee_mug.set_x(coffee_mug.x() + 2);
        coffee_mug_handle.set_x(coffee_mug_handle.x() + 2);
        coffee_decoration.set_x(coffee_decoration.x() + 2);
        if(coffee_mug.x() == 0){
            moving_mug = false;
            toggle_HUD(true);
        }
        return bn::nullopt;
    }

    if(bn::keypad::left_held()) dir.set_x(-1);
    if(bn::keypad::right_held()) dir.set_x(dir.x()+1);
    if(bn::keypad::up_held()) dir.set_y(-1);
    if(bn::keypad::down_held()) dir.set_y(dir.y()+1);

    if(dir.x() != 0 || dir.y() != 0){
        fill_circle(coffee_pointer_tiles_vram,circle_center.x(),circle_center.y(),circle_radius,0,false);
        move_cursor();
        fill_circle(coffee_pointer_tiles_vram,circle_center.x(),circle_center.y(),circle_radius,2,false);
    }

    if(bn::keypad::a_pressed()) pouring_sound = bn::sound_items::water_falling.play();

    if(bn::keypad::a_held()){
        fill_circle(coffee_decoration_tiles_vram,circle_center.x(),circle_center.y(),circle_radius,1,true);
        pouring_milk = true;
        rotate_milk_jug(bn::fixed(45));
        update_bars();

        if(fig_state == 0 && bar_cream_ratio <= bn::fixed(0.57) && bar_fig_ratio >= bn::fixed(0.5)){
            aux_label.clear();
            global.text_gen_fix_8x16().generate(88, 16, "Send it!", aux_label);
            global.text_gen_fix_8x16().generate(88, 32, "(start)", aux_label);
            fig_state = 1;
        }else if(fig_state != 2 && bar_cream_ratio > bn::fixed(0.57)){
            // Need another mug
            fig_state = 2;
            aux_label.clear();
            global.text_gen_fix_8x16().generate(88, 0, "Start", aux_label);
            global.text_gen_fix_8x16().generate(88, 16, "again!!", aux_label);
            global.text_gen_fix_8x16().generate(88, 32, "(start)", aux_label);
        }

        if(pouring_sound.has_value() && !pouring_sound.value().active()){
            pouring_sound = bn::sound_items::water_falling.play();
        }
    }

    if(bn::keypad::a_released()){
        pouring_milk = false;
        rotate_milk_jug(bn::fixed(12.5));
        if(pouring_sound.has_value()){
            pouring_sound.value().stop();
            pouring_sound.reset();
        }
    }

    if(bn::keypad::start_pressed()){
        if(!sending && fig_state == 1){
            sending = true;
            op_frames = 0;
            aux_label.clear();
            toggle_HUD(false);
            if(bar_fig_ratio >= bn::fixed(0.88)){
                global.text_gen_fix_8x16().generate(0, 16, "Perfect!", perfect_label);
                global.text_gen_fix_8x16().generate(0, 32, "+20", perfect_label);
                global.add_game_money(20);
            }
            // Calculate tip bonus
            int coins_tip = (bn::fixed(MAX_FRAMES-frame_counter)/60).ceil_integer();
            coins_tip *= global.game_difficulty();
            global.add_game_money(coins_tip);
            global.text_gen_fix_8x16().generate(0,48, "Time BONUS", win_lose_label);
            global.text_gen_fix_8x16().generate(0,64, "+" + bn::to_string<10,int>(coins_tip), win_lose_label);
            
            frame_counter = 0;
        }else if(!starting_over && fig_state == 2){
            starting_over = true;
            op_frames = 0;
            aux_label.clear();
            toggle_HUD(false);
        }
    }

    return bn::nullopt;
}

void Minigame_coffee_1::init_mug(){
    bn::memory::set_words(0x00000000, 64*8, coffee_decoration_tiles_vram.data()[0].data);
    bn::memory::set_words(0x00000000, 64*8, coffee_pointer_tiles_vram.data()[0].data);

    expected_coffee_decoration.set_tiles(bn::sprite_items::coffee_figures.tiles_item().create_tiles(global.game_difficulty()-1));
    expected_decorations_tiles = expected_coffee_decoration.tiles().tiles_ref().value();

    pixels_painted = 0;
    pixels_painted_correctly = 0;
    fig_state = 0;
    sending = false;
    starting_over = false;
    moving_mug = true;
    op_frames = 0;
    pouring_milk = false;

    coffee_mug.set_x(-154);
    coffee_mug_handle.set_x(-114);
    coffee_mug.set_y(16);
    coffee_decoration.set_x(-154);
    coffee_decoration.set_y(16);

    bar_cream_arrow.set_rotation_angle(90);

    milk_jug.set_x(circle_center.x() + 4);
    milk_jug.set_y(circle_center.y() - 32);
    rotate_milk_jug(bn::fixed(12.5));

    update_bars();

    dir = bn::fixed_point(0,0);
}

void Minigame_coffee_1::toggle_HUD(bool show){
    bar_cream.set_visible(show);
    bar_cream_arrow.set_visible(show);
    bar_fig.set_visible(show);
    bar_fig_arrow.set_visible(show);
    coffee_pointer.set_visible(show);
    milk_jug.set_visible(show);
    milk_jug_hand.set_visible(show);
    expected_coffee_decoration.set_visible(show);
}

void Minigame_coffee_1::move_cursor(){
    if(dir.x() != 0 && dir.y() != 0){
        dir.set_x(dir.x()*bn::fixed(0.70710678118)); // 1/sqrt(2)
        dir.set_y(dir.y()*bn::fixed(0.70710678118));
    }

    circle_center.set_x(circle_center.x()+dir.x()*bn::fixed(0.5));
    circle_center.set_y(circle_center.y()+dir.y()*bn::fixed(0.5));

    bn::fixed x_theta = circle_center.x()-32;
    bn::fixed y_theta = circle_center.y()-32;
    bn::fixed distance_pow2 = x_theta*x_theta + y_theta*y_theta;

    bn::fixed r = 30 - circle_radius;

    if(distance_pow2 > r*r){ // the cursor is outside the inside of the mug
        bn::fixed radians = bn::atan2(int(y_theta),int(x_theta));
        bn::pair<bn::fixed,bn::fixed> cos_n_sin = bn::sin_and_cos(radians);
        circle_center.set_x(bn::fixed(32) + cos_n_sin.second*r);
        circle_center.set_y(bn::fixed(32) + cos_n_sin.first*r);
    }

    dir = bn::fixed_point(0,0);

    // Move the jag and hand
    milk_jug.set_x(circle_center.x() + 4);
    milk_jug.set_y(circle_center.y() - 32);
    if(pouring_milk) rotate_milk_jug(bn::fixed(45));
    else rotate_milk_jug(bn::fixed(12.5));
}

void Minigame_coffee_1::rotate_milk_jug(bn::fixed degrees){
    milk_jug.set_rotation_angle(degrees);
    milk_jug_hand.set_rotation_angle(degrees);

    // Move the hand cause the rotation center is on the center of the milk jug
    bn::pair<bn::fixed,bn::fixed> sin_n_cos = bn::degrees_sin_and_cos(degrees);
    milk_jug_hand.set_x(milk_jug.x() + sin_n_cos.second*32);
    milk_jug_hand.set_y(milk_jug.y() - sin_n_cos.first*32);
}

void Minigame_coffee_1::fill_circle(bn::span<bn::tile> &destination,bn::fixed c_x,bn::fixed c_y,bn::fixed radius,int color,bool pixel_count){
    bn::fixed x0 = c_x-radius; // Top left corner
    bn::fixed y0 = c_y-radius;
    bn::fixed x1 = c_x+radius; // Bottom right corner
    bn::fixed y1 = c_y+radius;
    bn::fixed radius2 = radius*radius;
    bn::fixed distance_f_c;
    int tile_index,color_exp;
    for(int x = bn::max(int(x0),0); x<=bn::min(int(x1),63); x++){
        for(int y= bn::max(int(y0),0); y<=bn::min(int(y1),63); y++){
            distance_f_c = (x-c_x)*(x-c_x) + (y-c_y)*(y-c_y);
            if(distance_f_c <= radius2){
                tile_index = (y/8)*8 + x/8; // (y/8)*8 looks stupid but its integer division
                if(pixel_count){
                    if((destination.data()[tile_index].data[y%8]&(0xF << ((x%8)*4)))== 0){ 
                        pixels_painted++;
                        color_exp = (expected_decorations_tiles.data()[tile_index].data[y%8]&(0xF << ((x%8)*4)));
                        color_exp >>= ((x%8)*4);
                        if(color_exp == color) pixels_painted_correctly++;
                    }
                }
                destination.data()[tile_index].data[y%8] &= ~ (0xF << ((x%8)*4));
                destination.data()[tile_index].data[y%8] |= color << ((x%8)*4);
            }
        }
    }
}

void Minigame_coffee_1::update_bars(){
    bar_cream_ratio = bn::fixed(pixels_painted)/2790; // 2790 - COFFEE_DECORATIONS_PIXELS[global.game_difficulty()-1]
    bar_fig_ratio = bn::fixed(pixels_painted_correctly)/COFFEE_DECORATIONS_PIXELS[global.game_difficulty()-1]; // 0-4

    bar_cream_arrow.set_y(46 - 56*bar_cream_ratio);
    bar_fig_arrow.set_x(-32 + 64*bar_fig_ratio);
}