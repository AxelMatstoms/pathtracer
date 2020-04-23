#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <optional>
#include "primitive.h"
#include "ray.h"

struct Scene {
    std::vector<Primitive*> primitives;

    ~Scene();

    void add_primitive(Primitive* p);

    std::optional<Hit> intersect(const Ray3& ray) const;
};

#endif /* SCENE_H */
