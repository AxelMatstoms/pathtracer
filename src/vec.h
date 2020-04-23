#ifndef VEC_H
#define VEC_H

#include <cstdint>
#include <iostream>

struct Vec2 {
    float x, y;

    Vec2();
    Vec2(float x, float y);
    Vec2(float v);

    static Vec2 random_offset_vector();

    float mag() const;

    Vec2& operator+=(const Vec2& rhs);
    Vec2& operator-=(const Vec2& rhs);
    Vec2& operator*=(const Vec2& rhs);
    Vec2& operator*=(float rhs);
    Vec2& operator/=(const Vec2& rhs);
    Vec2& operator/=(float rhs);
};

Vec2 operator+(Vec2 lhs, const Vec2& rhs);
Vec2 operator-(Vec2 lhs, const Vec2& rhs);
Vec2 operator*(Vec2 lhs, const Vec2& rhs);
Vec2 operator*(Vec2 lhs, float rhs);
Vec2 operator*(float lhs, Vec2 rhs);
Vec2 operator/(Vec2 lhs, const Vec2& rhs);
Vec2 operator/(Vec2 lhs, float rhs);
Vec2 operator/(float lhs, Vec2 rhs);

struct Vec3 {
    float x, y, z;

    Vec3();
    Vec3(float x, float y, float z);
    Vec3(const Vec2& xy, float z);
    Vec3(float x, const Vec2& yz);
    Vec3(float v);

    static Vec3 random_unit_vector();
    static Vec3 random_unit_vector_in_hemisphere(const Vec3& normal);

    float mag() const;
    float avg() const;
    Vec3& normalize();
    Vec3& invert();

    Vec3& fix();

    Vec3& operator+=(const Vec3& rhs);
    Vec3& operator-=(const Vec3& rhs);
    Vec3& operator*=(const Vec3& rhs);
    Vec3& operator*=(float rhs);
    Vec3& operator/=(const Vec3& rhs);
    Vec3& operator/=(float rhs);
    //Vec3 operator-() const;
};

struct Color {
    uint8_t r, g, b;

    static Color from_vec3(const Vec3& vec, float gamma);
};

float dot(const Vec3& lhs, const Vec3& rhs);
Vec3 cross(const Vec3& lhs, const Vec3& rhs);
Vec3 normalized(Vec3 vec);
Vec3 reflect(Vec3 ray, const Vec3& normal);


Vec3 operator+(Vec3 lhs, const Vec3& rhs);
Vec3 operator-(Vec3 lhs, const Vec3& rhs);
Vec3 operator-(Vec3 lhs);
Vec3 operator*(Vec3 lhs, const Vec3& rhs);
Vec3 operator*(Vec3 lhs, float rhs);
Vec3 operator*(float lhs, Vec3 rhs);
Vec3 operator/(Vec3 lhs, const Vec3& rhs);
Vec3 operator/(Vec3 lhs, float rhs);

std::ostream& operator<<(std::ostream& out, const Vec3& vec);

#endif /* VEC_H */
