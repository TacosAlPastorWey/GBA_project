#include <memory>
#include "bn_core.h"
#include "bn_memory.h"
#include "bn_random.h"

#include "Definitions.h"

#ifdef LOG_USED_MEMORY
#include "bn_log.h"
#endif

#include "scenes/Scene.h"
#include "scenes/Minigames_selector.h"  
#include "scenes/Minigame_construction_1.h"
#include "scenes/Minigame_2.h"

int main(){
    bn::core::init();

    std::unique_ptr<Scene> act_scene;
    bn::optional<SceneType> next_scene;
    act_scene.reset(new Minigames_selector());
    
    bn::random rng;

    #ifdef LOG_USED_MEMORY
    int frame_counter = 0;
    #endif

    while(true){ 
        if(act_scene){
            next_scene = act_scene->update();
            if(next_scene){
                switch (*next_scene){
                    /// FIXME: Add a case for each scene and initialize it}
                    case SceneType::MINIGAME_CONSTRUCTION_1:{
                        act_scene.reset(new Minigame_construction_1());
                        break;
                    }
                    case SceneType::MINIGAME_2:{
                        act_scene.reset(new Minigame_2(rng));
                        break;
                    }
                    case SceneType::MINIGAMES_SELECTOR:{
                        act_scene.reset(new Minigames_selector());
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
        rng.update();
        bn::core::update();
    }
}
