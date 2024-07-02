#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include "bn_random.h"

#include "Save_game.h"

class Global_variables{
    private:
        Save_game _save;
        int _game_difficulty;
        int _game_money;
        bn::random _rng;

    public:
        Global_variables();
        ~Global_variables() = default;

        Save_game& save();
        int game_difficulty();
        void set_game_difficulty(int difficulty);
        int game_money();
        void add_game_money(int amount);
        bn::random& rng();
};

#endif