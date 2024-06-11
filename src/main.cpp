#include <memory>
#include "bn_core.h"

#include "bn_log.h"

#include "scenes/Scene.h"
#include "scenes/Minigames_selector.h"  

int main(){
    bn::core::init();

    std::unique_ptr<Scene> act_scene;
    bn::optional<SceneType> next_scene;
    act_scene.reset(new Minigames_selector());

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

        bn::core::update();
    }
}
