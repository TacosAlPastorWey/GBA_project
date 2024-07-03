#ifndef MINIGAME_CONSTRUCTION_1_H
#define MINIGAME_CONSTRUCTION_1_H

#include "bn_log.h"

#include "bn_keypad.h"
#include "bn_math.h"
#include "bn_string.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_items_character.h"
#include "bn_sprite_items_axolotcoin.h"
#include "bn_sprite_items_brick.h"

#include "bn_sprite_palette_ptr.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_construction.h"

#include "bn_sound_items.h"

#include "Scene.h"
#include "../Definitions.h"
#include "../Global_variables.h"

class Minigame_construction_1 : public Scene{
    private:
        // Difficulty constants
        const int MAX_BLOCKS_FALLING[5] = {5, 8, 12, 15, 20};
        const int MAX_COINS_FALLING[5] = {2, 3, 4, 5, 6};
        const bn::fixed INITIAL_VELOCITY[5] = {0.01f, 0.25f, 0.50f, 0.75f, 1.0f};

    private:
        Global_variables& global;

        bn::sprite_ptr player_spr;
        bn::sprite_palette_ptr player_palette;
        bn::fixed velocity;
        int stunned_frames;
        bn::sprite_animate_action<4> player_animation;
        bn::regular_bg_ptr background;

        static const int MAX_COINS = 8;
        bn::vector<bn::sprite_ptr, MAX_COINS> coins;
        bool coins_falling[MAX_COINS];
        static const int BRICK_COUNT = 22;
        bn::vector<bn::sprite_ptr, BRICK_COUNT> bricks;
        bn::vector<bn::fixed, BRICK_COUNT> bricks_velocity;

        int objects_falling;

        int frame_counter = -1;
        const int MAX_FRAMES = 600; // 10 seconds (60fps)

        bn::vector<bn::sprite_ptr, 10> timer_label;
    public:
        Minigame_construction_1(Global_variables& _global);
        ~Minigame_construction_1() = default;

        bn::optional<SceneType> update() final;
};

#endif