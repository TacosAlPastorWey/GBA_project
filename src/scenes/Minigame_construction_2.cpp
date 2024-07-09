#include "scenes/Minigame_construction_2.h"

#include "bn_log.h"

Pipes_bg::Pipes_bg(int _size,Global_variables& _global) :
    global(_global),
    bg_ptr( bn::regular_bg_item(bn::regular_bg_tiles_items::bg_pipes_tiles,bn::regular_bg_tiles_items::bg_pipes_tiles_palette, bn::regular_bg_map_item(cells[0], bn::size(columns,rows))).create_bg(0,0)),
    map_ptr(bg_ptr.map()),
    size(_size),
    real_size(size * 2)
    {

    bn::memory::clear(n_cells, cells[0]);

    // Set the init and end pipes
    {
        bn::regular_bg_map_cell& act_cell_0_1 = get_cell(0,-1);
        bn::regular_bg_map_cell& act_cell_1_1 = get_cell(1,-1);
    
        act_cell_0_1 = 2 | TILE_V_FLIP;
        act_cell_1_1 = 2 | TILE_H_FLIP | TILE_V_FLIP;
    }
    {  
        bn::regular_bg_map_cell& act_cell_0_0 = get_cell(real_size,real_size-2);
        bn::regular_bg_map_cell& act_cell_0_1 = get_cell(real_size,real_size-1);

        act_cell_0_0 = 3 | TILE_V_FLIP;
        act_cell_0_1 = 3;
    }

    make_puzzle();

    if(is_solved())
        rotate_pipe(0,0); // Make it unsolved XDXDXDXD
    else
        map_ptr.reload_cells_ref();
}

void Pipes_bg::rotate_pipe(int x, int y){
    if(pipes_map[x][y] < 2) // STRAIGHT
        pipes_map[x][y] = PIPE_TYPE::PIPE_TYPE((pipes_map[x][y] + 1) % 2);
    else // CURVE
        pipes_map[x][y] = PIPE_TYPE::PIPE_TYPE(bn::max((pipes_map[x][y] + 1) % 6, 2));

    build_pipe(x,y);
    map_ptr.reload_cells_ref();
}

bool Pipes_bg::is_solved(){
    // Check if size-1,size-1 is correct
    if(pipes_map[size-1][size-1] != PIPE_TYPE::STRAIGHT_2 && pipes_map[size-1][size-1] != PIPE_TYPE::CURVE_1){
        return false;
    }

    bn::point act(0,0);
    int dir = 1; // From where it comes 1 up, 2 right, 3 down, 4 left
    for(int i= 0; i<52; i++){
        // Check if act is on (size-1,size-1)
        if(act.x() == size-1 && act.y() == size-1){
            return true;
        }

        switch (dir){
        case 1:
            switch (pipes_map[act.x()][act.y()]){
            case PIPE_TYPE::STRAIGHT_1:
                dir = 1;
                act.set_y(act.y() + 1);
                break;
            case PIPE_TYPE::CURVE_1:
                dir = 4;
                act.set_x(act.x() + 1);
                break;
            case PIPE_TYPE::CURVE_4:
                dir = 2;
                act.set_x(act.x() - 1);
                break;
            default:
                return false;
            }
            break;

        case 2:
            switch (pipes_map[act.x()][act.y()]){
            case PIPE_TYPE::STRAIGHT_2:
                dir = 2;
                act.set_x(act.x() - 1);
                break;
            case PIPE_TYPE::CURVE_1:
                dir = 3;
                act.set_y(act.y() - 1);
                break;
            case PIPE_TYPE::CURVE_2:
                dir = 1;
                act.set_y(act.y() + 1);
                break;
            default:
                return false;
            }
            break;

        case 3:
            switch (pipes_map[act.x()][act.y()]){
            case PIPE_TYPE::STRAIGHT_1:
                dir = 3;
                act.set_y(act.y() - 1);
                break;
            case PIPE_TYPE::CURVE_2:
                dir = 4;
                act.set_x(act.x() + 1);
                break;
            case PIPE_TYPE::CURVE_3:
                dir = 2;
                act.set_x(act.x() - 1);
                break;
            default:
                return 0;
            }
            break;
        
        default: // 4
            switch (pipes_map[act.x()][act.y()]){
            case PIPE_TYPE::STRAIGHT_2:
                dir = 4;
                act.set_x(act.x() + 1);
                break;
            case PIPE_TYPE::CURVE_3:
                dir = 1;
                act.set_y(act.y() + 1);
                break;
            case PIPE_TYPE::CURVE_4:
                dir = 3;
                act.set_y(act.y() - 1);
                break;
            default:
                return 0;
            }
            break;
        }

        // Check if act is on invalid position
        if(act.x() < 0 || act.x() >= size || act.y() < 0 || act.y() >= size){
            return false;
        }
    }

    return false;
}

void Pipes_bg::make_puzzle(){
    bn::array<bn::array<int,7>, 7>visit;

    for(int i=0; i<7; i++){
        for(int j=0; j<7; j++){
            visit[i][j] = 0; // bn::memory::clear crash sometimes if used here
        }
    }

    bn::point act(0,0);
    visit[0][0] = 2;
    while(!(act.x() == size-1 && act.y() == size-1)){

        if(act.x() == size-1){
            act.set_y(act.y() + 1);
            visit[act.x()][act.y()] = 2;
            continue;
        }

        if(act.y() == size-1){
            act.set_x(act.x() + 1);
            visit[act.x()][act.y()] = 2;
            continue;
        }

        if(global.rng().get_unbiased_int(2)){
            act.set_x(act.x() + 1);
            visit[act.x()][act.y()] = 2;    
        }else{
            act.set_y(act.y() + 1);
            visit[act.x()][act.y()] = 2;
        }
    }
    visit[size-1][size-1] = 2;

    // Create the pipes part of the solution
    bool vertical, horizontal;
    int i2, j2,k;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if(visit[i][j] == 2){
                vertical = false;
                for(k=0; k<2; k++){
                    i2 = i + mov[k][0];
                    j2 = j + mov[k][1];
                    if(i2 >= 0 && i2 < size && j2 >= 0 && j2 < size && visit[i2][j2] == 2){
                        vertical = true;
                    }
                }
                horizontal = false;
                for(k=2; k<4; k++){
                    i2 = i + mov[k][0];
                    j2 = j + mov[k][1];
                    if(i2 >= 0 && i2 < size && j2 >= 0 && j2 < size && visit[i2][j2] == 2){
                        horizontal = true;
                    }
                }

                if(vertical && horizontal){
                    pipes_map[i][j] = PIPE_TYPE::PIPE_TYPE(global.rng().get_unbiased_int(4)+2); // 2,3,4,5
                }else{
                    pipes_map[i][j] = PIPE_TYPE::PIPE_TYPE(global.rng().get_unbiased_int(2)); // 0,1
                }
            }else{
                if(global.rng().get_unbiased_int(2)){
                    pipes_map[i][j] = PIPE_TYPE::PIPE_TYPE(global.rng().get_unbiased_int(2)); // 0,1
                }else{
                    pipes_map[i][j] = PIPE_TYPE::PIPE_TYPE(global.rng().get_unbiased_int(4)+2); // 2,3,4,5
                }
            }
        }
    }

    
    // Set the [0][0] pipe
    if(visit[0][1] == 2){
        pipes_map[0][0] = PIPE_TYPE::PIPE_TYPE(global.rng().get_unbiased_int(2)); // 0,1
    }else{
        pipes_map[0][0] = PIPE_TYPE::PIPE_TYPE(global.rng().get_unbiased_int(4)+2); // 2,3,4,5
    }

    // Set the [size-1][size-1] pipe
    if(visit[size-2][size-1] == 2){
        pipes_map[size-1][size-1] = PIPE_TYPE::PIPE_TYPE(global.rng().get_unbiased_int(2)); // 0,1
    }else{
        pipes_map[size-1][size-1] = PIPE_TYPE::PIPE_TYPE(global.rng().get_unbiased_int(4)+2); // 2,3,4,5
    }

    // Build the pipes from the tiles
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            build_pipe(i,j);
        }
    }
}

void Pipes_bg::build_pipe(int x,int y){
    int x2 = x * 2;
    int y2 = y * 2;

    bn::regular_bg_map_cell& act_cell_0_0 = get_cell(x2,y2);
    bn::regular_bg_map_cell& act_cell_0_1 = get_cell(x2,y2+1);
    bn::regular_bg_map_cell& act_cell_1_0 = get_cell(x2+1,y2);
    bn::regular_bg_map_cell& act_cell_1_1 = get_cell(x2+1,y2+1);

    switch (pipes_map[x][y]){
    case PIPE_TYPE::STRAIGHT_1:
        act_cell_0_0 = 2;
        act_cell_0_1 = 2 | TILE_V_FLIP;
        act_cell_1_0 = 2 | TILE_H_FLIP;
        act_cell_1_1 = 2 | TILE_H_FLIP | TILE_V_FLIP;
        break;
    
    case PIPE_TYPE::STRAIGHT_2:
        act_cell_0_0 = 3 | TILE_V_FLIP;
        act_cell_0_1 = 3;
        act_cell_1_0 = 3 | TILE_V_FLIP | TILE_H_FLIP;
        act_cell_1_1 = 3 | TILE_H_FLIP;
        break;

    case PIPE_TYPE::CURVE_1:
        act_cell_0_0 = 2;
        act_cell_0_1 = 5;
        act_cell_1_0 = 4 | TILE_H_FLIP;
        act_cell_1_1 = 3 | TILE_H_FLIP;
        break;

    case PIPE_TYPE::CURVE_2:
        act_cell_0_0 = 5 | TILE_V_FLIP;
        act_cell_0_1 = 2 | TILE_V_FLIP;
        act_cell_1_0 = 3 | TILE_V_FLIP | TILE_H_FLIP;
        act_cell_1_1 = 4 | TILE_V_FLIP | TILE_H_FLIP;
        break;
    
    case PIPE_TYPE::CURVE_3:
        act_cell_0_0 = 3 | TILE_V_FLIP;
        act_cell_0_1 = 4 | TILE_V_FLIP;
        act_cell_1_0 = 5 | TILE_H_FLIP | TILE_V_FLIP;
        act_cell_1_1 = 2 | TILE_H_FLIP | TILE_V_FLIP;
        break;

    default: // PIPE_TYPE::CURVE_4
        act_cell_0_0 = 4;
        act_cell_0_1 = 3;
        act_cell_1_0 = 2 | TILE_H_FLIP;
        act_cell_1_1 = 5 | TILE_H_FLIP;
        break;
    }
}

bn::regular_bg_map_cell& Pipes_bg::get_cell(int x, int y){
    x += x_offset;
    y += y_offset;
    return cells[y*32 + x];
}


Minigame_construction_2::Minigame_construction_2(Global_variables& _global) : 
    global(_global),
    bg_map(bg_cells[0], bn::size(32,32)),
    background(bn::regular_bg_items::bg_pipes.create_bg(8,48)),
    pipes_bg(new Pipes_bg(global.game_difficulty()+2,global)),
    cursor_limit(global.game_difficulty()+1,global.game_difficulty()+1),
    selected_pipe(0,0),
    cursor(bn::sprite_items::border_16.create_sprite(-48,-40)),
    cursor_palette(cursor.palette()),
    finished(false),
    frame_counter(-1)
    {

    { //Set the dark tiles of the background
        bn::memory::copy(bn::regular_bg_items::bg_pipes.map_item().cells_ref(),bn::regular_bg_items::bg_pipes.map_item().cells_count(), bg_cells[0]);
        int size = global.game_difficulty() + 2;
        int xoffset = 8;
        int yoffset = 4;
        int lim = size * 2 -2, actx,acty;
        for(int i = 0; i<=lim; i+=2){
            for(int j = 0; j<=lim; j+=2){
                actx = i + xoffset;
                acty = j + yoffset;
                bg_cells[bg_map.cell_index(actx,acty)] = 5;
                bg_cells[bg_map.cell_index(actx,acty+1)] = 7;
                bg_cells[bg_map.cell_index(actx+1,acty)] = 6;
                bg_cells[bg_map.cell_index(actx+1,acty+1)] = 8;
            }
        }
        background.set_map(bg_map);
    }

    if(pipes_bg->is_solved()){
        cursor_palette.set_color(1, bn::color(0,31,0));
    }else{
        cursor_palette.set_color(1, bn::color(0,0,31));
    }

    auto map_item = bn::regular_bg_items::bg_pipes.map_item();
    BN_LOG(map_item.cell(0,0));
    
}

bn::optional<SceneType> Minigame_construction_2::update(){
    #ifdef MINIGAMES_TEST
    if(bn::keypad::start_pressed()){
        return SceneType::MINIGAMES_SELECTOR;
    }
    #endif

    if(finished){
        frame_counter++;
        if(frame_counter >= 120){
            return SceneType::MINIGAMES_SELECTOR;
        }
        return bn::nullopt;
    }

    frame_counter++;
    if(frame_counter <= MAX_FRAMES && frame_counter >= 300){
        timer_label.clear();
        bn::fixed act_sec(MAX_FRAMES-frame_counter);
        act_sec /= 60;
        global.text_gen_fix_8x16().generate(80,-60,bn::to_string<3,int>(act_sec.ceil_integer()), timer_label);
    }

    if(frame_counter == MAX_FRAMES){
        global.text_gen_fix_8x16().generate(0,0, "TIME'S UP!", timer_label);
    }

    if(frame_counter > MAX_FRAMES){
        if(frame_counter >= MAX_FRAMES + 120){
            return SceneType::HOUSE;
        }
        return bn::nullopt;
    }

    if(bn::keypad::a_pressed()){
        pipes_bg->rotate_pipe(selected_pipe.x(),selected_pipe.y());

        if(pipes_bg->is_solved()){
            cursor_palette.set_color(1, bn::color(0,31,0));
            finished = true;

            int coins_tip = (bn::fixed(MAX_FRAMES-frame_counter)/60).ceil_integer();
            coins_tip *= global.game_difficulty();

            global.add_game_money(coins_tip);
            global.text_gen_fix_8x16().generate(0,0, "+" + bn::to_string<10,int>(coins_tip), timer_label);
            
            frame_counter = 0;
            bn::sound_items::nice.play();
            return bn::nullopt;
        }else{
            cursor_palette.set_color(1, bn::color(0,0,31));
        }
    }

    if(bn::keypad::left_pressed()){
        if(selected_pipe.x() > 0){
            selected_pipe.set_x(selected_pipe.x() - 1);
            cursor.set_x(selected_pipe.x() * 16 -48);
        }
    }

    if(bn::keypad::right_pressed()){
        if(selected_pipe.x() < cursor_limit.x()){
            selected_pipe.set_x(selected_pipe.x() + 1);
            cursor.set_x(selected_pipe.x() * 16 -48);
        }
    }

    if(bn::keypad::up_pressed()){
        if(selected_pipe.y() > 0){
            selected_pipe.set_y(selected_pipe.y() - 1);
            cursor.set_y(selected_pipe.y() * 16 -40);
        }
    }

    if(bn::keypad::down_pressed()){
        if(selected_pipe.y() < cursor_limit.y()){
            selected_pipe.set_y(selected_pipe.y() + 1);
            cursor.set_y(selected_pipe.y() * 16 -40);
        }
    }


    return bn::nullopt;
}
