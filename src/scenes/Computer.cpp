#include "scenes/Computer.h"

Computer::Computer(Global_variables& _global) : 
    computer_background(bn::regular_bg_items::bg_computer.create_bg(-8, 0)),
    computer_palette(computer_background.palette()),
    mouse_arrow(bn::sprite_items::mouse_arrow.create_sprite(0, 0)),
    tab_selected(0),
    text_generator(bn::sprite_font(bn::sprite_items::fixed_8x16_font)),
    global(_global)
    {   
    computer_background.set_priority(1);
    mouse_arrow.set_bg_priority(0);
    mouse_arrow.set_z_order(1);

    tab.reset(new Jobs_tab(text_generator,global));
}

bn::optional<SceneType> Computer::update() {
    if(bn::keypad::b_pressed()) {
        return SceneType::HOUSE;
    }

    // Move the mouse arrow
    if(bn::keypad::left_held()) {
        mouse_arrow.set_x(mouse_arrow.x() - bn::fixed(1.2f));
    }
    if(bn::keypad::right_held()) {
        mouse_arrow.set_x(mouse_arrow.x() + bn::fixed(1.2f));
    }
    if(bn::keypad::up_held()) {
        mouse_arrow.set_y(mouse_arrow.y() - bn::fixed(1.2f));
    }
    if(bn::keypad::down_held()) {
        mouse_arrow.set_y(mouse_arrow.y() + bn::fixed(1.2f));
    }

    if(mouse_arrow.x() < -112) mouse_arrow.set_x(-112);
    if(mouse_arrow.x() > 128) mouse_arrow.set_x(128);
    if(mouse_arrow.y() < -72) mouse_arrow.set_y(-72);
    if(mouse_arrow.y() > 90) mouse_arrow.set_y(90);

    bn::fixed_point mouse_top = bn::fixed_point(mouse_arrow.x() - 8, mouse_arrow.y() - 10);

    if(bn::keypad::a_pressed()) {
        bn::sound_items::click_down.play();

        for(int i = 0; i < 4; i++) {
            if(tab_selected != i && Utils::is_inside(mouse_top, tabs_rect[i]) ) {
                tab_selected = i;
                switch (tab_selected) {
                case 0:
                    tab.reset(new Jobs_tab(text_generator,global));
                    break;
                case 1:
                    tab.reset(new Casino_tab());
                    break;
                case 2:
                    tab.reset(new Itch_tab());
                    break;
                default:
                    tab.reset(new SongStorm_tab());
                    break;
                }
                computer_palette.set_colors(bn::regular_bg_items::bg_computer.palette_item());
                computer_palette.set_color(4+i, bn::color(27,29,31)); 
                break;
            }
        }
    }

    if(bn::keypad::a_released()) bn::sound_items::click_up.play();

    tab.get()->update(mouse_top);

    return bn::nullopt;
}

// ----- Jobs Tab ----- 

Jobs_tab::Jobs_tab(bn::sprite_text_generator& _text_generator,Global_variables& _global) :
    global(_global),
    tab_background(bn::regular_bg_items::bg_computer_2.create_bg(8, 78)),
    tab_palette(tab_background.palette()),
    text_generator(_text_generator),
    scroll_bar(bn::sprite_items::scroll_bar.create_sprite(112, -34)),
    scroll_bar_palette(scroll_bar.palette())
    {

    tab_background.set_priority(3);
    text_generator.set_center_alignment();
    text_generator.set_palette_item(bn::sprite_items::mouse_arrow.palette_item());

    text_generator.generate(0, -40, "My jobs", title_sprites);
    text_generator.set_left_alignment();

    for(int i = 0; i < MINIGAMES_COLLECTIONS; i++) {
        if(global.save().minigames_collections(i) & 1)
            jobs_icons.push_back(bn::sprite_items::jobs_icons.create_sprite(-80, -3+(32*i),i+1));
        else
            jobs_icons.push_back(bn::sprite_items::jobs_icons.create_sprite(-80, -3+(32*i),0));
    }

    for(int i = 0; i < MINIGAMES_COLLECTIONS; i++) {
        if(global.save().minigames_collections(i) & 1){
                text_generator.generate(-50,(32*i) -12, Definitions::COLLECTIONS_NAMES[i], jobs_info_name[i]);
                bn::string<8> count;
                bn::ostringstream count_stream(count);
                count_stream << Utils::count_set_bits(global.save().minigames_collections(i)) << "/30";
                text_generator.generate(-40, (32*i) + 4, count, jobs_info_number[i]);
        }else{
                text_generator.generate(-50, (32*i) -12, "??????????", jobs_info_name[i]);
        }
    }
}

const bn::fixed_point Jobs_tab::jobs_lines_limits[MINIGAMES_COLLECTIONS] = {
    bn::fixed_point(0,61),
    bn::fixed_point(0,93),
    bn::fixed_point(0,125),
    bn::fixed_point(0,157),
    bn::fixed_point(32,189),
    bn::fixed_point(64,221),
    bn::fixed_point(96,253),
    bn::fixed_point(128,285),
    bn::fixed_point(160,317),
    bn::fixed_point(192,349),
    bn::fixed_point(224,381),
    bn::fixed_point(256,413),
    bn::fixed_point(288,445),
    bn::fixed_point(320,477),
    bn::fixed_point(352,509)
};

Jobs_tab::~Jobs_tab() {
    text_generator.set_palette_item(bn::sprite_items::fixed_8x16_font.palette_item());
}

void Jobs_tab::update(bn::fixed_point mouse_pos) {
    if(Utils::is_inside(mouse_pos, scroll_bar.x(), scroll_bar.y(), 8, 16)) {
        scroll_bar_palette.set_color(1, bn::color(0,0,0));
        scroll_bar_palette.set_color(2, bn::color(10,10,10));

        if(bn::keypad::a_pressed()) {
            scroll_attached = true;
        }
    }else{
        scroll_bar_palette.set_color(1, bn::color(3,3,3));
        scroll_bar_palette.set_color(2, bn::color(18,18,18));
    }

    if(bn::keypad::a_released()) {
        scroll_attached = false;
    }

    if(scroll_attached) {
        scroll_bar.set_y(mouse_pos.y());
        if(scroll_bar.y() < -34) scroll_bar.set_y(-34);
        if(scroll_bar.y() > 66) scroll_bar.set_y(66);
        // This gives a total scroll of 100 units

        // Move everything
        bn::fixed scroll = bn::min( (scroll_bar.y() + 34) * 4, bn::fixed(384)); // 42

        // Ilusion of infinite scroll, scroll everything
        if(scroll <= 128)
            tab_background.set_y(78 - scroll);  // to -50
        else
            tab_background.set_y(46 - (scroll % 96));

        for(int i = 0; i < title_sprites.size(); i++) {
            title_sprites[i].set_y(-40 - scroll);
        }

        for(int i = 0; i < jobs_icons.size(); i++) {
            jobs_icons[i].set_y(-3 + (32*i) - scroll);
        }

        for(int i = 0; i < MINIGAMES_COLLECTIONS; i++) {
            for(int j = 0; j < jobs_info_name[i].size(); j++) {
                jobs_info_name[i][j].set_y(-scroll + (32*i) -12);
            }

            for(int j = 0; j < jobs_info_number[i].size(); j++) {
                jobs_info_number[i][j].set_y(-scroll + (32*i) + 4);
            }
        }
    }
}

// --------------------

// ----- Casino Tab -----

Casino_tab::Casino_tab() :
    tab_background(bn::regular_bg_items::bg_computer_casino.create_bg(8, 80)),
    play_button(bn::sprite_items::button_play.create_sprite(-56, 32)),
    play_button_palette(play_button.palette())
    {
}

void Casino_tab::update(bn::fixed_point mouse_pos) {
    if(Utils::is_inside(mouse_pos, -56, 32, 64, 32)) {
        play_button_palette.set_color(6, bn::color(10,10,10));

        if(bn::keypad::a_held()) {
            play_button.set_y(34);
        }
    }else{
        play_button_palette.set_color(6, bn::color(30,30,30));
        play_button.set_y(32);
    }

    if(bn::keypad::a_released()) play_button.set_y(32);
}

// --------------------

// ----- Itch Tab -----

Itch_tab::Itch_tab() :
    tab_background(bn::regular_bg_items::bg_computer_itch.create_bg(8, 80)),
    tab_palette(tab_background.palette())
    {
    tab_palette.set_rotate_range(2,7);
}

void Itch_tab::update(bn::fixed_point mouse_pos) {
    palette_rotation_frames++;
    if(palette_rotation_frames > 10) {
        palette_rotation_frames = 0;
        tab_palette.set_rotate_count((tab_palette.rotate_count() + 1)%7);
    }
}

// --------------------

// ----- SongStorm Tab -----

SongStorm_tab::SongStorm_tab() {

}

void SongStorm_tab::update(bn::fixed_point mouse_pos) {

}