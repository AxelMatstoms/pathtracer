#include <random>
#include <cmath>

#include "vec.h"
#include "ray.h"
#include "plane.h"
#include "scene.h"
#include "sphere.h"
#include "texture.h"
#include "brdf.h"
#include "mix.h"

#define WIDTH 320
#define HEIGHT 180
#define SAMPLES 512
#define REC_DEPTH 5
#define EXPOSURE 1.0

//#define DBG_PRINT


static std::default_random_engine gen;
static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

Vec3 pathtrace(const Scene& scene, const Ray3& ray, int iter)
{
    if (iter == 0) {
        return Vec3(0.0f);
    }

    auto intersect = scene.intersect(ray);

    if (!intersect) {
        return Vec3(0.0f);
    }

    const Hit& hit = *intersect;

    Ray3 new_ray(ray * hit.len + hit.normal * 0.0001, 
            Vec3::random_unit_vector_in_hemisphere(hit.normal));
    Vec3 incoming = pathtrace(scene, new_ray, iter - 1);

    Vec3 b = brdf(hit.material, hit.normal, -ray.direction, new_ray.direction);
    Vec3 sample = (hit.material.emittance * hit.material.color 
        + incoming * b) * (2 * M_PI);

    return sample;
}

Vec3 construct_tangent(const Vec3& normal)
{
    Vec3 up(0.0, 1.0, 0.0);
    Vec3 right(1.0, 0.0, 0.0);

    float up_dot_n = abs(dot(normal, up));
    float right_dot_n = abs(dot(normal, right));

    Vec3 most_orthogonal;

    if (up_dot_n < right_dot_n) {
        most_orthogonal = up;
    } else {
        most_orthogonal = right;
    }

    Vec3 tangent = normalized(cross(normal, most_orthogonal));

    return tangent;
}

Vec3 pathtrace2(const Scene& scene, const Ray3& ray, int iter)
{
    if (iter == 0) {
        return Vec3(0.1f);
    }

    auto intersect = scene.intersect(ray);

    if (!intersect) {
        return Vec3(0.0f);
    }

    const Hit& hit = *intersect;

    Vec3 normal = hit.normal;
    Vec3 view = -ray.direction;
    Vec3 tangent = construct_tangent(normal);
    Vec3 bitangent = cross(normal, tangent);

    const Material& material = hit.material;

    float alpha = material.roughness * material.roughness;

    Vec3 F0 = 0.08 * material.specular;
    F0 = mix(F0, material.color, material.metallic);
    Vec3 F = fresnel_schlick(F0, normal, view);
    
    float F_avg = F.avg();

    float q0 = dist(gen);

    Vec3 sample_direction = normal;
    float p = 1.0;
    F_avg = 0.0;

    F_avg = 0.0;
    if (q0 > F_avg) { /* sample cosine distribution */
        float phi = 2.0 * M_PI * dist(gen);
        float r = std::sqrt(dist(gen));

        float y = std::sqrt(1 - r * r);
        float x = r * cos(phi);
        float z = r * sin(phi);

        //Vec3 sample = Vec3(x, y, z); /* in tangent space */
        //p = dot(Vec3(0, 1.0, 0.0), sample) / (M_PI * (1.0 - F_avg));
        p = (1.0 - F_avg) * y / (2 * M_PI);

        sample_direction = tangent * x + normal * y + bitangent * z;

    } else { /* sample ggx brdf */
        float xi0 = dist(gen);
        float xi1 = dist(gen);

        float theta = std::acos(std::sqrt((1.0 - xi0) / (xi0 * (alpha * alpha - 1.0) + 1.0)));
        float phi = 2 * M_PI * xi1;

        float sin_theta = sin(theta);

        float x = sin_theta * cos(phi);
        float y = cos(theta);
        float z = sin_theta * sin(phi);
        
#ifdef DBG_PRINT
        std::cout << "half vector in tangent space" << std::endl;
        std::cout << Vec3(x, y, z) << std::endl;
#endif

        Vec3 half_vec = x * tangent + y * normal + z * bitangent;

#ifdef DBG_PRINT
        std::cout << "half vector in world space" << std::endl;
        std::cout << half_vec << std::endl;
#endif

        sample_direction = reflect(-view, half_vec);

#ifdef DBG_PRINT
        std::cout << "sample direction in world space" << std::endl;
        std::cout << sample_direction << std::endl;

        std::cout << "normal in world space" << std::endl;
        std::cout << normal << std::endl;
#endif
        if (dot(sample_direction, normal) < 0.0 || dot(view, half_vec) < 0.0) {
            return Vec3(0.0);
        }
        p = F_avg * (ndf_ggx(alpha, normal, half_vec) * dot(normal, half_vec)) / (4.0 * dot(view, half_vec));

#ifdef DBG_PRINT
        std::cout << "probability of ray" << std::endl;
        std::cout << p << std::endl;
#endif
    }
    
    Ray3 new_ray(ray * hit.len + hit.normal * 0.0001, sample_direction);
    Vec3 incoming = pathtrace2(scene, new_ray, iter - 1);

    return material.color * material.emittance
        + incoming * brdf(material, normal, view, sample_direction) * dot(sample_direction, normal) / p;
}

int main()
{
    Material white = Material::dielectric(Vec3(0.9, 0.9, 0.9), 0.9);
    Material ground_material = Material::dielectric(Vec3(0.9, 0.9, 0.9), 0.3);
    Material red = Material::dielectric(Vec3(1, 0.05, 0.05), 0.9);
    Material blue = Material::dielectric(Vec3(0.05, 0.3, 1), 0.3);
    Material light = Material::glow(Vec3(0.9, 0.9, 0.9), 0.6);
    Material gold = Material::metal(Vec3(1.00, 0.86, 0.57), 0.5);
    Material shiny = Material::dielectric(Vec3(1.00, 0.2, 0.2), 0.5);
    //Texture oak_planks = Texture::load("assets/oak_planks.ppm", 1.0f);

    auto ground = new MaterialPlane(Vec3(-2, 0, 0), Vec3(4, 0, 0), Vec3(0, 0, 4), ground_material);
    auto lwall = new MaterialPlane(Vec3(-2, 0, 0), Vec3(0, 2, 0), Vec3(0, 0, 4), red);
    auto rwall = new MaterialPlane(Vec3(2, 0, 0), Vec3(0, 0, 4), Vec3(0, 2, 0), blue);
    auto fwall = new MaterialPlane(Vec3(-2, 0, 4), Vec3(0, 2, 0), Vec3(4, 0, 0), white);
    auto bwall = new MaterialPlane(Vec3(-2, 0, 0), Vec3(4, 0, 0), Vec3(0, 2, 0), white);
    auto ceiling = new MaterialPlane(Vec3(-2, 2, 0), Vec3(4, 0, 0), Vec3(0, 0, 4), white);
    auto lamp = new MaterialPlane(Vec3(-0.5, 1.95, 2.5), Vec3(1, 0, 0), Vec3(0, 0, 1), light);
    auto lamp2 = new MaterialPlane(Vec3(-0.5, 0.5, 0.05), Vec3(1, 0, 0), Vec3(0, 1, 0), light);
    auto sphere = new MaterialSphere(Vec3(-1, 0.5, 2), 0.5, gold);
    auto sphere2 = new MaterialSphere(Vec3(1, 0.5, 2), 0.5, shiny);
    /*
    auto fblock = new TexturePlane(Vec3(0.75, 0, 1.75), Vec3(0.5, 0, 0), Vec3(0, 0.5, 0), oak_planks);
    auto bblock = new TexturePlane(Vec3(0.75, 0, 2.25), Vec3(0.5, 0, 0), Vec3(0, 0.5, 0), oak_planks);
    auto lblock = new TexturePlane(Vec3(0.75, 0, 1.75), Vec3(0, 0, 0.5), Vec3(0, 0.5, 0), oak_planks);
    auto rblock = new TexturePlane(Vec3(1.25, 0, 1.75), Vec3(0, 0, 0.5), Vec3(0, 0.5, 0), oak_planks);
    auto tblock = new TexturePlane(Vec3(0.75, 0.5, 1.75), Vec3(0, 0, 0.5), Vec3(0.5, 0, 0), oak_planks);
    */

    Scene scene;
    scene.add_primitive(ground);
    scene.add_primitive(lwall);
    scene.add_primitive(rwall);
    scene.add_primitive(fwall);
    scene.add_primitive(bwall);
    scene.add_primitive(ceiling);
    scene.add_primitive(lamp);
    scene.add_primitive(lamp2);
    scene.add_primitive(sphere);
    scene.add_primitive(sphere2);
    /*
    scene.add_primitive(fblock);
    scene.add_primitive(bblock);
    scene.add_primitive(tblock);
    scene.add_primitive(rblock);
    scene.add_primitive(lblock);
    */


    Vec2 dimen(WIDTH, HEIGHT);

    Ray3 cam_ray(Vec3(0, 1.0, 0.1), Vec3(0, -0.2, 1));

    for (int i = 0; i < 5; i++) {
        std::cout << "<<<<<<<<<<<<<<<<< " << i << std::endl;
        Vec3 sample = pathtrace2(scene, cam_ray, 1);
        std::cout << ">>>>>>>>>>>>>>>>>" << sample << std::endl;
    }
    //return 0;

    Texture out(WIDTH, HEIGHT);
    #pragma omp parallel for
    for (int y = 0; y < HEIGHT; y++) {
        std::cout << "Row: " << y << std::endl;
        for (int x = 0; x < WIDTH; x++) {
            Vec3 color(0);
            Vec2 xy(x, y);
            for (int s = 0; s < SAMPLES; s++) {
                Vec2 delta = Vec2::random_offset_vector();

                Vec2 uv = (xy + delta - dimen / 2) / HEIGHT;
                Ray3 ray = cam_ray.get_ray(uv, 0.75);
                Vec3 sample = pathtrace2(scene, ray, REC_DEPTH);
                color += sample.fix();
            }
            color /= SAMPLES;
            out(x, y) = color;
        }
    }
    out.save("out.ppm", 0.45);
}
