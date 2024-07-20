#include "scenes/Minigame_bar_1.h"

#include "bn_log.h"
#include "bn_core.h"

Minigame_bar_1::Minigame_bar_1(Global_variables &_global) :
    global(_global),
    bar_behind_bg(bn::regular_bg_item(bn::regular_bg_items::bg_bar_behind.tiles_item(),bn::regular_bg_items::bg_bar_behind.palette_item(),bn::regular_bg_map_item(bar_behind_cells[0],bn::size(32,32))).create_bg(8,48)),  // 8,48
    bar_behind_map(bar_behind_bg.map()),
    liquid_spr(bn::sprite_items::liquid.create_sprite(-72, 4)),
    beer_mug(bn::sprite_item(bn::sprite_shape_size(64,64),bn::sprite_tiles_item(beer_mug_tiles,bn::bpp_mode::BPP_4),bn::sprite_items::beer_mug.palette_item()).create_sprite(-64, 32)),
    beer_mug_tiles_ptr(beer_mug.tiles()),
    mug_hand(bn::sprite_items::mug_hand.create_sprite(-40, 37)),
    hand_position(0),
    curr_level(0),
    frames_held(0),
    drinks_served(0),
    level_arrow(bn::sprite_items::arrow_8x8.create_sprite(-96, 0)),
    state(0),
    frame_counter(0),
    overflow_label_lifespan(0),
    tip_label_lifespan(0)
    {    
    BN_LOG("CPU After",bn::core::current_cpu_usage());

    liquid_spr.set_visible(false);
    level_arrow.set_rotation_angle(90);

    drinks_to_serve = int(bn::fixed(global.game_difficulty())*bn::fixed(1.5));
    drinks_to_serve = bn::max(2,drinks_to_serve);

    next_drink();
    start_bar_begin_bg();

    BN_LOG("CPU After",bn::core::current_cpu_usage());
}

bn::optional<SceneType> Minigame_bar_1::update(){
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
        global.text_gen_fix_8x16().generate(0,16, "You Lose", win_label);
    }

    if(state == 1){
        if(frame_counter >= 90) return SceneType::MINIGAMES_SELECTOR;
        return bn::nullopt;
    }
    if(state == 2){
        if(frame_counter >= 60) return SceneType::HOUSE;
        return bn::nullopt;
    }

    if(overflow_label_lifespan > 0){
        if(--overflow_label_lifespan <= 0) overflow_label.clear();
    }

    if(tip_label_lifespan > 0){
        if(--tip_label_lifespan <= 0) tip_label.clear();
    }

    if(bn::keypad::left_pressed() && !filling){
        hand_position = bn::max(hand_position - 1,0);
        set_hand_in_position();
    }

    if(bn::keypad::right_pressed() && !filling){
        hand_position = bn::min(hand_position + 1,2);
        set_hand_in_position();
    }

    if(bn::keypad::b_pressed()){
        start_filling();
    }

    if(bn::keypad::b_held()){
        update_filling();
    }

    if(bn::keypad::b_released()){
        stop_filling();
    }

    if(bn::keypad::a_pressed() && !sent_mug_label.empty()){
        show_and_add_tip();

        add_drink_to_finished(type_of_drink);
        drinks_served++;
        sent_mug_label.clear();
        stop_filling();
        next_drink();

        if(drinks_served >= drinks_to_serve){
            state = 1;
            frame_counter = 0;
            global.text_gen_fix_8x16().generate(0, 0, "Nice job!", win_label);
            beer_mug.set_visible(false);
            mug_hand.set_visible(false);
            liquid_spr.set_visible(false);
            level_arrow.set_visible(false);
        }
    }

    return bn::nullopt;
}

void Minigame_bar_1::set_hand_in_position(){
    beer_mug.set_x(-64 + (hand_position * 72));
    mug_hand.set_x(-40 + (hand_position * 72));
    liquid_spr.set_x(-72 + (hand_position * 72));
    level_arrow.set_x(-96 + (hand_position * 72));
}

void Minigame_bar_1::start_filling(){
    if(type_of_drink != hand_position) return; // Is not on the correct drink
    if(curr_level<531) liquid_spr.set_visible(true);
    level_arrow.set_visible(false);
    filling = true;

    fill_sound = bn::sound_items::water_falling.play();
}

void Minigame_bar_1::update_filling(){
    if(filling){
        frames_held++;
        auto frames_held_fixed = bn::fixed(frames_held)*bn::fixed(0.1);
        frames_held_fixed *= frames_held_fixed;
        curr_level += bn::min(bn::fixed(40),frames_held_fixed);
        curr_level = bn::min(curr_level,bn::fixed(550));
        set_liquid_level();

        if(fill_sound.has_value() && !fill_sound.value().active()){
            fill_sound = bn::sound_items::water_falling.play();
        }

        if(curr_level > 530){ // Whoops overflow
            sent_mug_label.clear();
            overflow_label_lifespan = 60;
            global.text_gen_fix_8x16().generate(0, -76, "Overflow!!!", overflow_label);
            global.text_gen_fix_8x16().generate(0, -68, "GET ANOTHER MUG", overflow_label);
            stop_filling();
            load_empty_mug();
        }
    }
}

void Minigame_bar_1::stop_filling(){
    if(!filling) return;
    filling = false;
    frames_held = 0;
    liquid_spr.set_visible(false);
    level_arrow.set_visible(true);
    if(curr_level >= min_level_needed && sent_mug_label.empty()){
        global.text_gen_fix_8x16().generate(80, 64, "Sent Mug", sent_mug_label);
        global.text_gen_var_8x8().generate(80, 72, "(A)", sent_mug_label);
    }

    if(fill_sound.has_value()){
        fill_sound.value().stop();
        fill_sound.reset();
    }
}

void Minigame_bar_1::next_drink(){
    min_level_needed = 340 + global.rng().get_unbiased_int(121);
    type_of_drink = global.rng().get_unbiased_int(3);
    
    auto liquid_palette = liquid_spr.palette();
    liquid_palette.set_color(1, DRINKS_COLOR[type_of_drink]);

    auto beer_mug_palette = beer_mug.palette();
    beer_mug_palette.set_color(5, DRINKS_COLOR[type_of_drink]);

    load_empty_mug();
}

void Minigame_bar_1::load_empty_mug(){
    bn::memory::copy(bn::sprite_items::beer_mug.tiles_item().tiles_ref()[0], beer_mug_tiles.size(), beer_mug_tiles[0]);
    beer_mug_tiles_ptr.reload_tiles_ref();

    curr_level = 0;
    frames_held = 0;
    sent_mug_label.clear();
    set_hand_in_position();
    set_level_arrow();

    // Display the type of drink to serve
    type_drink_label.clear();
    global.text_gen_fix_8x16().generate(-24, 72, "Serve:", type_drink_label);
    global.text_gen_fix_8x16().set_left_alignment();
    global.text_gen_fix_8x16().generate(0, 72, DRINKS_NAMES[type_of_drink], type_drink_label);
    global.text_gen_fix_8x16().set_center_alignment();
}

void Minigame_bar_1::set_liquid_level(){
    if(curr_level < 10) return;

    // 8 rows of tiles in total -- > top and bottom of 20, and 6 middle of 80 -> 520 + 30 extra = 550 max
    int first_index_row = 7*8;

    // Botton of the mug
    beer_mug_tiles[first_index_row + 1] = beer_mug_liquid_a_tiles_bn_gfxTiles[0 + (curr_level >20? 2:1)];
    beer_mug_tiles[first_index_row + 2] = beer_mug_liquid_a_tiles_bn_gfxTiles[4 + (curr_level >20? 2:1)];
    beer_mug_tiles[first_index_row + 3] = beer_mug_liquid_a_tiles_bn_gfxTiles[4 + (curr_level >20? 2:1)];
    beer_mug_tiles[first_index_row + 4] = beer_mug_liquid_a_tiles_bn_gfxTiles[8 + (curr_level >20? 2:1)];

    // Middle of the mug
    int curr_top;
    for(int i = 0; i < 6; i++){
        first_index_row = (6-i)*8;
        if(curr_level >= ((80*i)+20)){ 
            curr_top = curr_level >= (80*(i+1)+20)? 8: (int(curr_level-20)%80)/10;
            //Red rows
            for(int j=0; j <curr_top; j++){
                beer_mug_tiles[first_index_row+1].data[7-j] = 0x55555555;
                beer_mug_tiles[first_index_row+2].data[7-j] = 0x55555555;
                beer_mug_tiles[first_index_row+3].data[7-j] = 0x55555555;
                beer_mug_tiles[first_index_row+4].data[7-j] = 0x55555555;
            }
        }else
            break;
    }

    // Top of the mug
    if(curr_level >= 510 && curr_level <= 530){
        beer_mug_tiles[1] = beer_mug_liquid_a_tiles_bn_gfxTiles[12 + (curr_level >520? 2:1)];
        beer_mug_tiles[2] = beer_mug_liquid_a_tiles_bn_gfxTiles[16 + (curr_level >520? 2:1)];
        beer_mug_tiles[3] = beer_mug_liquid_a_tiles_bn_gfxTiles[16 + (curr_level >520? 2:1)];
        beer_mug_tiles[4] = beer_mug_liquid_a_tiles_bn_gfxTiles[20 + (curr_level >520? 2:1)];
    }else if(curr_level > 530){
        beer_mug_tiles[0] = beer_mug_liquid_a_tiles_bn_gfxTiles[25];
        beer_mug_tiles[1] = beer_mug_liquid_a_tiles_bn_gfxTiles[15];
        beer_mug_tiles[2] = beer_mug_liquid_a_tiles_bn_gfxTiles[19];
        beer_mug_tiles[3] = beer_mug_liquid_a_tiles_bn_gfxTiles[19];
        beer_mug_tiles[4] = beer_mug_liquid_a_tiles_bn_gfxTiles[23];
        beer_mug_tiles[8] = beer_mug_liquid_a_tiles_bn_gfxTiles[29];
        beer_mug_tiles[16] = beer_mug_liquid_a_tiles_bn_gfxTiles[27];
    }

    beer_mug_tiles_ptr.reload_tiles_ref(); 
}

void Minigame_bar_1::set_level_arrow(){
    level_arrow.set_y(57 - (min_level_needed)/10);
}

void Minigame_bar_1::start_bar_begin_bg(){
    // Same as original draw
    bn::memory::copy(bn::regular_bg_items::bg_bar_behind.map_item().cells_ref(), 32*32, bar_behind_cells[0]);
    
    // Erase the top mugs
    bn::memory::set_half_words(0,8,&bar_behind_cells[0]);
    bn::memory::set_half_words(0,8,&bar_behind_cells[32]);

    int i2;
    for(int i=0; i<drinks_to_serve; i++){
        i2 = i*2;
        bar_behind_cells[64+i2] = 1;
        bar_behind_cells[65+i2] = 2; // 64 + i2 + 1
        bar_behind_cells[96+i2] = 9;
        bar_behind_cells[97+i2] = 10; // 96 + i2 + 1
    }

    bar_behind_map.reload_cells_ref();
}

void Minigame_bar_1::add_drink_to_finished(int type){
    if(drinks_served >= drinks_to_serve) return;
    int i2 = drinks_served*2;
    bar_behind_cells[64+i2] = 3 + type*2;
    bar_behind_cells[65+i2] = 4 + type*2; // 64 + i2 + 1
    bar_behind_cells[96+i2] = 11 + type*2;
    bar_behind_cells[97+i2] = 12 + type*2; // 96 + i2 + 1
    bar_behind_map.reload_cells_ref();
}

void Minigame_bar_1::show_and_add_tip(){
    // 520 is the maximum level(actually 530 but XD)
    bn::fixed curr_level_min = bn::min(bn::fixed(520),curr_level);
    bn::fixed tip_fixed = (curr_level_min-min_level_needed)/(520-min_level_needed);
    tip_fixed *= global.game_difficulty()+5;

    int tip = int(tip_fixed);
    global.add_game_money(tip);
    tip_label.clear();
    if(tip > 0) {
        global.text_gen_fix_8x16().generate(0,-16, "+" + bn::to_string<5,int>(tip), tip_label);    
        tip_label_lifespan = 60;
    }
}