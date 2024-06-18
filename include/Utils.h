#ifndef UTILS_H
#define UTILS_H

#include "bn_math.h"
#include "bn_fixed_point.h"
#include "bn_fixed_rect.h"

namespace Utils{
    inline bool is_inside(bn::fixed_point point, bn::fixed_rect rect){
        return point.x() >= rect.left() && point.x() <= rect.right() && point.y() >= rect.top() && point.y() <= rect.bottom();
    }
}

#endif