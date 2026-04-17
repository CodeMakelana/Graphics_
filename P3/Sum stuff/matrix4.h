#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include <cmath>
#include <iostream>

// ---------------------------------------------------------------------------
// Vec3 -- lightweight 3-component vector used throughout transforms
// ---------------------------------------------------------------------------
struct Vec3 {
    float x, y, z;

    Vec3(float x = 0.f, float y = 0.f, float z = 0.f) : x(x), y(y), z(z) {}

    Vec3  operator+(const Vec3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vec3  operator-(const Vec3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vec3  operator*(float s)       const { return {x*s,   y*s,   z*s};   }

    // Dot product
    float dot(const Vec3& o) const { return x*o.x + y*o.y + z*o.z; }

    // Cross product
    Vec3 cross(const Vec3& o) const {
        return { y*o.z - z*o.y,
                 z*o.x - x*o.z,
                 x*o.y - y*o.x };
    }

    // Normalise (returns zero vector if length is ~0)
    Vec3 normalise() const {
        float len = std::sqrt(x*x + y*y + z*z);
        if (len < 1e-6f) return {0.f, 0.f, 0.f};
        return {x/len, y/len, z/len};
    }
};

// ---------------------------------------------------------------------------
// Matrix4 -- 4x4 float matrix stored in COLUMN-MAJOR order (matches OpenGL)
//
// Element at row r, column c  =>  m[c*4 + r]
//
// Column layout:
//   col 0: m[0..3]   col 1: m[4..7]
//   col 2: m[8..11]  col 3: m[12..15]
// ---------------------------------------------------------------------------
struct Matrix4 {
    float m[16];

    // Default: identity matrix
    Matrix4();

    // Construct from a raw 16-element column-major array
    explicit Matrix4(const float src[16]);

    // --------------- factory helpers ----------------------------------------
    static Matrix4 identity();

    // --------------- operators -----------------------------------------------

    // Matrix-matrix multiplication:  result = this * other
    Matrix4 operator*(const Matrix4& other) const;

    // Matrix-vector multiply (treats v as a column vec4 with w=1, discards w)
    Vec3 operator*(const Vec3& v) const;

    // Element access by (row, col) -- convenience for debugging
    float  at(int row, int col) const { return m[col*4 + row]; }
    float& at(int row, int col)       { return m[col*4 + row]; }

    // Return a pointer to the raw data for glUniformMatrix4fv
    const float* ptr() const { return m; }

    // Debug print
    void print() const;
};

#endif // MATRIX4_HPP
