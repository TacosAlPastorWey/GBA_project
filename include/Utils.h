#ifndef UTILS_H
#define UTILS_H

#include "bn_math.h"
#include "bn_fixed_point.h"
#include "bn_fixed_rect.h"

namespace Utils{
    inline bool is_inside(bn::fixed_point point, bn::fixed_rect rect){
        return point.x() >= rect.left() && point.x() <= rect.right() && point.y() >= rect.top() && point.y() <= rect.bottom();
    }

    inline bool is_inside(bn::fixed_point point, bn::fixed x, bn::fixed y, bn::fixed width, bn::fixed height){
        return is_inside(point, bn::fixed_rect(x, y, width, height));
    }

     inline int count_set_bits(int n){
        int count = 0;
        while(n){
            count += n & 1;
            n >>= 1;
        }
        return count;
    }
}

#endif