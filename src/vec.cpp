#include "vec.h"
#include "clamp.h"

#include <random>
#include <cmath>
#include <random>
#include <functional>

static std::default_random_engine gen;
static std::uniform_real_distribution<float> unit1d(-1.0, 1.0);
static std::uniform_real_distribution<float> offset(0.0, 1.0);

Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(float x, float y) : x(x), y(y) {}
Vec2::Vec2(float v) : x(v), y(v) {}

Vec2 Vec2::random_offset_vector()
{
    return Vec2(offset(gen), offset(gen));
}

float Vec2::mag() const
{
    return sqrt(x * x + y * y);
}

Vec2& Vec2::operator+=(const Vec2& rhs)
{
    x += rhs.x;
    y += rhs.y;

    return *this;
}

Vec2& Vec2::operator-=(const Vec2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}

Vec2& Vec2::operator*=(const Vec2& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    
    return *this;
}

Vec2& Vec2::operator*=(float rhs)
{
    x *= rhs;
    y *= rhs;

    return *this;
}

Vec2& Vec2::operator/=(const Vec2& rhs)
{
    x /= rhs.x;
    y /= rhs.y;

    return *this;
}

Vec2& Vec2::operator/=(float rhs)
{
    x /= rhs;
    y /= rhs;

    return *this;
}

Vec2 operator+(Vec2 lhs, const Vec2& rhs)
{
    lhs += rhs;

    return lhs;
}

Vec2 operator-(Vec2 lhs, const Vec2& rhs)
{
    lhs -= rhs;

    return lhs;
}

Vec2 operator*(Vec2 lhs, const Vec2& rhs)
{
    lhs *= rhs;

    return lhs;
}

Vec2 operator*(Vec2 lhs, float rhs)
{
    lhs *= rhs;

    return lhs;
}

Vec2 operator*(float lhs, Vec2 rhs)
{
    return rhs * lhs;
}

Vec2 operator/(Vec2 lhs, const Vec2& rhs)
{
    lhs /= rhs;

    return lhs;
}

Vec2 operator/(Vec2 lhs, float rhs)
{
    lhs /= rhs;

    return lhs;
}

Vec2 operator/(float lhs, Vec2 rhs)
{
    return rhs * lhs;
}

Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
Vec3::Vec3(const Vec2& xy, float z) : x(xy.x), y(xy.y), z(z) {}
Vec3::Vec3(float x, const Vec2& yz) : x(x), y(yz.x), z(yz.y) {}
Vec3::Vec3(float v) : x(v), y(v), z(v) {}

Vec3 Vec3::random_unit_vector()
{
    Vec3 v;
    do {
        v.x = unit1d(gen);
        v.y = unit1d(gen);
        v.z = unit1d(gen);
    } while (v.mag() > 1.0);
    v.normalize();
    return v;
}

Vec3 Vec3::random_unit_vector_in_hemisphere(const Vec3& normal)
{
    Vec3 v = Vec3::random_unit_vector();
    if (dot(v, normal) < 0) v.invert();
    return v;
}

float Vec3::mag() const
{
    return sqrt(x*x + y*y + z*z);
}

float Vec3::avg() const
{
    return dot(*this, *this) / 3.0;
}

Vec3& Vec3::normalize()
{
    (*this) /= mag();
    return *this;
}

Vec3& Vec3::invert()
{
    x = -x;
    y = -y;
    z = -z;
    return *this;
}

Vec3& Vec3::fix()
{
    if (std::isnan(x)) x = 0.0;
    if (std::isnan(y)) y = 0.0;
    if (std::isnan(z)) z = 0.0;

    return *this;
}

Vec3& Vec3::operator+=(const Vec3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

Vec3& Vec3::operator*=(const Vec3& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

Vec3& Vec3::operator*=(float rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}

Vec3& Vec3::operator/=(const Vec3& rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
}

Vec3& Vec3::operator/=(float rhs)
{
    rhs = 1 / rhs;
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}

float dot(const Vec3& lhs, const Vec3& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vec3 cross(const Vec3& lhs, const Vec3& rhs)
{
    Vec3 ret;

    ret.x = lhs.y * rhs.z - lhs.z * rhs.y;
    ret.y = lhs.z * rhs.x - lhs.x * rhs.z;
    ret.z = lhs.x * rhs.y - lhs.y * rhs.x;

    return ret;
}

Vec3 normalized(Vec3 vec)
{
    return vec.normalize();
}

Vec3 reflect(Vec3 ray, const Vec3& normal)
{
    return ray - 2 * dot(ray, normal) * normal;
}

Vec3 operator+(Vec3 lhs, const Vec3& rhs)
{
    lhs += rhs;
    return lhs;
}

Vec3 operator-(Vec3 lhs, const Vec3& rhs)
{
    lhs -= rhs;
    return lhs;
}

Vec3 operator-(Vec3 lhs)
{
    return lhs.invert();
}

Vec3 operator*(Vec3 lhs, const Vec3& rhs)
{
    lhs *= rhs;
    return lhs;
}

Vec3 operator*(Vec3 lhs, float rhs)
{
    lhs *= rhs;
    return lhs;
}

Vec3 operator*(float lhs, Vec3 rhs)
{
    return rhs * lhs;
}

Vec3 operator/(Vec3 lhs, const Vec3& rhs)
{
    lhs /= rhs;
    return lhs;
}

Vec3 operator/(Vec3 lhs, float rhs)
{
    lhs /= rhs;
    return lhs;
}

std::ostream& operator<<(std::ostream& out, const Vec3& vec)
{
    out << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return out;
}

Color Color::from_vec3(const Vec3& vec, float gamma)
{
    auto value_to_color_int = [=](float v) { return clamp<int>(255 * pow(v, gamma), 0, 255); };
    uint8_t r = value_to_color_int(vec.x);
    uint8_t g = value_to_color_int(vec.y);
    uint8_t b = value_to_color_int(vec.z);
    return Color{r, g, b};
}
