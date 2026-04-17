#include "Mat4.h"
#include <cmath>

/**
 * Mat4.cpp
 * Full implementation of the 4x4 transformation matrix.
 * No GLM math functions used — all math is implemented from first principles.
 */

// ── Constructor ───────────────────────────────────────────────────────────────

Mat4::Mat4()
{
    // Initialise all elements to zero
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            data[r][c] = 0.0f;
}

// ── Static factory methods ────────────────────────────────────────────────────

Mat4 Mat4::identity()
{
    Mat4 m;
    // Place 1s on the main diagonal
    m.data[0][0] = 1.0f;
    m.data[1][1] = 1.0f;
    m.data[2][2] = 1.0f;
    m.data[3][3] = 1.0f;
    return m;
}

Mat4 Mat4::translate(float tx, float ty)
{
    // Start with identity so unchanged components pass through
    Mat4 m = identity();

    // Translation lives in the last column (column 3)
    // Row 0, col 3 → x offset
    // Row 1, col 3 → y offset
    m.data[0][3] = tx;
    m.data[1][3] = ty;

    return m;
}

Mat4 Mat4::scale(float sx, float sy)
{
    Mat4 m = identity();

    // Scale factors on the diagonal
    m.data[0][0] = sx;
    m.data[1][1] = sy;
    // data[2][2] stays 1.0 (z unchanged), data[3][3] stays 1.0

    return m;
}

Mat4 Mat4::rotate(float angle)
{
    Mat4 m = identity();

    float c = cosf(angle); // cosine of angle
    float s = sinf(angle); // sine of angle

    // Standard 2D rotation embedded in a 4x4 matrix
    //   [ cos  -sin  0  0 ]
    //   [ sin   cos  0  0 ]
    //   [  0     0   1  0 ]
    //   [  0     0   0  1 ]
    m.data[0][0] =  c;
    m.data[0][1] = -s;
    m.data[1][0] =  s;
    m.data[1][1] =  c;

    return m;
}

Mat4 Mat4::rotateAround(float angle, float cx, float cy)
{
    // Three-step transform:
    //   1. Move centre to origin:  translate(-cx, -cy)
    //   2. Rotate:                 rotate(angle)
    //   3. Move back:              translate(cx, cy)
    //
    // Combined (right-to-left): T(cx,cy) * R(angle) * T(-cx,-cy)
    Mat4 toOrigin   = translate(-cx, -cy);
    Mat4 rot        = rotate(angle);
    Mat4 toBack     = translate(cx, cy);

    return toBack * rot * toOrigin;
}

Mat4 Mat4::scaleAround(float sx, float sy, float cx, float cy)
{
    // Same three-step pattern as rotateAround:
    //   T(cx,cy) * S(sx,sy) * T(-cx,-cy)
    Mat4 toOrigin = translate(-cx, -cy);
    Mat4 sc       = scale(sx, sy);
    Mat4 toBack   = translate(cx, cy);

    return toBack * sc * toOrigin;
}

// ── Operators ─────────────────────────────────────────────────────────────────

Mat4 Mat4::operator*(const Mat4& other) const
{
    Mat4 result; // initialised to all zeros

    // Standard matrix multiplication: result[r][c] = dot(row r, col c)
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            float sum = 0.0f;
            for (int k = 0; k < 4; k++)
            {
                sum += data[r][k] * other.data[k][c];
            }
            result.data[r][c] = sum;
        }
    }

    return result;
}

// ── OpenGL output ─────────────────────────────────────────────────────────────

void Mat4::toArray(float out[16]) const
{
    // OpenGL expects COLUMN-MAJOR order:
    // out[col * 4 + row] = data[row][col]
    //
    // i.e. the first 4 values are column 0 (top to bottom),
    //      the next 4 are column 1, and so on.
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            out[c * 4 + r] = data[r][c];
}