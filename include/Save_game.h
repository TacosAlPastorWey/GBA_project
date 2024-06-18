#ifndef SAVE_GAME_H
#define SAVE_GAME_H

#include "bn_sram.h"
#include "bn_string_view.h"

class Save_game
{
private:
    int _debt;
    int _money;
    char _highscore_names[10][12];
    int _highscore_values[10];
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
};

#endif