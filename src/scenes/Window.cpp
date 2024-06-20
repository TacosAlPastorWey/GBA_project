#include "scenes/Window.h"

Window::Window():
    theme_selector(bn::sprite_items::ph_border.create_sprite(-20,-26)),
    current_theme(bn::regular_bg_items::daytime_theme.create_bg(-8,0)),
    pos(2,0),
    text_generator(bn::sprite_font(bn::sprite_items::fixed_8x16_font))
    {
        menu.push_back(bn::sprite_items::theme_menu.create_sprite(-16,-24,0));
        menu.push_back(bn::sprite_items::theme_menu.create_sprite(48,-24,1));
        menu.push_back(bn::sprite_items::theme_menu.create_sprite(-16,40,2));
        menu.push_back(bn::sprite_items::theme_menu.create_sprite(48,40,3));

        theme.push_back(bn::sprite_items::themes.create_sprite(-20,-26,0));
        theme.push_back(bn::sprite_items::themes.create_sprite(20,-26,1));
        theme.push_back(bn::sprite_items::themes.create_sprite(-20,8,2));
        theme.push_back(bn::sprite_items::themes.create_sprite(20,8,3));
        theme.push_back(bn::sprite_items::themes.create_sprite(-20,42,4));
        theme.push_back(bn::sprite_items::themes.create_sprite(20,42,5));

        text_generator.set_center_alignment();
        text_generator.generate(0, -68, "Theme Selector", text);
        theme_selector.set_z_order(-1);
    }

bn::optional<SceneType> Window::update() {
    
    if (bn::keypad::b_pressed()) {
        return SceneType::HOUSE;
    }

    if (bn::keypad::up_pressed() && pos[0]) {
        pos[0] -= 1;
        theme_selector.set_position(theme_pos[pos[0]][pos[1]]);
    }

    if (bn::keypad::down_pressed() && pos[0] != 2) {
        pos[0] += 1;
        theme_selector.set_position(theme_pos[pos[0]][pos[1]]);
    }

    if (bn::keypad::left_pressed() && pos[1]) {
        pos[1] = 0;
        theme_selector.set_position(theme_pos[pos[0]][pos[1]]);
    }

    if (bn::keypad::right_pressed() && !pos[1]) {
        pos[1] = 1;
        theme_selector.set_position(theme_pos[pos[0]][pos[1]]);
    }

    /// FIXME: Add palette change based on theme chosen

    return bn::nullopt;
}