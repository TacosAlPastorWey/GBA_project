#ifndef MINIGAME_PIZZA_1_H
#define MINIGAME_PIZZA_1_H

#include "bn_log.h"

#include "bn_keypad.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_items_pizza_paddle.h"
#include "bn_sprite_items_pizza_oven.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_empty_pizzeria.h"

#include "bn_sound_items.h"

#include "Scene.h"

class Minigame_pizza_1 : public Scene {
    private:
        bn::sprite_ptr player_spr;
        bn::sprite_ptr oven_spr;
        bn::regular_bg_ptr background;
        bn::fixed velocity;
        bn::sprite_animate_action<4> oven_animation;
    public:
        Minigame_pizza_1();
        ~Minigame_pizza_1() = default;

        bn::optional<SceneType> update() final;
};

#endif
