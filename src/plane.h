#ifndef PLANE_H
#define PLANE_H

#include <optional>
#include <cmath>

#include "primitive.h"
#include "vec.h"
#include "ray.h"
#include "texture.h"


struct Plane : Primitive {
    Vec3 origin;
    Vec3 v1;
    Vec3 v2;

    Plane(Vec3 origin, Vec3 v1, Vec3 v2);

    std::optional<Hit> intersect(const Ray3& ray) const;
    virtual Material get_material(float u, float v) const;
};

struct MaterialPlane : Plane {
    Material material;

    MaterialPlane(Vec3 origin, Vec3 v1, Vec3 v2, Material material);

    Material get_material(float u, float v) const;
};

struct TexturePlane : Plane {
    Texture texture;

    TexturePlane(Vec3 origin, Vec3 v1, Vec3 v2, Texture texture);

    Material get_material(float u, float v) const;
};

#endif /* PLANE_H */
