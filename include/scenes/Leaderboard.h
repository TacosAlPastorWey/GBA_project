#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "bn_vector.h"
#include "bn_string.h"
#include "bn_sstream.h"

#include "bn_keypad.h"

#include "bn_sprite_ptr.h"

#include "bn_sprite_text_generator.h"
#include "bn_sprite_items_fixed_8x16_font.h"

#include "Scene.h"
#include "Scene_type.h"
#include "..\Save_game.h"

class Leaderboard : public Scene {
private:
    bn::sprite_text_generator text_generator;
    bn::vector<bn::sprite_ptr, 60> text_sprites;
public:
    Leaderboard(Save_game &Save);
    ~Leaderboard() = default;

    bn::optional<SceneType> update() final;
};

#endif