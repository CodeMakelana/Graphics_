#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Shape.h"

/**
 * Rectangle.hpp
 * An axis-aligned rectangle defined by its centre (cx, cy),
 * half-width (hw) and half-height (hh) in NDC coordinates.
 *
 * Vertices are defined relative to centre, so the transform
 * pivot is always exactly the visual centre of the shape.
 *
 *   (-hw, +hh) ─────── (+hw, +hh)
 *       │                   │
 *   (-hw, -hh) ─────── (+hw, -hh)
 *
 * Two triangles cover the quad:
 *   Triangle 1: top-left,  bottom-left,  bottom-right
 *   Triangle 2: top-left,  bottom-right, top-right
 */
class Rectangle : public Shape
{
public:
    /**
     * @param cx      Centre X in NDC  (-1 to +1)
     * @param cy      Centre Y in NDC  (-1 to +1)
     * @param hw      Half-width  in NDC units
     * @param hh      Half-height in NDC units
     * @param colour  Base RGB colour
     */
    Rectangle(float cx, float cy, float hw, float hh, const Vec3& colour);

protected:
    void buildVertices()          override;
    void buildWireframeVertices() override;

private:
    float halfW; // half-width  in NDC
    float halfH; // half-height in NDC
};

#endif // RECTANGLE_HPP