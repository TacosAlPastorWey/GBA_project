#ifndef MINIGAME_CONSTRUCTION_4_H
#define MINIGAME_CONSTRUCTION_4_H

#include "bn_keypad.h"
#include "bn_memory.h"
#include "bn_math.h"
#include "bn_vector.h"
#include "bn_string.h"
#include "bn_camera_ptr.h"
#include "bn_unique_ptr.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_items_steve_lateral.h"
#include "bn_sprite_items_stone.h"
#include "bn_sprite_items_hit_bar.h"
#include "bn_sprite_items_arrow_8x8.h"
#include "bn_sprite_items_pickaxe.h"
#include "bn_sprite_items_steve_arm.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_daytime_theme.h"

#include "bn_sound_items.h"

#include "Scene.h"
#include "Definitions.h"
#include "../Global_variables.h"

struct Stone{
    bn::sprite_ptr sprite;
    int max_life;
    int life; // 0 = dead, >0 = life

    Stone(int _life);

    void break_stone(int damage);
    bool is_breaked();
}; 

struct Hit_bar{
    static constexpr bn::fixed_point POS = bn::fixed_point(-40, 0);

    Global_variables& global;
    bn::sprite_tiles_ptr floor_tiles_ptrs[2];
    bn::span<bn::tile> vram_tiles_ptrs[2];
    bn::sprite_ptr sprites[2];
    int crit_center;
    bn::sprite_ptr arrow;
    bool arrow_move_left;

    bool active;

    Hit_bar(Global_variables& _global);

    void activate();
    void update();
    int hit();
    bool is_active();

    void deactivate();
};

class Minigame_construction_4 : public Scene{
private:
    Global_variables& global;

    bn::camera_ptr camera;
    int camera_shake_frames;
    int camera_shake_counter;
    bool shaking_camera;

    bn::regular_bg_ptr theme_bg;

    bn::sprite_ptr spr_steve;
    static constexpr bn::fixed_point steve_arm_joint = bn::fixed_point(48, 8);
    bn::sprite_ptr spr_pickaxe;
    bn::sprite_ptr spr_steve_arm;
    int rotation_angle;
    int rotation_speed;
    bool arm_moving;

    int stones_to_break;
    int stones_breaked;
    bn::unique_ptr<Stone> stone;  
    bool replacing_stone;
    Hit_bar hit_bar;

    int frame_counter;
    static constexpr int MAX_FRAMES = 600; // 10 seconds (60fps)
    int state = 0; // 0 = playing, 1 = win, 2 = lose

    bn::vector<bn::sprite_ptr,6> hit_label;
    bn::vector<bn::sprite_ptr,8> stones_breaked_label;
    bn::vector<bn::sprite_ptr,10> timer_label;
public:
    Minigame_construction_4(Global_variables& _global);
    ~Minigame_construction_4() = default;

    bn::optional<SceneType> update() final;

private:
    void rotate_steve_arm(int angle);

    void shake_camera(int frames);
    void update_camera();

    void update_stones_breaked_label();
};

#endif