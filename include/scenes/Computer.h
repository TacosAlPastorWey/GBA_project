#ifndef COMPUTER_H
#define COMPUTER_H

#include "bn_keypad.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_items_mouse_arrow.h"

#include "bn_regular_bg_ptr.h"
#include "bn_bg_palette_ptr.h"
#include "bn_regular_bg_items_bg_computer.h"

#include "Scene.h"
#include "Scene_type.h"
#include "../Save_game.h"

class Computer : public Scene {
private:
    bn::regular_bg_ptr background;
    bn::bg_palette_ptr computer_palette;
    bn::sprite_ptr mouse_arrow;

    Save_game &save;
public:
    Computer(Save_game& _save);
    ~Computer() = default;
    
    bn::optional<SceneType> update() final;
};

#endif