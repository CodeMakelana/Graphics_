#include "MathUtils.h"

// ===========================================================================
//  Vector3 
// ===========================================================================

Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

// ----- Getters -----
float Vector3::getX() const { return x; }
float Vector3::getY() const { return y; }
float Vector3::getZ() const { return z; }

// ----- Setters -----
void Vector3::setX(float x) { this->x = x; }
void Vector3::setY(float y) { this->y = y; }
void Vector3::setZ(float z) { this->z = z; }

// ----- Member operations -----

float Vector3::dot(const Vector3& other) const {
    // TODO: return the dot product  x*other.x + y*other.y + z*other.z
    Vector3 result;
    result.setX(x * other.getX());
    result.setY(y * other.getY());
    result.setZ(z * other.getZ());
    return result.getX() + result.getY() + result.getZ();
}

float Vector3::length() const {
    // TODO: return sqrt(x*x + y*y + z*z)
    Vector3 result;
    result.setX(x * x);
    result.setY(y * y);
    result.setZ(z * z);
    return sqrt(result.getX() + result.getY() + result.getZ());
}

Vector3 Vector3::normalize() const {
    // TODO: compute len = this->length().
    Vector3 res;
    float len = this->length();
    if (len == 0) {
        return Vector3(0, 0, 0);
    } else {
        res.setX(x / len);
        res.setY(y / len);
        res.setZ(z / len);
    }
    return res;
}

Vector3 Vector3::cross(const Vector3& other) const {
    // TODO: return the cross product this x other
    Vector3 res;
      res.x = y * other.z - z * other.y;
      res.y = z * other.x - x * other.z;
      res.z = x * other.y - y * other.x;
    return res;
}

Vector3 Vector3::add(const Vector3& other) const {
    // TODO: return Vector3(x + other.x, y + other.y, z + other.z)
    Vector3 res;
    res.x = x + other.x;
    res.y = y + other.y;
    res.z = z + other.z;
    return res;
}

Vector3 Vector3::subtract(const Vector3& other) const {
    // TODO: return Vector3(x - other.x, y - other.y, z - other.z)
    Vector3 res;
    res.x = x - other.x;
    res.y = y - other.y;
    res.z = z - other.z;
    return res;
}

Vector3 Vector3::scale(float s) const {
    // TODO: return Vector3(x * s, y * s, z * s)
    Vector3 res;
    res.x = x * s;
    res.y = y * s;
    res.z = z * s;
    return res;
}

// ===========================================================================
//  Matrix4 implementation
// ===========================================================================

Matrix4::Matrix4() {
    // Zero-initialise all 16 entries
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] = 0.0f;
}

const float* Matrix4::getData() const {
    // Returns pointer to m[0][0]; the 16 floats are contiguous in row-major
    // order.  Pass to OpenGL with GL_TRUE to have the driver transpose.
    return &m[0][0];
}

float Matrix4::get(int row, int col) const {
    return m[row][col];
}

void Matrix4::set(int row, int col, float value) {
    m[row][col] = value;
}

// ---------------------------------------------------------------------------
//  multiply:  returns this * other
// ---------------------------------------------------------------------------
Matrix4 Matrix4::multiply(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                result.m[i][j] += m[i][k] * other.m[k][j];
    return result;
}

Matrix4 Matrix4::identity() {
    // TODO: Create a Matrix4 and set m[0][0]=m[1][1]=m[2][2]=m[3][3]=1.
    Matrix4 mat;
    mat.set(0,0,1);
    mat.set(1,1,1);
    mat.set(2,2,1);
    mat.set(3,3,1);

    return mat;
}

// ---------------------------------------------------------------------------
//  translate(tx, ty, tz)
// ---------------------------------------------------------------------------
Matrix4 Matrix4::translate(float tx, float ty, float tz) {
    // TODO: Start from identity, then set:

    Matrix4 mat = identity();
      mat.set(0,3,tx);
      mat.set(1,3,ty);
      mat.set(2,3,tz);
    
    //   [ 1  0  0  tx ]
    //   [ 0  1  0  ty ]
    //   [ 0  0  1  tz ]
    //   [ 0  0  0   1 ]
    return mat;
}

// ---------------------------------------------------------------------------
//  scaleMatrix(sx, sy, sz)
// ---------------------------------------------------------------------------
Matrix4 Matrix4::scaleMatrix(float sx, float sy, float sz) {
    // TODO: Start from zero matrix, then set:
    Matrix4 mat = identity();
    mat.set(0,0,sx);
    mat.set(1,1,sy);
    mat.set(2,2,sz);
    //   [ sx  0   0  0 ]
    //   [  0  sy  0  0 ]
    //   [  0  0  sz  0 ]
    //   [  0  0   0  1 ]
    return mat;
}

// ---------------------------------------------------------------------------
//  rotateX(deg)
// ---------------------------------------------------------------------------
Matrix4 Matrix4::rotateX(float deg) {
    // TODO: Convert deg to radians using toRad(deg).
      float c = cos(toRad(deg)), s = sin(toRad(deg));

    Matrix4 mat = identity();

    mat.set(1,1,c);
    mat.set(1,2,-(s));
    mat.set(2,1,s);
    mat.set(2,2,c);

    //   [ 1   0    0   0 ]
    //   [ 0   c   -s   0 ]
    //   [ 0   s    c   0 ]
    //   [ 0   0    0   1 ]
    return mat;
}

// ---------------------------------------------------------------------------
//  rotateY(deg)
// ---------------------------------------------------------------------------
Matrix4 Matrix4::rotateY(float deg) {
    // TODO: Convert deg to radians.
    float c = cos(toRad(deg)), s = sin(toRad(deg));

    Matrix4 mat = identity();
    mat.set(0,0,c);
    mat.set(0,2,s);
    mat.set(2,0,-(s));
    mat.set(2,2,c);
    //       0   1  2  3
    //   0[  c  0   s  0 ]
    //   1[  0  1   0  0 ]
    //   2[ -s  0   c  0 ]
    //   3[  0  0   0  1 ]
    return mat;
}

// ---------------------------------------------------------------------------
//  rotateZ(deg)
// ---------------------------------------------------------------------------
Matrix4 Matrix4::rotateZ(float deg) {
    // TODO: Convert deg to radians.
    float c = cos(toRad(deg)), s = sin(toRad(deg));

    Matrix4 mat = identity();
    mat.set(0,0,c);
    mat.set(0,1,-(s));
    mat.set(1,0,s);
    mat.set(1,1,c);

    //      0   1   2  3
    //   0[ c  -s   0  0 ]
    //   1[ s   c   0  0 ]
    //   2[ 0   0   1  0 ]
    //   3[ 0   0   0  1 ]
    return mat;
}

// ---------------------------------------------------------------------------
//  rotateArbitraryAxis (through the origin)
// ---------------------------------------------------------------------------
Matrix4 Matrix4::rotateArbitraryAxis(const Vector3& axisDir, float deg) {
    // TODO: Rotation around an arbitrary axis that passes through the origin.
    //
    //   1. Normalize axisDir -> u = (ux, uy, uz).
    Vector3 u = axisDir.normalize();
    //
    //   2. Compute alignment angles:
    //        theta_x = atan2(uy, uz)
    //          -- This is the angle to rotate the axis into the XZ plane
    //             by rotating around the X axis.
    float theta_x = atan2(u.getY(), u.getZ());
    //
    //        d = sqrt(uy*uy + uz*uz)
    float d = sqrt(((u.getY())*(u.getY())) + ((u.getZ())*(u.getZ())));
    //        theta_y = atan2(ux, d)
    //          -- After Rx(theta_x), the axis lies in the XZ plane.
    //             This angle rotates it onto the +Z axis via Ry.
    //          -- NOTE the sign: we need Ry(-theta_y) to bring the axis
    //             FROM its XZ-plane direction ONTO +Z, so theta_y as
    //             computed here will be negated when building the Ry step.
    float theta_y = atan2(u.getX(), d);
    //
    //   3. Build the decomposition:
    //        R = Rx(-theta_x) * Ry(-theta_y) * Rz(deg) * Ry(theta_y) * Rx(theta_x)
    float theta_x_deg = theta_x * (180.0f / 3.14159265358979323846f);
    float theta_y_deg = theta_y * (180.0f / 3.14159265358979323846f);
    
    // T(P0) * Rx(-θx) * Ry(+θy) * Rz(θ) * Ry(-θy) * Rx(+θx)
    Matrix4 R = rotateX(-theta_x_deg).multiply(rotateY(theta_y_deg).multiply(rotateZ(deg).multiply(rotateY(-theta_y_deg).multiply(rotateX(theta_x_deg)))));
    //
    //      Reading right-to-left (the order transformations are applied):
    //        a) Rx(theta_x):     rotate axis into XZ plane
    //        b) Ry(theta_y):     rotate axis onto +Z    (see sign note above)
    //        c) Rz(deg):         perform the actual rotation
    //        d) Ry(-theta_y):    undo step b
    //        e) Rx(-theta_x):    undo step a
    //
    //   4. Multiply the five matrices together using this->multiply()
    //      (or call the static/member multiply). Return the result.
    return R;
}

// ---------------------------------------------------------------------------
//  rotateArbitraryAxis (around axis through point P0)
// ---------------------------------------------------------------------------
Matrix4 Matrix4::rotateArbitraryAxis(const Vector3& axisPoint,
                                     const Vector3& axisDir,
                                     float deg) {
    // TODO: Rotation around an arbitrary axis defined by point P0 and direction.
    //
    //   Full decomposition:
    //     R = T(P0) * Rx(-theta_x) * Ry(-theta_y) * Rz(deg)
    //              * Ry(theta_y) * Rx(theta_x) * T(-P0)
    Vector3 u = axisDir.normalize();
    float d = sqrt(u.getY() * u.getY() + u.getZ() * u.getZ());
    float theta_x_deg = atan2(u.getY(), u.getZ()) * (180.0f / 3.14159265358979323846f);
    float theta_y_deg = atan2(u.getX(), d) * (180.0f / 3.14159265358979323846f);

    Matrix4 T1     = Matrix4::translate(-axisPoint.getX(), -axisPoint.getY(), -axisPoint.getZ());
    Matrix4 Rx_pos = Matrix4::rotateX( theta_x_deg);
    Matrix4 Ry_neg = Matrix4::rotateY(-theta_y_deg);
    Matrix4 Rz_val = Matrix4::rotateZ(deg);
    Matrix4 Ry_pos = Matrix4::rotateY( theta_y_deg);
    Matrix4 Rx_neg = Matrix4::rotateX(-theta_x_deg);
    Matrix4 T2     = Matrix4::translate(axisPoint.getX(), axisPoint.getY(), axisPoint.getZ());

    // T(P0) * Rx(-θx) * Ry(+θy) * Rz(θ) * Ry(-θy) * Rx(+θx) * T(-P0)
    return T2.multiply(Rx_neg.multiply(Ry_pos.multiply(Rz_val.multiply(Ry_neg.multiply(Rx_pos.multiply(T1))))));
}

// ---------------------------------------------------------------------------
//  perspective
// ---------------------------------------------------------------------------
Matrix4 Matrix4::perspective(float fovDeg, float aspect,
                             float near, float far) {
    // TODO: Symmetric perspective projection matrix.
    //
    float vert_fov_Rad = toRad(fovDeg);
    float t = tan(vert_fov_Rad / 2.0f);   // half-angle tangent
    
    Matrix4 P;
    float top = near * t;
    float right = top * aspect;
    float bottom = -top;
    float left = -right;

    P.set(0,0,((2*near) / (right - left)));
    P.set(1,1,((2*near) / (top - bottom)));
    P.set(2,2,(-(far + near) / (far - near)));
    P.set(2,3,(-(2*far*near) / (far - near)));
    P.set(3,2,-1);
    
    return P;
}

// ---------------------------------------------------------------------------
//  lookAt
// ---------------------------------------------------------------------------
Matrix4 Matrix4::lookAt(const Vector3& eye, const Vector3& center,
                        const Vector3& up) {
    // TODO: Construct the view matrix.
    //
    //   1. Compute forward direction: f = normalize(center - eye)
    //      (use center.subtract(eye).normalize())
    Vector3 sub = center.subtract(eye);
    Vector3 f = sub.normalize(); // Forward directions
    //
    //   2. Compute right direction:   r = normalize(f x up)
    //      (use f.cross(up).normalize())
    Vector3 cross = f.cross(up);
    Vector3 r = cross.normalize(); // right direction
    //
    //   3. Compute true up direction: u = r x f
    //      (use r.cross(f))
    Vector3 u = r.cross(f); //true up direction
    //
    //   4. Build the matrix (row-major):
    //             0      1      2         3
    //        0[  r.x    r.y    r.z   -r.dot(eye) ]
    //        1[  u.x    u.y    u.z   -u.dot(eye) ]
    //        2[ -f.x   -f.y   -f.z    f.dot(eye) ]
    //        3[   0      0      0        1       ]

    //
    //   Note: the third row uses -f because OpenGL looks down -Z.
    //   The translation column is the dot of each basis vector with -eye.
    Matrix4 look;
    //1st row
    look.set(0,0,r.getX());
    look.set(0,1,r.getY());
    look.set(0,2,r.getZ());
    look.set(0,3,-(r.dot(eye)));

    // 2nd row
    look.set(1,0,u.getX());
    look.set(1,1,u.getY());
    look.set(1,2,u.getZ());
    look.set(1,3,-(u.dot(eye)));

    //3rd row
    look.set(2,0,-f.getX());
    look.set(2,1,-f.getY());
    look.set(2,2,-f.getZ());
    look.set(2,3,(f.dot(eye)));

    //4th row
    look.set(3,0,0);
    look.set(3,1,0);
    look.set(3,2,0);
    look.set(3,3,1);

    return look;
}
