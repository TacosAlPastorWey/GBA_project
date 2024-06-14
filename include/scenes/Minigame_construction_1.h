#ifndef MINIGAME_CONSTRUCTION_1_H
#define MINIGAME_CONSTRUCTION_1_H

#include "bn_log.h"

#include "bn_keypad.h"
#include "bn_math.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_items_character.h"
#include "bn_sprite_items_axolotcoin.h"
#include "bn_sprite_items_brick.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_construction.h"

#include "bn_sound_items.h"

#include "Scene.h"

class Minigame_construction_1 : public Scene{
    private:
        bn::sprite_ptr player_spr;
        bn::fixed velocity;
        bn::sprite_animate_action<4> player_animation;
        bn::regular_bg_ptr background;

        bn::vector<bn::sprite_ptr, 5> coins;
        bn::vector<bn::sprite_ptr, 5> bricks;
    public:
        Minigame_construction_1();
        ~Minigame_construction_1() = default;

        bn::optional<SceneType> update() final;
};

#endif