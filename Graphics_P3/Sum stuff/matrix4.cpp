#include "matrix4.h"
#include <iomanip>

// ---------------------------------------------------------------------------
// Matrix4 implementation
// All arithmetic is explicit -- no GLM or any other math library used.
// ---------------------------------------------------------------------------

Matrix4::Matrix4() {
    // Initialise to identity
    for (int i = 0; i < 16; ++i) m[i] = 0.f;
    m[0] = m[5] = m[10] = m[15] = 1.f;
}

Matrix4::Matrix4(const float src[16]) {
    for (int i = 0; i < 16; ++i) m[i] = src[i];
}

Matrix4 Matrix4::identity() {
    return Matrix4(); // default constructor already produces identity
}

// ---------------------------------------------------------------------------
// Matrix multiplication: result(r,c) = sum_k  this(r,k) * other(k,c)
// In column-major storage:  element(r,c) = m[c*4 + r]
// ---------------------------------------------------------------------------
Matrix4 Matrix4::operator*(const Matrix4& o) const {
    Matrix4 result;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            float sum = 0.f;
            for (int k = 0; k < 4; ++k) {
                // this(row, k)  *  other(k, col)
                sum += m[k*4 + row] * o.m[col*4 + k];
            }
            result.m[col*4 + row] = sum;
        }
    }
    return result;
}

// ---------------------------------------------------------------------------
// Matrix * Vec3  (homogeneous: w=1 in, perspective divide on output)
// ---------------------------------------------------------------------------
Vec3 Matrix4::operator*(const Vec3& v) const {
    float x = m[0]*v.x + m[4]*v.y + m[8]*v.z  + m[12];
    float y = m[1]*v.x + m[5]*v.y + m[9]*v.z  + m[13];
    float z = m[2]*v.x + m[6]*v.y + m[10]*v.z + m[14];
    float w = m[3]*v.x + m[7]*v.y + m[11]*v.z + m[15];
    if (std::abs(w) > 1e-6f) { x /= w; y /= w; z /= w; }
    return {x, y, z};
}

// ---------------------------------------------------------------------------
// Debug helper
// ---------------------------------------------------------------------------
void Matrix4::print() const {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            std::cout << std::setw(10) << std::fixed << std::setprecision(4)
                      << m[col*4 + row];
        }
        std::cout << '\n';
    }
}
