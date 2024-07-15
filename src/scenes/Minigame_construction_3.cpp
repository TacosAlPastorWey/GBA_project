#include "scenes/Minigame_construction_3.h"

Building_bg::Building_bg() : 
    bg_ptr(bn::regular_bg_items::bg_construction_3.create_bg(8, -48)),
    map_item(cells[0], bn::size(32, 32)),
    crane_level(0)
    {
    bn::memory::copy(bn::regular_bg_items::bg_construction_3.map_item().cells_ref(), n_cells, cells[0]);

    bg_ptr.set_map(map_item);
}

void Building_bg::raise_crane(){
}

Floor_spr::Floor_spr(bn::fixed_point pos, int _width, bn::camera_ptr& camera) : 
    floor_tiles_ptrs{bn::sprite_tiles_ptr::allocate(32, bn::bpp_mode::BPP_4), bn::sprite_tiles_ptr::allocate(32, bn::bpp_mode::BPP_4)},
    vram_tiles_ptrs{floor_tiles_ptrs[0].vram().value(), floor_tiles_ptrs[1].vram().value()},
    sprites{
        bn::sprite_ptr::create(-40, 48, bn::sprite_shape_size(64,32),floor_tiles_ptrs[0],bn::sprite_palette_ptr::create(bn::sprite_items::building_tiles.palette_item())),
        bn::sprite_ptr::create(-8, 48, bn::sprite_shape_size(64,32),floor_tiles_ptrs[1],bn::sprite_palette_ptr::create(bn::sprite_items::building_tiles.palette_item()))
    },
    width(_width),
    left_move(false)
    {

    // Construct the tiles for the floor (it uses two sprites)   
    bn::memory::set_words(0, vram_tiles_ptrs[0].size_bytes()/4, vram_tiles_ptrs[0].data()); // Clear the tiles first (set them to 0 a.k.a. clear tile)
    bn::memory::set_words(0, vram_tiles_ptrs[1].size_bytes()/4, vram_tiles_ptrs[1].data());

    sprites[0].set_horizontal_flip(true);
    
    int ind_width = width/2, i = 0;
    floor_pos = pos;
    sprites[0].set_position(bn::fixed_point(floor_pos.x()-32,floor_pos.y()));
    sprites[1].set_position(bn::fixed_point(floor_pos.x()+32,floor_pos.y()));
    sprites[0].set_camera(camera);
    sprites[1].set_camera(camera);

    auto tiles_ref = building_tiles_bn_gfxTiles; // Get the tiles from ROM
    while(ind_width >= 8){
        for(int j=0; j<3; j++){
            // Full 8x8 tile
            bn::memory::copy(tiles_ref[0+j], 1, vram_tiles_ptrs[0][j*8 + i]);
            bn::memory::copy(tiles_ref[0+j], 1, vram_tiles_ptrs[1][j*8 + i]);
        }
        ind_width -= 8;
        i++;
    }
    if(ind_width > 0){
        for(int j=0; j<3; j++){
            // Partial 8x8 tile (Some pixels are transparent)
            bn::memory::copy(tiles_ref[3*(8-ind_width)+j], 1, vram_tiles_ptrs[0][j*8 + i]);
            bn::memory::copy(tiles_ref[3*(8-ind_width)+j], 1, vram_tiles_ptrs[1][j*8 + i]);
        }
        i++;
    }
}

void Floor_spr::update(){
    if(left_move) move_x(-2);
    else move_x(2);

    if(floor_pos.x() < -72-(48-(width/2))){
        move_x(2);
        left_move = false;
    }
    if(floor_pos.x() > 24+(48-(width/2))){
        move_x(-2);
        left_move = true;
    }
}

Utils::Fixed_line Floor_spr::get_x_line(){
    return Utils::Fixed_line(floor_pos, bn::fixed(width));
}

void Floor_spr::move_x(int dx){
    floor_pos.set_x(floor_pos.x() + dx);
    sprites[0].set_x(sprites[0].x() + dx);
    sprites[1].set_x(sprites[1].x() + dx);
}

Minigame_construction_3::Minigame_construction_3(Global_variables& _global) : 
    global(_global),
    camera(bn::camera_ptr::create(0, 0)),
    camera_speed_y(-2.0f),
    camera_delta_y(0),
    theme_map_item(theme_cells[0], bn::size(32, 32)),
    theme_bg(bn::regular_bg_items::daytime_theme.create_bg(-8, -48)),
    building_bg(),
    steve_spr(bn::sprite_items::character.create_sprite(48, -20)),
    floors_count(1),
    state(0),
    frame_counter(-1)
    {
    // Change the theme background
    bn::memory::copy(bn::regular_bg_items::daytime_theme.map_item().cells_ptr()[32*6], 32*6, theme_cells[0]);
    bn::memory::copy(bn::regular_bg_items::daytime_theme.map_item().cells_ptr()[32*12], 32*6, theme_cells[32*6]);
    bn::memory::copy(bn::regular_bg_items::daytime_theme.map_item().cells_ptr()[32*12], 32*10, theme_cells[32*12]);
    bn::memory::copy(bn::regular_bg_items::daytime_theme.map_item().cells_ptr()[32*12], 32*6, theme_cells[32*22]);
    bn::memory::copy(bn::regular_bg_items::daytime_theme.map_item().cells_ptr()[32*22], 32*4, theme_cells[32*28]);
    theme_bg.set_camera(camera);
    theme_bg.set_map(theme_map_item);
    
    // Construct the first floor
    int initial_width = 96 - 8*(global.game_difficulty()-1);
    floor_sprites.push_back(Floor_spr(bn::fixed_point(-24,72),initial_width,camera));

    // Construct the second floor
    floor_sprites.push_back(Floor_spr(bn::fixed_point(-96,48),initial_width,camera));

    // Set the goal and text sprites
    global.text_gen_fix_8x16().set_z_order(-1);
    floors_goal = 3 + int(bn::fixed(1.5f)*global.game_difficulty());
    set_floors_count_text();
}

Minigame_construction_3::~Minigame_construction_3(){
    global.text_gen_fix_8x16().set_z_order(0); // Back to default
}   

bn::optional<SceneType> Minigame_construction_3::update(){
    #ifdef MINIGAMES_TEST
    if(bn::keypad::start_pressed()){
        return SceneType::MINIGAMES_SELECTOR;
    }
    #endif

    frame_counter++;
    if(frame_counter <= MAX_FRAMES && frame_counter >= 300){
        timer_label.clear();
        bn::fixed act_sec(MAX_FRAMES-frame_counter);
        act_sec /= 60;
        global.text_gen_fix_8x16().generate(80,-60,bn::to_string<3,int>(act_sec.ceil_integer()), timer_label);
    }
    if(frame_counter == MAX_FRAMES){
        state = 2;
        frame_counter = 0;
        global.text_gen_fix_8x16().generate(0,0, "TIME'S UP!", timer_label);
    }

    if(state == 1){
        if(frame_counter == 60){
            return SceneType::MINIGAMES_SELECTOR;
        }
        return bn::nullopt;
    }else if (state == 2){
        if(frame_counter == 60){
            return SceneType::HOUSE;
        }
        return bn::nullopt;
    }

    // Update the last floor pushed
    floor_sprites.back().update();

    // Stack the current floor and create a new one
    if(bn::keypad::a_pressed()){
        auto opt_intersect = floor_sprites.back().get_x_line().x_intersection(floor_sprites[floor_sprites.size()-2].get_x_line());
        if(opt_intersect.has_value()){
            bn::sound_items::floor_clicking.play();
            auto intersection = opt_intersect.value();

            floors_count++;
            set_floors_count_text();
            if(floors_count == floors_goal){
                state = 1;
                frame_counter = 0;

                // Calculate the tip for finish early
                int coins_tip = (bn::fixed(MAX_FRAMES-frame_counter)/60).ceil_integer();
                coins_tip *= global.game_difficulty();

                global.add_game_money(coins_tip);
                global.text_gen_fix_8x16().generate(0,0, "+" + bn::to_string<10,int>(coins_tip), timer_label);         
            }

            bn::fixed new_x = (intersection.p1.x() + intersection.p2.x())/2;
            bn::fixed y_pos = floor_sprites.back().floor_pos.y();
            int new_width = int(bn::abs(intersection.p1.x() - intersection.p2.x()));
            
            floor_sprites.back() = Floor_spr(bn::fixed_point(new_x,y_pos),new_width,camera);
            if(state == 0) floor_sprites.push_back(Floor_spr(bn::fixed_point(-96,y_pos-24),new_width,camera));
            if(floors_count > 2) camera_delta_y += 24;
        }else{
            state = 2; //GAMEOVER
            frame_counter = 0;
            global.text_gen_fix_8x16().generate(0,0, "You LOSE", timer_label);
        }
    }

    // Update the camera
    if(camera_delta_y > 0){
        camera_delta_y += camera_speed_y;
        camera.set_y(camera.y() + camera_speed_y);
    }
    if(camera.y() <= -96.0f){
        theme_bg.set_y(48);
        theme_bg.remove_camera();
    }


    if(floor_sprites.size() > 5){
        // Delete the first floor
        floor_sprites.erase(floor_sprites.begin());
    }

    return bn::nullopt;
}

void Minigame_construction_3::set_floors_count_text(){
    floors_count_sprs.clear();
    bn::string<8> floors_count_str;
    floors_count_str += bn::to_string<4>(floors_count);
    floors_count_str += "/";
    floors_count_str += bn::to_string<4>(floors_goal);
    global.text_gen_fix_8x16().generate(88, 50, "Goal", floors_count_sprs);
    global.text_gen_fix_8x16().generate(88, 66, floors_count_str, floors_count_sprs);
}