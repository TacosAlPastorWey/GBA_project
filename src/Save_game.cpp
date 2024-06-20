#include "Save_game.h"

#include "bn_log.h"

Save_game :: Save_game() :
    _label(),
    _debt(),
    _money(),
    _highscore_names(),
    _highscore_values(),
    _uniforms(),
    _hats(),
    _gloves(),
    _themes(),
    _minigames_collections()
    {
}

void Save_game::save() {
    bn::sram::write(*this);
}

void Save_game::load() {
    bn::sram::read(*this);

    if(bn::string_view(_label) != bn::string_view("STEVE")) {
        _label[0] = 'S';
        _label[1] = 'T';
        _label[2] = 'E';
        _label[3] = 'V';
        _label[4] = 'E';
        _label[5] = '\0';

        _debt = 0;
        _money = 0;

        for(int i = 0; i < 10; i++) {
            for(int j = 0; j < 12; j++) {
                _highscore_names[i][j] = '\0';
            }
            _highscore_values[i] = 0;
        }

        _uniforms = 0;
        _hats = 0;
        _gloves = 0;
        _themes = 0;

        for(int i = 0; i < 15; i++) {
            _minigames_collections[i] = 0;
        }

        save();
    }
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

int Save_game::uniforms() {
    return _uniforms;
}

void Save_game::set_uniforms(int uniforms) {
    _uniforms = uniforms;
}

int Save_game::hats() {
    return _hats;
}

void Save_game::set_hats(int hats) {
    _hats = hats;
}

int Save_game::gloves() {
    return _gloves;
}

void Save_game::set_gloves(int gloves) {
    _gloves = gloves;
}

int Save_game::themes() {
    return _themes;
}

void Save_game::set_themes(int themes) {
    _themes = themes;
}

int Save_game::minigames_collections(int pos) {
    return _minigames_collections[pos];
}

void Save_game::set_minigames_collections(int pos, int value) {
    _minigames_collections[pos] = value;
}
