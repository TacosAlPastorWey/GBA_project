#ifndef SAVE_GAME_H
#define SAVE_GAME_H

#include "bn_sram.h"
#include "bn_string_view.h"

class Save_game
{
private:
    char _label[8]; // Note: the size of each atrribute must be mutiples of 4 bytes
    int _debt;
    int _money;
    char _highscore_names[10][12];
    int _highscore_values[10];
    int _uniforms; // -> 32 bits, each bit represents a uniform (0 = locked, 1 = unlocked)
    int _hats;
    int _gloves;
    int _themes;
    int _selected_stuff; // first 5 bits for uniform, next 5 for hat, next 5 for gloves, next 5 for theme
    int _minigames_collections[15]; // first bit represents if the collection is unlocked, the next bits represent the minigames unlocked in the collection
public:
    Save_game();
    ~Save_game() = default;

    void save();
    void load();

    int debt();
    void set_debt(int debt);
    int money();
    void set_money(int money);
    bn::string_view highscore_names(int pos);
    void set_highscore_names(int pos, bn::string_view name);
    int highscore_values(int pos);
    void set_highscore_values(int pos, int value);
    int uniforms();
    void set_uniforms(int uniforms);
    int hats();
    void set_hats(int hats);
    int gloves();
    void set_gloves(int gloves);
    int themes();
    void set_themes(int themes);
    int selected_stuff();
    void set_selected_stuff(int stuff);
    int minigames_collections(int pos);
    void set_minigames_collections(int pos, int value);
};

#endif