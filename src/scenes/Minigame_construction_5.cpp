#include "scenes/Minigame_construction_5.h"

#include "bn_core.h"
#include "bn_log.h"

Minigame_construction_5::Minigame_construction_5(Global_variables& _global) : 
    global(_global),
    dirt_bg_ptr(bn::regular_bg_item(bn::regular_bg_tiles_items::bg_construction_fd_2_tiles,bn::regular_bg_tiles_items::bg_construction_fd_2_tiles_palette, bn::regular_bg_map_item(dirt_cells[0], bn::size(columns,rows))).create_bg(8,48)),
    dirt_map_ptr(dirt_bg_ptr.map()),
    holes_bg_ptr(bn::regular_bg_item(bn::regular_bg_tiles_items::bg_construction_fd_1_tiles,bn::regular_bg_tiles_items::bg_construction_fd_1_tiles_palette, bn::regular_bg_map_item(holes_cells[0], bn::size(columns,rows))).create_bg(8,48)),
    holes_map_ptr(holes_bg_ptr.map()),
    tile_selected_border(bn::sprite_items::construction_5_border.create_sprite(0,0)),
    digger_sprite(bn::sprite_items::digger.create_sprite(0,0)),
    digger_dirt_sprite(bn::sprite_items::digger_dirt.create_sprite(-8,0)),
    digger_direction(-1,0),
    digger_has_dirt(false)
    {
    BN_LOG("CPU Before: ", bn::core::current_cpu_usage());
    bn::memory::clear(n_cells, dirt_cells[0]);
    bn::memory::clear(n_cells, holes_cells[0]);
    bn::memory::set_words(1,holes_map_len, holes_map);
    bn::memory::set_words(0,holes_map_len, coins_buried_map);

    fill_dirt();

    int pairs_of_holes_to_generate = 1 + int(global.game_difficulty() * bn::fixed(1.2f));  
    num_heaps = pairs_of_holes_to_generate;
    num_holes = pairs_of_holes_to_generate;

    bn::forward_list<bn::point,150> pos_candidates;
    for(int i = 1; i<v_16x16_columns-1; i++){
        for(int j = 1; j<v_16x16_rows-1; j++){
            pos_candidates.push_front(bn::point(i,j));
        }
    }
    make_n_random_holes(pos_candidates,pairs_of_holes_to_generate, HOLE_TYPE::HOLE);
    make_n_random_holes(pos_candidates,pairs_of_holes_to_generate, HOLE_TYPE::HEAP);

    // Hide coins in the flat sections
    int coins_to_bury = 10 + global.game_difficulty() * 5;

    for(int i = 0; i < coins_to_bury; i++){
        int index = global.rng().get_unbiased_int(pos_candidates.size());
        auto it = pos_candidates.before_begin();
        for(int j = 0; j < index; j++) ++it;
        auto it_next = it;
        bn::point act_pos = *(++it_next);
        pos_candidates.erase_after(it);

        coins_buried_map[act_pos.y()*holes_map_columns + act_pos.x()] = 1;
    }

    holes_map_ptr.reload_cells_ref();
    dirt_map_ptr.reload_cells_ref();
    
    BN_LOG("CPU After: ", bn::core::current_cpu_usage());
}

bn::optional<SceneType> Minigame_construction_5::update(){
    #ifdef MINIGAMES_TEST
        if(bn::keypad::start_pressed()){
            return SceneType::MINIGAMES_SELECTOR;
        }
    #endif

    frame_counter++;
    if(frame_counter <= MAX_FRAMES && frame_counter >= 600){  // Display after 5 seconds
        timer_label.clear();
        bn::fixed act_sec(MAX_FRAMES-frame_counter);
        act_sec /= 60;
        global.text_gen_fix_8x16().generate(80,-60,bn::to_string<3,int>(act_sec.ceil_integer()), timer_label);
    }
    if(frame_counter == MAX_FRAMES){
        frame_counter = 0;
        state = 2;
        if(is_all_nivelated()) state = 1;
        global.text_gen_fix_8x16().generate(0,0, "TIME'S UP!", timer_label);
        if(state == 2) global.text_gen_fix_8x16().generate(0,8, "You lose!", win_lose_label);
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

    if(bn::keypad::left_held() || bn::keypad::right_held() || bn::keypad::up_held() || bn::keypad::down_held()) digger_direction = bn::point(0,0);
    
    if(bn::keypad::left_held()){
        digger_direction.set_x(-1);
    }
    if(bn::keypad::right_held()){
        digger_direction.set_x(1);
    }
    if(bn::keypad::up_held()){
        digger_direction.set_y(-1);
    }
    if(bn::keypad::down_held()){
        digger_direction.set_y(1);
    }

    if(bn::keypad::a_held()){
        digger_sprite.set_x(digger_sprite.x() + digger_direction.x());
        digger_sprite.set_y(digger_sprite.y() + digger_direction.y());
    }

    if(digger_sprite.x() < -104) digger_sprite.set_x(-104);
    if(digger_sprite.x() > 104) digger_sprite.set_x(104);
    if(digger_sprite.y() < -64) digger_sprite.set_y(-64);
    if(digger_sprite.y() > 64) digger_sprite.set_y(64);

    digger_dirt_sprite.set_x(digger_sprite.x() - 8);
    digger_dirt_sprite.set_y(digger_sprite.y());

    // Set the tile selected border to the tile where the digger is pointing
    {
        bn::fixed_point true_digger_pos = bn::fixed_point(digger_sprite.x() + 8, digger_sprite.y() + 12);
        bn::point tile_16x16_digger_pos = bn::point(int((true_digger_pos.x() + 112) / 16), int((true_digger_pos.y() + 72) / 16)); 
        tile_selected = bn::point(tile_16x16_digger_pos.x() + digger_direction.x(), tile_16x16_digger_pos.y() + digger_direction.y());
        tile_selected.set_x(bn::max(0, bn::min(v_16x16_columns-1, tile_selected.x())));
        tile_selected.set_y(bn::max(0, bn::min(v_16x16_rows-1, tile_selected.y())));

        tile_selected_border.set_position(tile_selected.x()*16 - 112, tile_selected.y()*16 - 72);
    }

    if(bn::keypad::b_pressed()){
        HOLE_TYPE& act_hole = get_hole_map(tile_selected.x(),tile_selected.y());

        if(digger_has_dirt && act_hole != HOLE_TYPE::HEAP){ // HOLE or FLAT
            bn::sound_items::fill.play();
            
            digger_has_dirt = false;
            digger_dirt_sprite.set_tiles(bn::sprite_items::digger_dirt.tiles_item().create_tiles(0));
            
            if(act_hole == HOLE_TYPE::HOLE) num_holes--;
            if(act_hole == HOLE_TYPE::FLAT) num_heaps++;

            act_hole = HOLE_TYPE(int(act_hole) + 1);
            set_holes_cells(tile_selected.x(),tile_selected.y(),act_hole);
            holes_map_ptr.reload_cells_ref();            
        }else if(!digger_has_dirt && act_hole != HOLE_TYPE::HOLE){ // HEAP or FLAT
            bn::sound_items::dig.play();
            
            digger_has_dirt = true;
            digger_dirt_sprite.set_tiles(bn::sprite_items::digger_dirt.tiles_item().create_tiles(1));
            
            if(act_hole == HOLE_TYPE::HEAP) num_heaps--;
            if(act_hole == HOLE_TYPE::FLAT) num_holes++;

            if(coins_buried_map[tile_selected.y()*holes_map_columns + tile_selected.x()] == 1){
                coins.push_back(bn::pair<bn::sprite_ptr,int>(bn::sprite_items::axolotcoin.create_sprite(tile_selected.x()*16 - 112, tile_selected.y()*16 - 72), 60));
                coins.back().first.set_z_order(1);
                bn::sound_items::coin_sound.play();
                global.add_game_money(1);
                coins_buried_map[tile_selected.y()*holes_map_columns + tile_selected.x()] = 0;
            }

            act_hole = HOLE_TYPE(int(act_hole) - 1);
            set_holes_cells(tile_selected.x(),tile_selected.y(),act_hole);
            holes_map_ptr.reload_cells_ref();
        } 

        if(is_all_nivelated()){
            global.text_gen_fix_8x16().generate(0, -72, "All terrain nivelated!", win_lose_label);
        }else{
            win_lose_label.clear();
        }
    }

    // Update the coins
    for(auto it = coins.begin(); it != coins.end();){
        auto &act_coin = *it;
        act_coin.second--;
        if(act_coin.second <= 30) act_coin.first.set_visible(act_coin.second % 4 <= 1);

        if(act_coin.second <= 0) {
            it = coins.erase(it);
        }else{
            ++it;
        }
    }

    return bn::nullopt;
}

bn::regular_bg_map_cell& Minigame_construction_5::get_holes_cell(int x, int y){
    return holes_cells[y*32 + x];
}

HOLE_TYPE& Minigame_construction_5::get_hole_map(int x, int y){
    return holes_map[y*holes_map_columns + x];
}

void Minigame_construction_5::set_holes_cells(int x, int y, HOLE_TYPE type){
    int x2 = x * 2;
    int y2 = y * 2;
    bn::regular_bg_map_cell& act_cell_0_0 = get_holes_cell(x2,y2);
    bn::regular_bg_map_cell& act_cell_0_1 = get_holes_cell(x2,y2+1);
    bn::regular_bg_map_cell& act_cell_1_0 = get_holes_cell(x2+1,y2);
    bn::regular_bg_map_cell& act_cell_1_1 = get_holes_cell(x2+1,y2+1);

    switch (type){
    case HOLE_TYPE::HOLE:
        act_cell_0_0 = 1;
        act_cell_0_1 = 2;
        act_cell_1_0 = 1 | TILE_H_FLIP;
        act_cell_1_1 = 2 | TILE_H_FLIP;
        break;

    case HOLE_TYPE::FLAT:
        act_cell_0_0 = 0;
        act_cell_0_1 = 0;
        act_cell_1_0 = 0;
        act_cell_1_1 = 0;
        break;

    default:  // HEAP
        act_cell_0_0 = 3;
        act_cell_0_1 = 5;
        act_cell_1_0 = 4;
        act_cell_1_1 = 6;
        break;
    }

    holes_map[y*holes_map_columns + x] = type;
}

void Minigame_construction_5::make_n_random_holes(bn::forward_list<bn::point,150> &pos_candidates, int n, HOLE_TYPE type){    
    for(int i = 0; i < n; i++){
        int index = global.rng().get_unbiased_int(pos_candidates.size());
        auto it = pos_candidates.before_begin();
        for(int j = 0; j < index; j++) ++it;
        auto it_next = it;
        bn::point act_pos = *(++it_next);
        pos_candidates.erase_after(it);

        set_holes_cells(act_pos.x(),act_pos.y(),type);
    }
}

bool Minigame_construction_5::is_all_nivelated(){
    return num_holes == 0 && num_heaps == 0;
}

bn::regular_bg_map_cell& Minigame_construction_5::get_dirt_cell(int x, int y){
    return dirt_cells[y*32 + x];
}

void Minigame_construction_5::set_dirt_cells(int x, int y, DIRT_TYPE type){
    int x2 = x * 2;
    int y2 = y * 2;
    bn::regular_bg_map_cell& act_cell_0_0 = get_dirt_cell(x2,y2);
    bn::regular_bg_map_cell& act_cell_0_1 = get_dirt_cell(x2,y2+1);
    bn::regular_bg_map_cell& act_cell_1_0 = get_dirt_cell(x2+1,y2);
    bn::regular_bg_map_cell& act_cell_1_1 = get_dirt_cell(x2+1,y2+1);

    switch (type){    
    case DIRT_TYPE::DOT_1:
        act_cell_0_0 = 12;
        act_cell_0_1 = 0;
        act_cell_1_0 = 0;
        act_cell_1_1 = 0;
        break;

    case DIRT_TYPE::DOT_2:
        act_cell_0_0 = 0;
        act_cell_0_1 = 0;
        act_cell_1_0 = 13;
        act_cell_1_1 = 14;
        break;

    case DIRT_TYPE::DOT_3:
        act_cell_0_0 = 15;
        act_cell_0_1 = 16;
        act_cell_1_0 = 0;
        act_cell_1_1 = 0;
        break;

    case DIRT_TYPE::DOT_4:
        act_cell_0_0 = 17;
        act_cell_0_1 = 18;
        act_cell_1_0 = 0;
        act_cell_1_1 = 19;
        break;

    case DIRT_TYPE::DOT_5:
        act_cell_0_0 = 20;
        act_cell_0_1 = 0;
        act_cell_1_0 = 0;
        act_cell_1_1 = 0;
        break;

    case DIRT_TYPE::DOT_6:
        act_cell_0_0 = 0;
        act_cell_0_1 = 22;
        act_cell_1_0 = 21;
        act_cell_1_1 = 0;
        break;

    case DIRT_TYPE::DOT_7:
        act_cell_0_0 = 0;
        act_cell_0_1 = 24;
        act_cell_1_0 = 23;
        act_cell_1_1 = 25;
        break;

    case DIRT_TYPE::DOT_8:
        act_cell_0_0 = 0;
        act_cell_0_1 = 0;
        act_cell_1_0 = 0;
        act_cell_1_1 = 26;
        break;

    default:  // CLEAN
        act_cell_0_0 = 0;
        act_cell_0_1 = 0;
        act_cell_1_0 = 0;
        act_cell_1_1 = 0;
        break;
    }
}

void Minigame_construction_5::fill_dirt(){
    // Fill top row and botton row with leafs
    for(int i = 1; i < v_16x16_columns-1; i++){
        for(int j=0; j < 2; j++){
            int x2 = i * 2;
            int y2 = j == 0? 0 : (v_16x16_rows-1)*2;
            bn::regular_bg_map_cell& act_cell_0_0 = get_dirt_cell(x2,y2);
            bn::regular_bg_map_cell& act_cell_0_1 = get_dirt_cell(x2,y2+1);
            bn::regular_bg_map_cell& act_cell_1_0 = get_dirt_cell(x2+1,y2);
            bn::regular_bg_map_cell& act_cell_1_1 = get_dirt_cell(x2+1,y2+1);

            int type = global.rng().get_unbiased_int(2);

            if(type == 0){
                act_cell_0_0 = j == 0? 4 : 0;
                act_cell_0_1 = j == 0? 0 : 4 |TILE_V_FLIP;
                act_cell_1_0 = j == 0? 5 : 0;
                act_cell_1_1 = j == 0? 0 : 5 |TILE_V_FLIP;
            }else{
                act_cell_0_0 = j == 0? 6 : 0;
                act_cell_0_1 = j == 0? 0 : 6 | TILE_V_FLIP;
                act_cell_1_0 = j == 0? 7 : 0;
                act_cell_1_1 = j == 0? 0 : 7 | TILE_V_FLIP;
            }
        }
    }

    // Fill left column and right column with leafs
    for(int i = 0; i < 2; i++){
        for(int j=1; j < v_16x16_rows-1; j++){
            int x2 = i == 0? 0 : (v_16x16_columns-1)*2;
            int y2 = j * 2;
            bn::regular_bg_map_cell& act_cell_0_0 = get_dirt_cell(x2,y2);
            bn::regular_bg_map_cell& act_cell_0_1 = get_dirt_cell(x2,y2+1);
            bn::regular_bg_map_cell& act_cell_1_0 = get_dirt_cell(x2+1,y2);
            bn::regular_bg_map_cell& act_cell_1_1 = get_dirt_cell(x2+1,y2+1);

            int type = global.rng().get_unbiased_int(2);

            if(type == 0){
                act_cell_0_0 = i == 0? 8 : 0;
                act_cell_0_1 = i == 0? 9 : 0;
                act_cell_1_0 = i == 0? 0 : 8 | TILE_H_FLIP;
                act_cell_1_1 = i == 0? 0 : 9 | TILE_H_FLIP;
            }else{
                act_cell_0_0 = i == 0? 10 : 0;
                act_cell_0_1 = i == 0? 11 : 0;
                act_cell_1_0 = i == 0? 0 : 10 | TILE_H_FLIP;
                act_cell_1_1 = i == 0? 0 : 11 | TILE_H_FLIP;
            }
        }
    }

    // Set the four corners to be a leaf
    {
        int x2 = 0;
        int y2 = 0;
        bn::regular_bg_map_cell& act_cell_0_0 = get_dirt_cell(x2,y2);
        bn::regular_bg_map_cell& act_cell_0_1 = get_dirt_cell(x2,y2+1);
        bn::regular_bg_map_cell& act_cell_1_0 = get_dirt_cell(x2+1,y2);
        bn::regular_bg_map_cell& act_cell_1_1 = get_dirt_cell(x2+1,y2+1);
        act_cell_0_0 = 1;
        act_cell_0_1 = 3;
        act_cell_1_0 = 2;
        act_cell_1_1 = 0;
    }
    {
        int x2 = (v_16x16_columns-1)*2;
        int y2 = 0;
        bn::regular_bg_map_cell& act_cell_0_0 = get_dirt_cell(x2,y2);
        bn::regular_bg_map_cell& act_cell_0_1 = get_dirt_cell(x2,y2+1);
        bn::regular_bg_map_cell& act_cell_1_0 = get_dirt_cell(x2+1,y2);
        bn::regular_bg_map_cell& act_cell_1_1 = get_dirt_cell(x2+1,y2+1);
        act_cell_0_0 = 2 | TILE_H_FLIP;
        act_cell_0_1 = 0 | TILE_H_FLIP;
        act_cell_1_0 = 1 | TILE_H_FLIP;
        act_cell_1_1 = 3 | TILE_H_FLIP;
    }
    {
        int x2 = (v_16x16_columns-1)*2;
        int y2 = (v_16x16_rows-1)*2;
        bn::regular_bg_map_cell& act_cell_0_0 = get_dirt_cell(x2,y2);
        bn::regular_bg_map_cell& act_cell_0_1 = get_dirt_cell(x2,y2+1);
        bn::regular_bg_map_cell& act_cell_1_0 = get_dirt_cell(x2+1,y2);
        bn::regular_bg_map_cell& act_cell_1_1 = get_dirt_cell(x2+1,y2+1);
        act_cell_0_0 = 0 | TILE_H_FLIP | TILE_V_FLIP;
        act_cell_0_1 = 2 | TILE_H_FLIP | TILE_V_FLIP;
        act_cell_1_0 = 3 | TILE_H_FLIP | TILE_V_FLIP;
        act_cell_1_1 = 1 | TILE_H_FLIP | TILE_V_FLIP;
    }
    {
        int x2 = 0;
        int y2 = (v_16x16_rows-1)*2;
        bn::regular_bg_map_cell& act_cell_0_0 = get_dirt_cell(x2,y2);
        bn::regular_bg_map_cell& act_cell_0_1 = get_dirt_cell(x2,y2+1);
        bn::regular_bg_map_cell& act_cell_1_0 = get_dirt_cell(x2+1,y2);
        bn::regular_bg_map_cell& act_cell_1_1 = get_dirt_cell(x2+1,y2+1);
        act_cell_0_0 = 3 | TILE_V_FLIP;
        act_cell_0_1 = 1 | TILE_V_FLIP;
        act_cell_1_0 = 0 | TILE_V_FLIP;
        act_cell_1_1 = 2 | TILE_V_FLIP;
    }

    for(int i = 1; i < v_16x16_columns-1; ++i){
        for(int j = 1; j < v_16x16_rows-1; ++j){
            // Set the cell to a random dirt type, 80% be clean dirt and 20% be a dot
            int is_dot = global.rng().get_unbiased_int(5);
            if(is_dot == 0) set_dirt_cells(i,j,DIRT_TYPE(global.rng().get_unbiased_int(8) + 1));
        }
    }

    dirt_map_ptr.reload_cells_ref();
}