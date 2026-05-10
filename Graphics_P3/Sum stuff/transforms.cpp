#include "transforms.hpp"
#include <cmath>

// Degrees to radians helper (internal linkage)
static inline float toRad(float deg) {
    return deg * static_cast<float>(M_PI) / 180.f;
}

namespace Transform {

// ---------------------------------------------------------------------------
// rotateX -- rotation about the X-axis
//
//  [ 1    0       0    0 ]
//  [ 0    cos(a) -sin(a) 0 ]
//  [ 0    sin(a)  cos(a) 0 ]
//  [ 0    0       0    1 ]
// ---------------------------------------------------------------------------
Matrix4 rotateX(float angleDeg) {
    float a   = toRad(angleDeg);
    float c   = std::cos(a);
    float s   = std::sin(a);
    Matrix4 r = Matrix4::identity();
    // Column 1 (index 4..7)
    r.m[5]  =  c;
    r.m[6]  =  s;
    // Column 2 (index 8..11)
    r.m[9]  = -s;
    r.m[10] =  c;
    return r;
}

// ---------------------------------------------------------------------------
// rotateY -- rotation about the Y-axis
//
//  [ cos(a)  0  sin(a)  0 ]
//  [ 0       1  0       0 ]
//  [-sin(a)  0  cos(a)  0 ]
//  [ 0       0  0       1 ]
// ---------------------------------------------------------------------------
Matrix4 rotateY(float angleDeg) {
    float a   = toRad(angleDeg);
    float c   = std::cos(a);
    float s   = std::sin(a);
    Matrix4 r = Matrix4::identity();
    // Column 0
    r.m[0]  =  c;
    r.m[2]  = -s;
    // Column 2
    r.m[8]  =  s;
    r.m[10] =  c;
    return r;
}

// ---------------------------------------------------------------------------
// rotateZ -- rotation about the Z-axis
//
//  [ cos(a) -sin(a)  0  0 ]
//  [ sin(a)  cos(a)  0  0 ]
//  [ 0       0       1  0 ]
//  [ 0       0       0  1 ]
// ---------------------------------------------------------------------------
Matrix4 rotateZ(float angleDeg) {
    float a   = toRad(angleDeg);
    float c   = std::cos(a);
    float s   = std::sin(a);
    Matrix4 r = Matrix4::identity();
    // Column 0
    r.m[0]  =  c;
    r.m[1]  =  s;
    // Column 1
    r.m[4]  = -s;
    r.m[5]  =  c;
    return r;
}

// ---------------------------------------------------------------------------
// rotateAxis -- Rodrigues' rotation formula around an arbitrary axis
// The axis (ax, ay, az) is normalised internally.
// ---------------------------------------------------------------------------
Matrix4 rotateAxis(float angleDeg, float ax, float ay, float az) {
    float a = toRad(angleDeg);
    float c = std::cos(a);
    float s = std::sin(a);
    float t = 1.f - c;

    // Normalise axis
    float len = std::sqrt(ax*ax + ay*ay + az*az);
    if (len < 1e-6f) return Matrix4::identity();
    ax /= len; ay /= len; az /= len;

    Matrix4 r = Matrix4::identity();

    // Row-major representation then transposed into column-major storage:
    // R = t*n*n^T  +  c*I  +  s*[n]_x
    r.m[0]  = t*ax*ax + c;
    r.m[1]  = t*ax*ay + s*az;
    r.m[2]  = t*ax*az - s*ay;
    r.m[3]  = 0.f;

    r.m[4]  = t*ax*ay - s*az;
    r.m[5]  = t*ay*ay + c;
    r.m[6]  = t*ay*az + s*ax;
    r.m[7]  = 0.f;

    r.m[8]  = t*ax*az + s*ay;
    r.m[9]  = t*ay*az - s*ax;
    r.m[10] = t*az*az + c;
    r.m[11] = 0.f;

    r.m[12] = r.m[13] = r.m[14] = 0.f;
    r.m[15] = 1.f;
    return r;
}

// ---------------------------------------------------------------------------
// translate
//
//  [ 1  0  0  tx ]
//  [ 0  1  0  ty ]
//  [ 0  0  1  tz ]
//  [ 0  0  0  1  ]
// ---------------------------------------------------------------------------
Matrix4 translate(float tx, float ty, float tz) {
    Matrix4 r = Matrix4::identity();
    r.m[12] = tx;
    r.m[13] = ty;
    r.m[14] = tz;
    return r;
}

// ---------------------------------------------------------------------------
// scale (non-uniform)
// ---------------------------------------------------------------------------
Matrix4 scale(float sx, float sy, float sz) {
    Matrix4 r = Matrix4::identity();
    r.m[0]  = sx;
    r.m[5]  = sy;
    r.m[10] = sz;
    return r;
}

// Uniform scale convenience overload
Matrix4 scale(float s) {
    return scale(s, s, s);
}

// ---------------------------------------------------------------------------
// perspective -- standard OpenGL perspective projection
//
//  f = 1 / tan(fovy/2)
//
//  [ f/a   0        0                       0 ]
//  [ 0     f        0                       0 ]
//  [ 0     0   (far+near)/(near-far)       -1 ]
//  [ 0     0   (2*far*near)/(near-far)      0 ]
// ---------------------------------------------------------------------------
Matrix4 perspective(float fovyDeg, float aspect, float nearZ, float farZ) {
    float f   = 1.f / std::tan(toRad(fovyDeg) * 0.5f);
    float nf  = nearZ - farZ;   // near - far (negative)

    Matrix4 r;
    // Default constructor zeros everything; set non-zero entries:
    r.m[0]  = f / aspect;
    r.m[5]  = f;
    r.m[10] = (farZ + nearZ) / nf;
    r.m[11] = -1.f;
    r.m[14] = (2.f * farZ * nearZ) / nf;
    r.m[15] = 0.f;              // override the identity 1 from constructor
    return r;
}

// ---------------------------------------------------------------------------
// lookAt -- builds a view matrix from eye, center, and up vectors
//
//  f = normalise(center - eye)       (forward / -z in view space)
//  s = normalise(cross(f, up))       (right)
//  u = cross(s, f)                   (corrected up)
//
//  [ s.x   s.y   s.z  -dot(s,eye) ]
//  [ u.x   u.y   u.z  -dot(u,eye) ]
//  [-f.x  -f.y  -f.z   dot(f,eye) ]
//  [  0     0     0         1     ]
// ---------------------------------------------------------------------------
Matrix4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
    Vec3 f = (center - eye).normalise();
    Vec3 s = f.cross(up).normalise();
    Vec3 u = s.cross(f);

    Matrix4 r = Matrix4::identity();

    // Column 0: right (s)
    r.m[0]  =  s.x;
    r.m[1]  =  u.x;
    r.m[2]  = -f.x;

    // Column 1: up (u)
    r.m[4]  =  s.y;
    r.m[5]  =  u.y;
    r.m[6]  = -f.y;

    // Column 2: -forward
    r.m[8]  =  s.z;
    r.m[9]  =  u.z;
    r.m[10] = -f.z;

    // Column 3: translation component
    r.m[12] = -s.dot(eye);
    r.m[13] = -u.dot(eye);
    r.m[14] =  f.dot(eye);

    return r;
}

} // namespace Transform
