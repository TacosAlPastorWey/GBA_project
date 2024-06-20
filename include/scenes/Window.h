#ifndef WINDOW_H
#define WINDOW_H

#include "bn_keypad.h"
#include "bn_vector.h"

#include "bn_sprite_text_generator.h"
#include "bn_sprite_items_fixed_8x16_font.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_items_theme_menu.h"
#include "bn_sprite_items_themes.h"
#include "bn_sprite_items_ph_border.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_daytime_theme.h"

#include "Scene.h"


class Window : public Scene
{
private:
    bn::sprite_ptr theme_selector;
    bn::regular_bg_ptr current_theme;
    bn::vector<int,2> pos;
    bn::sprite_text_generator text_generator;
    bn::vector<bn::sprite_ptr, 4> text;
    bn::vector<bn::sprite_ptr, 4> menu;
    bn::vector<bn::sprite_ptr, 6> theme;
    
    const bn::fixed_point theme_pos[3][2] = {
        {bn::fixed_point(-20,-26), bn::fixed_point(20,-26)},
        {bn::fixed_point(-20,8), bn::fixed_point(20,8)},
        {bn::fixed_point(-20,42), bn::fixed_point(20,42)}
    };
public:
    Window();
    ~Window() = default;

    bn::optional<SceneType> update() final;
};

#endif