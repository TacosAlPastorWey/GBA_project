#include "Save_game.h"

Save_game :: Save_game() :
    _debt(),
    _money(),
    _highscore_names(),
    _highscore_values()
    {
}

void Save_game::save() {
    bn::sram::write(*this);
}

void Save_game::load() {
    bn::sram::read(*this);
}

int Save_game::debt() {
    return _debt;
}

void Save_game::set_debt(int debt) {
    _debt = debt;
}

int Save_game::money() {
    return _money;
}

void Save_game::set_money(int money) {
    _money = money;
}

bn::string_view Save_game::highscore_names(int pos) {
    return _highscore_names[pos];
}

void Save_game::set_highscore_names(int pos, bn::string_view name) {
    for (int i = 0; i < 12; i++)
    {   
        if (i < name.size()) {
            _highscore_names[pos][i] = name[i];
        } else {
            _highscore_names[pos][i] = '\0';
        }
    }
}

int Save_game::highscore_values(int pos) {
    return _highscore_values[pos];
}

void Save_game::set_highscore_values(int pos, int value) {
    _highscore_values[pos] = value;
}