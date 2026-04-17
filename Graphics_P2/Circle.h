#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"

/**
 * Circle.hpp
 * A circle approximated by N evenly-spaced vertices around a centre.
 *
 * The spec requires two distinct circles:
 *   - High-poly  : numSegments >= 50  → looks like a smooth circle
 *   - Low-poly   : numSegments 6–10   → looks like a hexagon/octagon
 *
 * Fill geometry: triangle fan from centre to each edge segment.
 *   Centre + edge[i] + edge[i+1]  for i in 0..N-1  (wraps around)
 *
 * Wireframe geometry: the outer ring only — N line segments.
 *
 * @param cx           Centre X in NDC
 * @param cy           Centre Y in NDC
 * @param radius       Radius in NDC units
 * @param numSegments  Number of outer vertices (>= 3)
 * @param colour       Base RGB colour
 */
class Circle : public Shape
{
public:
    Circle(float cx, float cy, float radius, int numSegments, const Vec3& colour);

protected:
    void buildVertices()          override;
    void buildWireframeVertices() override;

private:
    float radius;
    int   numSegments;
};

#endif // CIRCLE_H