#ifndef SCENE_H
#define SCENE_H

#include "bn_optional.h"

#include "Scene_type.h"

class Scene {
public:
    virtual ~Scene() = default;

    virtual bn::optional<SceneType> update() = 0;

protected:
    Scene() = default;
};

#endif