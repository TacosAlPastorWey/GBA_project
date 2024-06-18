#ifndef MINIGAMES_SELECTOR_H
#define MINIGAMES_SELECTOR_H

#include "bn_vector.h"
#include "bn_keypad.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_items_arrow.h"
#include "bn_sprite_items_fixed_8x16_font.h"
#include "bn_sprite_text_generator.h"

#include "Scene.h"

class Minigames_selector : public Scene {
private:
    int top_scene = 0;
    int pos_arrow = 0;
    bool update_scene_list = true;
    bn::vector<bn::sprite_ptr, 32> scene_names_sprites;
    bn::sprite_ptr arrow_sprite;
    bn::sprite_text_generator text_generator;
    
    static constexpr int SCENE_COUNT = 3;
    static const bn::string_view scene_names[SCENE_COUNT];

public:
    Minigames_selector();
    ~Minigames_selector() = default;

    bn::optional<SceneType> update() final;
};

#endif