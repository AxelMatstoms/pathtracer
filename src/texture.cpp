#include "texture.h"
#include <fstream>
#include <cstdint>
#include <cmath>
#include "clamp.h"

Texture::Texture(int width, int height) : width(width), height(height)
{
    data = new Vec3*[height]();
    for (int y = 0; y < height; y++) {
        data[y] = new Vec3[width]();
    }
}

Texture Texture::load(std::string filename, float gamma)
{
    std::ifstream file;
    file.open(filename);

    file.get();
    file.get();
    int width, height, maxval_int;
    file >> width >> height >> maxval_int;
    float maxval = static_cast<float>(maxval_int);
    file.get();

    auto read_component = [&](){ return pow(static_cast<float>(file.get()) / maxval, gamma); };

    Texture texture(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float r = read_component();
            float g = read_component();
            float b = read_component();
            texture.data[y][x] = Vec3(r, g, b);
        }
    }

    file.close();

    return texture;
}

Vec3 Texture::get_color(float u, float v) const
{
    int x = static_cast<int>(u * width);
    int y = static_cast<int>(v * height);
    x = min<int>(x, width - 1);
    y = min<int>(y, height - 1);
    return data[y][x];
}

void Texture::save(const std::string& filename, float gamma)
{
    std::ofstream file;
    file.open(filename);

    file << "P6" << std::endl;

    file << width << " " << height << std::endl;
    file << 255 << std::endl;

    uint8_t* row = new uint8_t[width * 3];

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color color = Color::from_vec3((*this)(x, y), gamma);
            row[3 * x] = color.r;
            row[3 * x + 1] = color.g;
            row[3 * x + 2] = color.b;
        }
        file.write((char*) row, width * 3);
    }

    file.close();
}

Vec3& Texture::operator()(int x, int y)
{
    return data[y][x];
}
