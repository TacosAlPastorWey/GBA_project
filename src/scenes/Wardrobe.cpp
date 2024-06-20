#include "scenes/Wardrobe.h"

Wardrobe::Wardrobe(Save_game& _save) : 
    save(_save), 
    background(bn::regular_bg_items::bg_wardrobe.create_bg(-8, 0)),
    bg_palette(background.palette()),
    left_arrow(bn::sprite_items::arrow_wardrobe.create_sprite(10, 24)),
    left_arrow_palette(left_arrow.palette()),
    right_arrow(bn::sprite_items::arrow_wardrobe.create_sprite(106, 24)),
    right_arrow_palette(bn::sprite_palette_ptr::create_new(bn::sprite_items::arrow_wardrobe.palette_item())),
    text_generator(bn::sprite_font(bn::sprite_items::fixed_8x16_font)),
    index_text("01/32"),
    curr_item(bn::sprite_items::object.create_sprite(60, 24)),
    prev_option_selected(-1),
    option_selected(0),
    item_index(0)
    {
    left_arrow.set_visible(false);
    right_arrow.set_horizontal_flip(true);
    load_curr_item();

    right_arrow.set_palette(right_arrow_palette);

    text_generator.generate(42, 68, index_text, text_sprites);
}

bn::optional<SceneType> Wardrobe::update() {

    //Change color of the arrows
    if(bn::keypad::left_held()) {
        left_arrow_palette.set_color(2, bn::color(28,13,1));
    }else if(bn::keypad::right_held()) {
        right_arrow_palette.set_color(2, bn::color(28,13,1));
    }

    if(bn::keypad::left_released()) {
        item_index--;
        if(item_index <= 0){ 
            item_index = 0;
            left_arrow.set_visible(false);
        }
        left_arrow_palette.set_color(2, bn::color(31,31,31));
        load_curr_item();
        right_arrow.set_visible(true);
    }else if(bn::keypad::right_released()) {
        item_index++;
        if(item_index >= 31){
            item_index = 31;
            right_arrow.set_visible(false);
        }
        right_arrow_palette.set_color(2, bn::color(31,31,31));
        load_curr_item();
        left_arrow.set_visible(true);
    }

    if(bn::keypad::a_pressed()){
        int flags = 0;
        switch (option_selected){
        case 0:
            flags = save.uniforms();
            break;
        case 1:
            flags = save.hats();
            break;
        case 2:
            flags = save.gloves();
            break;
        default:
            break;
        }

        // Si el objeto no esta bloqueado lo selecciona
        if(flags & (1 << item_index)) {
            save.set_selected_stuff(save.selected_stuff() & ~(0x1F << (option_selected * 5)) | (item_index << (option_selected*5)));
            load_curr_item();
        }
    }

    if(bn::keypad::l_pressed()) {
        option_selected--;
        if(option_selected < 0) option_selected = 0;
    }

    if(bn::keypad::r_pressed()) {
        option_selected++;
        if(option_selected > 2) option_selected = 2;
    }

    if(prev_option_selected != option_selected) {
        bg_palette.set_colors(bn::regular_bg_items::bg_wardrobe.palette_item());

        switch (option_selected){
            case 0:
                bg_palette.set_color(6, bn::color(24, 24, 31));
                bg_palette.set_color(9, bn::color(31, 26, 21));
                break;
            case 1:
                bg_palette.set_color(7, bn::color(24, 24, 31));
                bg_palette.set_color(10, bn::color(31, 26, 21));
                break;
            case 2:
                bg_palette.set_color(8, bn::color(24, 24, 31));
                bg_palette.set_color(11, bn::color(31, 26, 21));
                break;
            default:
                break;
        }
        prev_option_selected = option_selected;
        item_index = 0;
        left_arrow.set_visible(false);
        right_arrow.set_visible(true);
        load_curr_item();
    }

    if(bn::keypad::b_pressed()) {
        save.save();
        return SceneType::HOUSE;
    }

    return bn::nullopt;
}

void Wardrobe::set_index_text() {
    index_text[0] = ((item_index+1) / 10) + '0';
    index_text[1] = ((item_index+1) % 10) + '0';
    text_sprites.clear();
    text_generator.generate(42, 68, index_text, text_sprites);
}

void Wardrobe::load_curr_item() {
    /// Check if the item is locked
    int flags = 0,selected = 0;
    switch (option_selected){
    case 0:
        flags = save.uniforms();
        selected = save.selected_stuff() & 0x1F;
        break;
    case 1:
        flags = save.hats();
        selected = (save.selected_stuff() >> 5) & 0x1F;
        break;
    case 2:
        flags = save.gloves();
        selected = (save.selected_stuff() >> 10) & 0x1F;
        break;
    default:
        break;
    }

    set_index_text();

    if(selected == item_index) {
        text_generator.generate(28, -28, "SELECTED", text_sprites);
    }

    if((flags & (1 << item_index)) == 0) {
        curr_item.set_tiles(bn::sprite_items::locked.tiles_item());
    }else{
        /// FIXME: Set the corresponding item
        curr_item.set_tiles(bn::sprite_items::object.tiles_item());
    }
}