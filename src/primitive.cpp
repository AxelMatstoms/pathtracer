#include "primitive.h"

Material::Material() {}
Material::Material(Vec3 color, float emittance, float roughness, float specular, float metallic) :
    color(color), emittance(emittance), roughness(roughness), specular(specular), metallic(metallic) {}

Primitive::~Primitive() {}

Material Material::dielectric(Vec3 color, float roughness)
{
    return Material(color, 0.0, roughness, 0.5, 0.0);
}

Material Material::metal(Vec3 color, float roughness)
{
    return Material(color, 0.0, roughness, 0.5, 1.0);
}

Material Material::glow(Vec3 color, float emittance)
{
    return Material(color, emittance, 0.0, 0.5, 0.0);
}

/*
std::optional<Hit> Primitive::intersect(const Ray3& ray) const
{
    return std::nullopt;
}
*/
