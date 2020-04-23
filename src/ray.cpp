#include "ray.h"

Ray3::Ray3(Vec3 origin_, Vec3 direction_) : origin(origin_), direction(direction_)
{
    direction.normalize();
}

Ray3 Ray3::get_ray(const Vec2& uv, float plane_dist)
{
    //Vec3 uvec(direction.z, 0, -direction.x);
    //Vec3 vvec = cross(direction, uvec);
    Vec3 uvec = normalized(cross(Vec3(0.0, 1.0, 0.0), direction));
    Vec3 vvec = cross(uvec, direction);

    return Ray3(origin, (direction * plane_dist) + (uv.x * uvec) + (uv.y * vvec));
}

Vec3 Ray3::operator*(float len) const
{
    return origin + direction * len;
}

std::ostream& operator<<(std::ostream& out, const Ray3& ray)
{
    out << "Ray3(origin: " << ray.origin << ", direction: " << ray.direction << ")";

    return out;
}
