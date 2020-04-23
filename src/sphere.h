#ifndef SPHERE_H
#define SPHERE_H

#include <optional>

#include "primitive.h"
#include "vec.h"
#include "ray.h"

struct Sphere : Primitive {
    Vec3 center;
    float radius;

    Sphere(Vec3 center, float radius);

    std::optional<Hit> intersect(const Ray3& ray) const;
    virtual Material get_material(float u, float v) const = 0;
};

struct MaterialSphere : Sphere {
    Material material;

    MaterialSphere(Vec3 center, float radius, Material material);

    Material get_material(float u, float v) const;
};

#endif /* SPHERE_H */
