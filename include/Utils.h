#ifndef UTILS_H
#define UTILS_H

#include "bn_math.h"
#include "bn_fixed_point.h"
#include "bn_fixed_rect.h"
#include "bn_optional.h"

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

    struct Fixed_line{
        bn::fixed_point p1;
        bn::fixed_point p2;

        Fixed_line(bn::fixed_point center, bn::fixed width) : p1(center.x() - width/2, center.y()), p2(center.x() + width/2, center.y()){}
        Fixed_line(bn::fixed_point _p1, bn::fixed_point _p2) : p1(_p1), p2(_p2){}
        Fixed_line(bn::fixed x1, bn::fixed y1, bn::fixed x2, bn::fixed y2) : p1(x1, y1), p2(x2, y2){}

        bn::fixed left() const{
            return p1.x() < p2.x() ? p1.x() : p2.x();
        }

        bn::fixed right() const{
            return p1.x() > p2.x() ? p1.x() : p2.x();
        }

        inline bn::optional<Utils::Fixed_line> x_intersection(const Utils::Fixed_line& line){
            if(left() < line.right() && right() > line.left()){
                bn::fixed left = this->left() > line.left() ? this->left() : line.left();
                bn::fixed right = this->right() < line.right() ? this->right() : line.right();
                return Utils::Fixed_line(left, 0, right, 0);
            }
            return bn::nullopt;
        }
    };
}

#endif