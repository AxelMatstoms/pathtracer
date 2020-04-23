#ifndef BRDF_H
#define BRDF_H

#include "primitive.h"
#include "vec.h"
#include "ray.h"

Vec3 brdf(const Material& material, const Vec3& normal, const Vec3& view, const Vec3& light);
Vec3 fresnel_schlick(const Vec3& F0, const Vec3& normal, const Vec3& view);
float ndf_ggx(float alpha, const Vec3& normal, const Vec3& micronormal);
float g_ggx_smith(float alpha, const Vec3& normal, const Vec3& view, const Vec3& light);

#endif /* BRDF_H */
