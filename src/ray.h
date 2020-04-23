#ifndef RAY_H
#define RAY_H

#include "vec.h"
#include <iostream>

struct Ray3 {
    Vec3 origin;
    Vec3 direction;

    Ray3(Vec3 origin, Vec3 direction);

    Ray3 get_ray(const Vec2& uv, float plane_dist);

    Vec3 operator*(float len) const;
};

std::ostream& operator<<(std::ostream& out, const Ray3& ray);

#endif /* RAY_H */
