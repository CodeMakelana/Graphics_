#include "Rectangle.h"

/**
 * Rectangle.cpp
 * Builds fill triangles and wireframe edge lines for a rectangle.
 */

Rectangle::Rectangle(float cx, float cy, float hw, float hh, const Vec3& colour)
    : Shape(cx, cy, colour)   // pass centre and colour to base class
    , halfW(hw)
    , halfH(hh)
{}

// ─────────────────────────────────────────────────────────────────────────────
//  Fill geometry — two triangles forming a quad
// ─────────────────────────────────────────────────────────────────────────────

void Rectangle::buildVertices()
{
    // Four corners relative to the centre (centreX, centreY)
    float l = centreX - halfW; // left   edge X
    float r = centreX + halfW; // right  edge X
    float b = centreY - halfH; // bottom edge Y
    float t = centreY + halfH; // top    edge Y

    // Two triangles (GL_TRIANGLES needs 6 vertices for a quad)
    // z = 0 for all 2D shapes
    vertices = {
        // Triangle 1 — bottom-left half
        l, b, 0.0f,   // bottom-left
        r, b, 0.0f,   // bottom-right
        r, t, 0.0f,   // top-right

        // Triangle 2 — top-right half
        l, b, 0.0f,   // bottom-left
        r, t, 0.0f,   // top-right
        l, t, 0.0f    // top-left
    };
}

// ─────────────────────────────────────────────────────────────────────────────
//  Wireframe geometry — four edges as GL_LINES pairs
// ─────────────────────────────────────────────────────────────────────────────

void Rectangle::buildWireframeVertices()
{
    float l = centreX - halfW;
    float r = centreX + halfW;
    float b = centreY - halfH;
    float t = centreY + halfH;

    // GL_LINES draws one line per pair of consecutive vertices
    // We need 4 edges → 8 vertices (each edge = start + end point)
    wireVertices = {
        // Bottom edge
        l, b, 0.0f,   r, b, 0.0f,
        // Right edge
        r, b, 0.0f,   r, t, 0.0f,
        // Top edge
        r, t, 0.0f,   l, t, 0.0f,
        // Left edge
        l, t, 0.0f,   l, b, 0.0f
    };
}