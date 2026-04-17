#ifndef VEC3_H
#define VEC3_H

/**
 * Vec3.h
 * A simple 3-component float vector used for 2D positions (x, y, z=0)
 * and colours (r, g, b). No external math libraries used.
 */
struct Vec3
{
    float x, y, z;

    // ── Constructors ──────────────────────────────────────────────────────────
    Vec3();                            // zero vector (0, 0, 0)
    Vec3(float x, float y, float z);   // explicit components

    // ── Arithmetic operators ──────────────────────────────────────────────────
    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator*(float scalar)      const; // scale by scalar
    Vec3& operator+=(const Vec3& other);
};

#endif // VEC3_H