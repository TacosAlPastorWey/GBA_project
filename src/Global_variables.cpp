#include "Global_variables.h"

Global_variables::Global_variables() : 
    _save(),
    _game_difficulty(1),
    _game_money(0),
    _rng()
    {
    _save.load();
}

Save_game& Global_variables::save(){
    return _save;
}

int Global_variables::game_difficulty(){
    return _game_difficulty;
}

void Global_variables::set_game_difficulty(int difficulty){
    _game_difficulty = difficulty;
}

int Global_variables::game_money(){
    return _game_money;
}

void Global_variables::add_game_money(int amount){
    _game_money += amount;
}

bn::random& Global_variables::rng(){
    return _rng;
}