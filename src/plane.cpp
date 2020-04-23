#include "plane.h"
#include <iostream>

Plane::Plane(Vec3 origin, Vec3 v1, Vec3 v2) : origin(origin), v1(v1), v2(v2) {}

std::optional<Hit> Plane::intersect(const Ray3& ray) const
{
    Vec3 normal = cross(v1, v2);
    float det = dot(-ray.direction, normal);

    if (det > -0.0001 && det < 0.0001) return std::nullopt;
    Hit hit;
    hit.len = dot(normal, ray.origin - origin) / det;
    if (hit.len < 0) return std::nullopt;
    float u = dot(cross(v2, -ray.direction), ray.origin - origin) / det;
    float v = dot(cross(-ray.direction, v1), ray.origin - origin) / det;
    if (u < 0 || u > 1 || v < 0 || v > 1) return std::nullopt;
    hit.normal = normalized(dot(-ray.direction, normal) * normal);
    hit.material = get_material(u, v);
    return hit;
}

Material Plane::get_material(float u, float v) const
{
    return Material::dielectric(Vec3(1, 0, 1), 1.0);
}


MaterialPlane::MaterialPlane(Vec3 origin, Vec3 v1, Vec3 v2, Material material) : Plane(origin, v1, v2), material(material) {}

Material MaterialPlane::get_material(float u, float v) const
{
    return material;
}

TexturePlane::TexturePlane(Vec3 origin, Vec3 v1, Vec3 v2, Texture texture) : Plane(origin, v1, v2), texture(texture) {}

Material TexturePlane::get_material(float u, float v) const
{
    return Material::dielectric(texture.get_color(u, v), 1.0);
}
