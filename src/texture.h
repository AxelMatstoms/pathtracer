#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "vec.h"

struct Texture {
    int width;
    int height;
    Vec3** data;

    Texture(int width, int height);
    static Texture load(std::string filename, float gamma = 2.2);

    Vec3 get_color(float u, float v) const;
    void save(const std::string& filename, float gamma = 0.45);

    Vec3& operator()(int x, int y);
};

#endif /* TEXTURE_H */
