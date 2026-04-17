#ifndef MAT4_H
#define MAT4_H

/**
 * Mat4.h
 * A 4x4 float matrix for 2D transformations in homogeneous coordinates.
 *
 * Storage: row-major internally (data[row][col]),
 * but toArray() outputs column-major for OpenGL's glUniformMatrix4fv.
 *
 * Transform convention (right-to-left application):
 *   Final = Translation * Rotation * Scale
 *
 * To rotate/scale around a shape's own centre (cx, cy):
 *   M = translate(cx,cy) * rotate(angle) * translate(-cx,-cy)
 */
class Mat4
{
public:
    float data[4][4]; // data[row][col]

    // ── Constructors ──────────────────────────────────────────────────────────
    Mat4();  // initialises to zero matrix

    // ── Static factory methods ─────────────────────────────────────────────────

    /** Identity matrix — the "do nothing" transform. */
    static Mat4 identity();

    /**
     * Translation matrix.
     * Moves a point by (tx, ty) in 2D (z unchanged).
     */
    static Mat4 translate(float tx, float ty);

    /**
     * Scale matrix.
     * Scales around the origin by (sx, sy).
     * To scale around a centre point, combine with translate().
     */
    static Mat4 scale(float sx, float sy);

    /**
     * Rotation matrix.
     * Rotates counter-clockwise by 'angle' radians around the origin.
     * To rotate around a centre point, combine with translate().
     */
    static Mat4 rotate(float angle);

    /**
     * Combined rotation around an arbitrary centre point (cx, cy).
     * Equivalent to: translate(cx,cy) * rotate(angle) * translate(-cx,-cy)
     */
    static Mat4 rotateAround(float angle, float cx, float cy);

    /**
     * Combined scale around an arbitrary centre point (cx, cy).
     * Equivalent to: translate(cx,cy) * scale(sx,sy) * translate(-cx,-cy)
     */
    static Mat4 scaleAround(float sx, float sy, float cx, float cy);

    // ── Operators ─────────────────────────────────────────────────────────────

    /** Matrix multiplication — combines two transforms into one. */
    Mat4 operator*(const Mat4& other) const;

    // ── OpenGL output ─────────────────────────────────────────────────────────

    /**
     * Returns a flat float[16] array in COLUMN-MAJOR order for OpenGL.
     * OpenGL's glUniformMatrix4fv expects column-major layout.
     *
     * Column-major means: index 0..3 = column 0, index 4..7 = column 1, etc.
     * i.e. out[col*4 + row] = data[row][col]
     */
    void toArray(float out[16]) const;
};

#endif // MAT4_H