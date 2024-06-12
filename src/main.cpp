#include <memory>
#include "bn_core.h"
#include "bn_memory.h"

#include "Definitions.h"

#ifdef LOG_USED_MEMORY
#include "bn_log.h"
#endif

#include "scenes/Scene.h"
#include "scenes/Minigames_selector.h"  

int main(){
    bn::core::init();

    std::unique_ptr<Scene> act_scene;
    bn::optional<SceneType> next_scene;
    act_scene.reset(new Minigames_selector());
    
    #ifdef LOG_USED_MEMORY
    int frame_counter = 0;
    #endif

    while(true){ 
        if(act_scene){
            next_scene = act_scene->update();
            if(next_scene){
                switch (*next_scene){
                    /// FIXME: Add a case for each scene and initialize it}
                    /*case SceneType::MINIGAME_1:{
                        // act_scene.reset(new Minigame_1());
                        break;
                    }*/
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
        bn::core::update();
    }
}
