#include "sphere.h"

#include <cmath>

Sphere::Sphere(Vec3 center, float radius) : center(center), radius(radius) {}

std::optional<Hit> Sphere::intersect(const Ray3& ray) const
{
    float b = dot(ray.direction, ray.origin - center);
    float c = dot(ray.origin - center, ray.origin - center) - radius * radius;

    float discriminant = b * b - c;

    if (discriminant < 0) return std::nullopt;
    
    float sqrt_disc = sqrt(discriminant);
    float d1 = -b - sqrt_disc;
    float d2 = -b + sqrt_disc;

    if (d2 < 0) return std::nullopt;

    float d = d1;

    if (d1 < 0) d = d2;

    Hit hit;
    hit.len = d;
    hit.normal = (ray * d - center).normalize();
    float u = 0.5 + atan2(-hit.normal.z, -hit.normal.x) / (2 * M_PI);
    float v = 0.5 - asin(-hit.normal.y) / M_PI;
    hit.material = get_material(u, v);
    return hit;
}

MaterialSphere::MaterialSphere(Vec3 center, float radius, Material material) : Sphere(center, radius), material(material) {}

Material MaterialSphere::get_material(float u, float v) const
{
    return material;
}
