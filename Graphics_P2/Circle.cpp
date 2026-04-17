#include "Circle.h"
#include <cmath>

/**
 * Circle.cpp
 * Generates a circle as a triangle fan (fill) and an edge loop (wireframe).
 *
 * Vertex positions on the outer ring:
 *   x = cx + radius * cos(angle)
 *   y = cy + radius * sin(angle)
 *   angle = 2π * i / numSegments   for i in 0..numSegments-1
 */

// Note: M_PI may not be defined on all compilers — define it ourselves
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

Circle::Circle(float cx, float cy, float radius, int numSegments,
               const Vec3& colour)
    : Shape(cx, cy, colour)
    , radius(radius)
    , numSegments(numSegments)
{}

// ─────────────────────────────────────────────────────────────────────────────
//  Fill geometry — triangle fan
// ─────────────────────────────────────────────────────────────────────────────

void Circle::buildVertices()
{
    vertices.clear();

    float cx = centreX;
    float cy = centreY;

    // Each sector: centre + two consecutive edge points = 1 triangle = 9 floats
    for (int i = 0; i < numSegments; i++)
    {
        float angle0 = 2.0f * (float)M_PI * i       / numSegments;
        float angle1 = 2.0f * (float)M_PI * (i + 1) / numSegments;

        float x0 = cx + radius * cosf(angle0);
        float y0 = cy + radius * sinf(angle0);
        float x1 = cx + radius * cosf(angle1);
        float y1 = cy + radius * sinf(angle1);

        // Triangle: centre → edge[i] → edge[i+1]
        vertices.push_back(cx); vertices.push_back(cy); vertices.push_back(0.0f);
        vertices.push_back(x0); vertices.push_back(y0); vertices.push_back(0.0f);
        vertices.push_back(x1); vertices.push_back(y1); vertices.push_back(0.0f);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Wireframe geometry — outer ring only
// ─────────────────────────────────────────────────────────────────────────────

void Circle::buildWireframeVertices()
{
    wireVertices.clear();

    float cx = centreX;
    float cy = centreY;

    // N line segments forming the outer ring
    for (int i = 0; i < numSegments; i++)
    {
        float angle0 = 2.0f * (float)M_PI * i       / numSegments;
        float angle1 = 2.0f * (float)M_PI * (i + 1) / numSegments;

        float x0 = cx + radius * cosf(angle0);
        float y0 = cy + radius * sinf(angle0);
        float x1 = cx + radius * cosf(angle1);
        float y1 = cy + radius * sinf(angle1);

        // Line: edge[i] → edge[i+1]
        wireVertices.push_back(x0); wireVertices.push_back(y0); wireVertices.push_back(0.0f);
        wireVertices.push_back(x1); wireVertices.push_back(y1); wireVertices.push_back(0.0f);

        
        // Spoke: centre → edge[i]
        // This makes each triangle sector visible and rotation obvious.
        // 6
    }
}