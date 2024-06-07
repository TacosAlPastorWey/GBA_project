#include "bn_core.h"
#include "bn_keypad.h"

int main(){
    bn::core::init();

    while(true){ 
        
        bn::core::update();
    }
}
