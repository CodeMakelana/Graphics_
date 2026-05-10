#ifndef TRANSFORMS_HPP
#define TRANSFORMS_HPP

#include "matrix4.h"

// ---------------------------------------------------------------------------
// All transform functions return a Matrix4 built from first principles.
// No GLM arithmetic functions are used anywhere in this file or its .cpp.
// Angles are in DEGREES; internally converted to radians.
// ---------------------------------------------------------------------------

namespace Transform {

    // -----------------------------------------------------------------------
    // Rotation matrices (right-handed, angle in degrees)
    // -----------------------------------------------------------------------
    Matrix4 rotateX(float angleDeg);
    Matrix4 rotateY(float angleDeg);
    Matrix4 rotateZ(float angleDeg);

    // Rotate around an arbitrary normalised axis (Rodrigues / axis-angle)
    Matrix4 rotateAxis(float angleDeg, float ax, float ay, float az);

    // -----------------------------------------------------------------------
    // Translation and scale
    // -----------------------------------------------------------------------
    Matrix4 translate(float tx, float ty, float tz);
    Matrix4 scale(float sx, float sy, float sz);
    Matrix4 scale(float s); // uniform scale

    // -----------------------------------------------------------------------
    // Projection
    //   fovyDeg  -- vertical field of view in degrees
    //   aspect   -- width / height
    //   near/far -- clip planes (both positive, near < far)
    // -----------------------------------------------------------------------
    Matrix4 perspective(float fovyDeg, float aspect, float nearZ, float farZ);

    // -----------------------------------------------------------------------
    // View matrix
    //   eye    -- camera position
    //   center -- point the camera looks at
    //   up     -- world up vector (usually {0,1,0})
    // -----------------------------------------------------------------------
    Matrix4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);

} // namespace Transform

#endif // TRANSFORMS_HPP
