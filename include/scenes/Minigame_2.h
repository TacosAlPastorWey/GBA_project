#ifndef MINIGAME_2_H
#define MINIGAME_2_H

#include "bn_log.h"

#include "bn_keypad.h"
#include "bn_math.h"
#include "bn_random.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_items_axolotcoin.h"
#include "bn_sprite_items_pizza_base.h"
#include "bn_sprite_items_ingredients.h"
#include "bn_sprite_items_ph_border.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_pizzeria.h"

#include "bn_sound_items.h"

#include "Scene.h"

class Minigame_2 : public Scene {
    private:
        bn::random &rng;

        bn::sprite_ptr pizza_spr;
        bn::regular_bg_ptr background;
        bn::sprite_ptr select_spr;
        bn::vector<int,2> pos;

        bn::vector<bn::sprite_ptr, 8> ingredients_spr;

        bn::vector<int, 16> recipe_val;
        bn::vector<bn::sprite_ptr, 16> recipe_spr;

        const bn::fixed_point ingredient_pos[2][4] = {
            {bn::fixed_point(-96,-24), bn::fixed_point(-56,-24), bn::fixed_point(56,-24), bn::fixed_point(96,-24)},
            {bn::fixed_point(-96,24), bn::fixed_point(-56,24), bn::fixed_point(56,24), bn::fixed_point(96,24)}
        };
    public:
        Minigame_2(bn::random &_rng);
        ~Minigame_2() = default;

        bn::optional<SceneType> update() final;
};

#endif
