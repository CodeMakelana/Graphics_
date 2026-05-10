#ifndef SHAPEBUILDER_H
#define SHAPEBUILDER_H

#include "Shape.h"
#include <cmath>

Shape buildCuboid(float w, float h, float d, float r, float g, float b);
Shape buildFrustum(float w, float h, float d, float topScale, float r, float g, float b);
Shape buildTriangularPrism(float base, float height, float depth, float r, float g, float b);
Shape buildCylinder(float radius, float height, int segments, float r, float g, float b);
Shape buildCone(float radius, float height, int segments, float r, float g, float b);
Shape buildSphere(float radius, int stacks, int slices, float r, float g, float b);

#endif