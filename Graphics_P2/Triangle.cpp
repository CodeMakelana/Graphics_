#include "Triangle.h"

/**
 * Triangle.cpp
 * Builds fill and wireframe geometry for a triangle.
 */

Triangle::Triangle(float x0, float y0,
                   float x1, float y1,
                   float x2, float y2,
                   const Vec3& colour)
    : Shape(
        (x0 + x1 + x2) / 3.0f,   // centreX = centroid X
        (y0 + y1 + y2) / 3.0f,   // centreY = centroid Y
        colour
      )
{
    vx[0] = x0; vy[0] = y0;
    vx[1] = x1; vy[1] = y1;
    vx[2] = x2; vy[2] = y2;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Fill geometry — one triangle, three vertices
// ─────────────────────────────────────────────────────────────────────────────

void Triangle::buildVertices()
{
    vertices = {
        vx[0], vy[0], 0.0f,
        vx[1], vy[1], 0.0f,
        vx[2], vy[2], 0.0f
    };
}

// ─────────────────────────────────────────────────────────────────────────────
//  Wireframe geometry — three edges as GL_LINES pairs
// ─────────────────────────────────────────────────────────────────────────────

void Triangle::buildWireframeVertices()
{
    // Each edge = a pair of vertices: start → end
    wireVertices = {
        // Edge 0→1
        vx[0], vy[0], 0.0f,   vx[1], vy[1], 0.0f,
        // Edge 1→2
        vx[1], vy[1], 0.0f,   vx[2], vy[2], 0.0f,
        // Edge 2→0
        vx[2], vy[2], 0.0f,   vx[0], vy[0], 0.0f
    };
}