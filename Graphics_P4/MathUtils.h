#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <cmath>

// ---------------------------------------------------------------------------
// Degrees -> Radians helper
// ---------------------------------------------------------------------------
inline float toRad(float deg) {
    return deg * (3.14159265358979323846f / 180.0f);
}

// ---------------------------------------------------------------------------
// Vector3 class
// ---------------------------------------------------------------------------
class Vector3 {
private:
    float x, y, z;

public:
    // Constructors
    Vector3();
    Vector3(float x, float y, float z);

    // Getters
    float getX() const;
    float getY() const;
    float getZ() const;

    // Setters
    void setX(float x);
    void setY(float y);
    void setZ(float z);

    // Returns the dot product of this vector with another
    float dot(const Vector3& other) const;

    // Returns the Euclidean length (magnitude) of this vector
    float length() const;

    // Returns a unit-length copy of this vector (does NOT modify *this)
    Vector3 normalize() const;

    // Returns the cross product: this x other
    Vector3 cross(const Vector3& other) const;

    // Component-wise addition: this + other
    Vector3 add(const Vector3& other) const;

    // Component-wise subtraction: this - other
    Vector3 subtract(const Vector3& other) const;

    // Uniform scalar multiplication: this * s
    Vector3 scale(float s) const;
};

// ---------------------------------------------------------------------------
// Matrix4 class  (row-major storage)
//
//   m[row][col]
//
//   Upload to OpenGL with:
//     glUniformMatrix4fv(loc, 1, GL_TRUE, &mat.getData()[0]);
//
//   GL_TRUE tells the driver to transpose from our row-major layout into
//   OpenGL's expected column-major order.
// ---------------------------------------------------------------------------
class Matrix4 {
private:
    float m[4][4];

public:
    // Default constructor: zero-initialise ALL 16 entries.
    Matrix4();

    // Returns a const pointer to the first element of the internal
    // float[4][4] array, suitable for passing to glUniformMatrix4fv.
    const float* getData() const;

    // Element access (read): mat.get(row, col)
    float get(int row, int col) const;

    // Element access (write): mat.set(row, col, value)
    void set(int row, int col, float value);

    // ----- Matrix arithmetic (non-static) -----

    // Returns the product: this * other  (standard 4x4 multiplication)
    Matrix4 multiply(const Matrix4& other) const;

    // ----- Factory methods (static) -----
    // Each returns a freshly constructed matrix.

    // 4x4 identity matrix
    static Matrix4 identity();

    // Translation by (tx, ty, tz)
    static Matrix4 translate(float tx, float ty, float tz);

    // Non-uniform scale by (sx, sy, sz)
    static Matrix4 scaleMatrix(float sx, float sy, float sz);

    // Rotation around the X axis by `deg` degrees
    static Matrix4 rotateX(float deg);

    // Rotation around the Y axis by `deg` degrees
    static Matrix4 rotateY(float deg);

    // Rotation around the Z axis by `deg` degrees
    static Matrix4 rotateZ(float deg);

    // -----------------------------------------------------------------------
    // Rotation around an arbitrary axis THROUGH THE ORIGIN by `deg` degrees.
    //
    // Uses the full decomposition:
    //   R = Rx(-theta_x) * Ry(-theta_y) * Rz(deg) * Ry(theta_y) * Rx(theta_x)
    //
    // where theta_x and theta_y are the angles needed to align axisDir with
    // the Z axis before performing the actual Rz rotation.
    //
    //   theta_x = atan2(uy, uz)        -- rotate axis into XZ plane (around X)
    //   theta_y = atan2(ux, sqrt(uy^2 + uz^2))  -- rotate axis onto Z (around Y)
    //             NOTE: negative sign here because Ry rotates +X toward +Z,
    //             so we need -theta_y to bring a positive-ux component onto +Z.
    // -----------------------------------------------------------------------
    static Matrix4 rotateArbitraryAxis(const Vector3& axisDir, float deg);

    // -----------------------------------------------------------------------
    // Rotation around an arbitrary axis defined by a point P0 and a direction.
    //
    // Full decomposition:
    //   R = T(P0) * Rx(-theta_x) * Ry(-theta_y) * Rz(deg)
    //            * Ry(theta_y) * Rx(theta_x) * T(-P0)
    //
    // Steps:
    //   1. T(-P0):       Translate so the axis passes through the origin.
    //   2. Rx(theta_x):  Rotate axis into the XZ plane.
    //   3. Ry(theta_y):  Rotate axis onto the Z axis.
    //   4. Rz(deg):      Perform the desired rotation.
    //   5. Ry(-theta_y): Undo step 3.
    //   6. Rx(-theta_x): Undo step 2.
    //   7. T(P0):        Undo step 1.
    // -----------------------------------------------------------------------
    static Matrix4 rotateArbitraryAxis(const Vector3& axisPoint,
                                       const Vector3& axisDir,
                                       float deg);

    // Symmetric perspective projection
    static Matrix4 perspective(float fovDeg, float aspect,
                               float near, float far);

    // View matrix from eye position, look-at target, and up direction
    static Matrix4 lookAt(const Vector3& eye, const Vector3& center,
                          const Vector3& up);
};

#endif // MATHUTILS_H
