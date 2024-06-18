#ifndef HOUSE_H
#define HOUSE_H

#include "bn_keypad.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_items_character.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_house_1.h"

#include "Scene.h"

class House : public Scene {
    private:
        bn::sprite_ptr steve_spr;
        bn::regular_bg_ptr house_bg;

    public:
        House();
        ~House() = default;

        bn::optional<SceneType> update() final;
};


#endif