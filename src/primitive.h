#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "vec.h"
#include "ray.h"
#include <optional>

struct Material {
    /**
     * The base color for the material.
     * For dielectric materials this is the albedo diffuse lighting.
     * For metals this is the color of the specular lighting.
     * 
     * No element of the vector should exceed 1.0.
     */
    Vec3 color = Vec3();

    /**
     * The amount of glow the material emits in proportion to its base color.
     *
     * Can exceed 1.0.
     */
    float emittance = 0.0;

    /**
     * The roughness of the material.
     *
     * Value should be between 0.0 (perfectly specular) and
     * 1.0 (perfectly rough). Values close to 0.0 but not equal may
     * give bad results even with relatively high sample counts.
     */
    float roughness = 1.0;

    /**
     * The amount of specular light at normal.
     *
     * The default value of 0.5 equals ~4% and corresponds to a
     * material with an IOR of 1.45. This is standard for most
     * dielectric values.
     */
    float specular = 0.5;

    /**
     * The metalness of the material.
     *
     * Metals usually have a higher albedo, a colored specular
     * reflection and a usually greater F0;
     */
    float metallic = 0.0;

    Material();
    Material(Vec3 color, float emittance, float roughness, float specular, float metallic);

    static Material dielectric(Vec3 color, float roughness);
    static Material metal(Vec3 color, float roughness);
    static Material glow(Vec3 color, float emittance);
};

struct Hit {
    float len;
    Vec3 normal;
    Material material;
};

struct Primitive {
    virtual ~Primitive();
    virtual std::optional<Hit> intersect(const Ray3& ray) const = 0;
};

#endif /* PRIMITIVE_H */
