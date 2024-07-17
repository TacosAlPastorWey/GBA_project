#include "bn_unique_ptr.h"
#include "bn_core.h"
#include "bn_memory.h"
#include "bn_backdrop.h"
#include "bn_dmg_music_items_wheretheheartis.h"
#include "bn_dmg_music.h"

// Game Systems
#include "Definitions.h"
#include "Global_variables.h"

// Log
#ifdef LOG_USED_MEMORY
#include "bn_log.h"
#endif

// Minigame Scenes
#include "scenes/Scene.h"
#include "scenes/House.h"
#include "scenes/Leaderboard.h"
#include "scenes/Computer.h"
#include "scenes/Window.h"
#include "scenes/Wardrobe.h"
#include "scenes/Minigames_selector.h"  
#include "scenes/Minigame_construction_1.h"
#include "scenes/Minigame_construction_2.h"
#include "scenes/Minigame_construction_3.h"
#include "scenes/Minigame_construction_4.h"
#include "scenes/Minigame_pizza_1.h"
#include "scenes/Minigame_2.h"

#ifdef SHOW_CPU_USAGE
#include "bn_vector.h"
#include "bn_sprite_text_generator.h"
#endif

int main(){
    bn::core::init();
    bn::backdrop::set_color(bn::color(0, 0, 31));

    Global_variables global;

    bn::unique_ptr<Scene> act_scene(bn::make_unique<House>());
    bn::optional<SceneType> next_scene;
    bn::dmg_music_items::wheretheheartis.play();

    #ifdef LOG_USED_MEMORY
    int frame_counter = 0;
    #endif

    #ifdef SHOW_CPU_USAGE
    bn::vector<bn::sprite_ptr, 5> cpu_usage_spr;
    #endif

    while(true){ 
        if(act_scene){
            next_scene = act_scene->update();
            if(next_scene){
                switch (*next_scene){
                    /// FIXME: Add a case for each scene and initialize it}
                    case SceneType::HOUSE:{
                        act_scene.reset(new House());
                        if(!bn::dmg_music::playing_item()){
                            bn::dmg_music_items::wheretheheartis.play();
                            }
                        break;
                    }
                    case SceneType::LEADERBOARD:{
                        act_scene.reset(new Leaderboard(global));
                        break;
                    }
                    case SceneType::COMPUTER:{
                        act_scene.reset(new Computer(global));
                        break;
                    }
                    case SceneType::WINDOW:{
                        act_scene.reset(new Window());
                        break;
                    }
                    case SceneType::WARDROBE:{
                        act_scene.reset(new Wardrobe(global));
                        break;
                    }
                    case SceneType::MINIGAME_CONSTRUCTION_1:{
                        act_scene.reset(new Minigame_construction_1(global));
                        break;
                    }
                    case SceneType::MINIGAME_CONSTRUCTION_2:{
                        act_scene.reset(new Minigame_construction_2(global));
                        break;
                    }
                    case SceneType::MINIGAME_CONSTRUCTION_3:{
                        act_scene.reset(new Minigame_construction_3(global));
                        break;
                    }
                    case SceneType::MINIGAME_CONSTRUCTION_4:{
                        act_scene.reset(new Minigame_construction_4(global));
                        break;
                    }
                    case SceneType::MINIGAME_2:{
                        act_scene.reset(new Minigame_2(global));
                        break;
                    }
                    case SceneType::MINIGAME_PIZZA_1:{
                        act_scene.reset(new Minigame_pizza_1());
                        break;
                    }
                    case SceneType::MINIGAMES_SELECTOR:{
                        bn::dmg_music::stop();
                        act_scene.reset(new Minigames_selector(global));
                        break;
                    }
                    
                    default:{
                        BN_LOG("Scene not found");
                        break;
                    }
                }
            }
        }

        #ifdef LOG_USED_MEMORY
        frame_counter++;
        if(frame_counter == 60){
            frame_counter = 0;
            BN_LOG("Used EWRAM: ", bn::memory::used_alloc_ewram() + bn::memory::used_static_ewram(), "/262144");
            BN_LOG("Used IWRAM: ", bn::memory::used_stack_iwram() + bn::memory::used_static_iwram(), "/32768");
        }
        #endif

        #ifdef SHOW_CPU_USAGE
        cpu_usage_spr.clear();
        global.text_gen_var_8x8().generate(-80, 70, "CPU:"+ bn::to_string<8,int>(int(bn::core::last_cpu_usage()*100)) +"%", cpu_usage_spr);
        #endif

        global.rng().update();
        bn::core::update();
    }
}
