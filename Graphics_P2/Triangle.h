#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "Shape.h"

/**
 * Triangle.hpp
 * A triangle defined by three explicit vertices in NDC coordinates.
 *
 * The centroid (average of the three vertices) is automatically
 * computed and used as the transform pivot for rotation and scale.
 *
 *        p2
 *       /  \
 *      /    \
 *    p0 ─── p1
 */
class Triangle : public Shape
{
public:
    /**
     * @param x0,y0   First  vertex in NDC
     * @param x1,y1   Second vertex in NDC
     * @param x2,y2   Third  vertex in NDC
     * @param colour  Base RGB colour
     */
    Triangle(float x0, float y0,
             float x1, float y1,
             float x2, float y2,
             const Vec3& colour);

protected:
    void buildVertices()          override;
    void buildWireframeVertices() override;

private:
    // Store the three vertices so buildVertices can use them
    float vx[3], vy[3];
};

#endif // TRIANGLE_HPP