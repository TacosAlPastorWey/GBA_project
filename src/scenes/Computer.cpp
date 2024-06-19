#include "scenes/Computer.h"

Computer::Computer(Save_game& _save) : 
    background(bn::regular_bg_items::bg_computer.create_bg(-8, 0)),
    computer_palette(background.palette()),
    mouse_arrow(bn::sprite_items::mouse_arrow.create_sprite(0, 0)),
    save(_save)
    {
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

    bn::fixed mouse_top_x = mouse_arrow.x() - 8;
    bn::fixed mouse_top_y = mouse_arrow.y() - 10;

    

    return bn::nullopt;
}