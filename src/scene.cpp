#include "scene.h"

Scene::~Scene()
{
    for (auto p : primitives) {
        delete p;
    }
}

void Scene::add_primitive(Primitive* p)
{
    primitives.push_back(p);
}

std::optional<Hit> Scene::intersect(const Ray3& ray) const
{
    std::optional<Hit> closest = std::nullopt;
    for (auto p : primitives) {
        std::optional<Hit> hit = p->intersect(ray);
        if (hit && (!closest || hit->len < closest->len)) closest = hit;
    }
    return closest;
}
