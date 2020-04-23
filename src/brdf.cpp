#include "brdf.h"
#include "mix.h"
#include "clamp.h"
#include <cmath>

Vec3 fresnel_schlick(const Vec3& F0, const Vec3& normal, const Vec3& view)
{
    float n_dot_v = max(0.0f, dot(normal, view));

    float f = 1 - n_dot_v;

    float f5 = f * f * f * f * f;

    return F0 + (Vec3(1) - F0) * f5;
}

float ndf_ggx(float alpha, const Vec3& normal, const Vec3& micronormal)
{
    float numerator = alpha * alpha;

    float n_dot_m = max(0.0f, dot(normal, micronormal));
    float base = (n_dot_m * n_dot_m) * (alpha * alpha - 1.0) + 1.0;
    float denominator = M_PI * base * base;

    return numerator / denominator;
}

float g1_ggx(float alpha, const Vec3& normal, const Vec3& view)
{
    float n_dot_v = max(0.0f, dot(normal, view));

    float numerator = 2.0 * n_dot_v;

    float alpha2 = alpha * alpha;
    
    float denominator = n_dot_v + sqrt(alpha2 + (1 - alpha2) * n_dot_v * n_dot_v);

    return numerator / denominator;
}

float g_ggx_smith(float alpha, const Vec3& normal, const Vec3& view, const Vec3& light)
{
    return g1_ggx(alpha, normal, view) * g1_ggx(alpha, normal, light);
}

Vec3 brdf(const Material& material, const Vec3& normal, const Vec3& view, const Vec3& light)
{
    Vec3 half_vector = normalized(view + light);

    float alpha = material.roughness * material.roughness;

    float n_dot_l = max(0.0f, dot(normal, light));
    float n_dot_v = max(0.0f, dot(normal, view));

    Vec3 F0 = 0.08 * material.specular;
    F0 = mix(F0, material.color, material.metallic);
    Vec3 F = fresnel_schlick(F0, normal, half_vector);/*normalized(mix(half_vector, normal, material.roughness))*/

    float G = g_ggx_smith(alpha, normal, view, light);

    float N = ndf_ggx(alpha, normal, half_vector);

    Vec3 specular = (N * G * F) / (M_PI * n_dot_v * n_dot_l);

    Vec3 diffuse = material.color / M_PI;

    diffuse = mix(diffuse, Vec3(0.0), material.metallic);

    return mix(diffuse, specular, F) * n_dot_l;
}
