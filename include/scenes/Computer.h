#ifndef COMPUTER_H
#define COMPUTER_H

#include "bn_keypad.h"
#include "bn_vector.h"
#include "bn_unique_ptr.h"
#include "bn_string.h"

#include "bn_sound_items.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_items_mouse_arrow.h"
#include "bn_sprite_items_jobs_icons.h"
#include "bn_sprite_items_scroll_bar.h"
#include "bn_sprite_items_fixed_8x16_font.h"
#include "bn_sprite_items_button_play.h"

#include "bn_regular_bg_ptr.h"
#include "bn_bg_palette_ptr.h"
#include "bn_regular_bg_items_bg_computer.h"
#include "bn_regular_bg_items_bg_computer_2.h"
#include "bn_regular_bg_items_bg_computer_casino.h"
#include "bn_regular_bg_items_bg_computer_itch.h"

#include "Scene.h"
#include "Scene_type.h"
#include "../Save_game.h"
#include "../Definitions.h"
#include "../Utils.h"

class Tab{
public:
    virtual ~Tab() = default;
    virtual void update(bn::fixed_point mouse_pos) = 0;
};

class Jobs_tab : public Tab{
private:
    Save_game& save;

    bn::regular_bg_ptr tab_background;
    bn::bg_palette_ptr tab_palette;
    bn::sprite_text_generator& text_generator;
    bn::vector<bn::sprite_ptr, 3> title_sprites;
    bn::sprite_ptr scroll_bar;
    bn::sprite_palette_ptr scroll_bar_palette;
    bool scroll_attached = false;

    bn::vector<bn::sprite_ptr, MINIGAMES_COLLECTIONS> jobs_icons;
    static const bn::fixed_point jobs_lines_limits[MINIGAMES_COLLECTIONS];
    bn::vector<bn::sprite_ptr, 64> text_sprites;
public:
    Jobs_tab(bn::sprite_text_generator& _text_generator, Save_game& _save);
    ~Jobs_tab();

    void update(bn::fixed_point mouse_pos) final;

private:

    void update_jobs_info(bn::fixed scroll);
};

class Casino_tab : public Tab{
private:
    bn::regular_bg_ptr tab_background;
    bn::sprite_ptr play_button;
    bn::sprite_palette_ptr play_button_palette;
    
public:
    Casino_tab();
    ~Casino_tab() = default;

    void update(bn::fixed_point mouse_pos) final;
};

class Itch_tab : public Tab{
private:
    bn::regular_bg_ptr tab_background;
    bn::bg_palette_ptr tab_palette;

    int palette_rotation_frames = 0;
public:
    Itch_tab();
    ~Itch_tab() = default;

    void update(bn::fixed_point mouse_pos) final;
};

class SongStorm_tab : public Tab{

public:
    SongStorm_tab();
    ~SongStorm_tab() = default;

    void update(bn::fixed_point mouse_pos) final;
};

class Computer : public Scene {
private:
    bn::regular_bg_ptr computer_background;
    bn::bg_palette_ptr computer_palette;

    bn::sprite_ptr mouse_arrow;
    const bn::fixed_rect tabs_rect[4] = {
        bn::fixed_rect(-91, -58, 48, 16),
        bn::fixed_rect(-42, -58, 48, 16),
        bn::fixed_rect(7, -58, 48, 16),
        bn::fixed_rect(56, -58, 48, 16)
    };
    int tab_selected;
    bn::unique_ptr<Tab> tab;

    bn::sprite_text_generator text_generator;

    Save_game &save;
public:
    Computer(Save_game& _save);
    ~Computer() = default;
    
    bn::optional<SceneType> update() final;
};

#endif