#ifndef COURSE_H
#define COURSE_H

#include <GL/glew.h>
#include "Shape.h"
#include "MathUtils.h"
#include "ShapeBuilder.h"

struct Course {
    // Playing surface
    Shape grassSurface;

    // Boundary walls
    Shape wallNorth;
    Shape wallSouth;
    Shape wallEast;
    Shape wallWest;

    // Tee box
    Shape startingMat;

    // Hole
    Shape holeCylinder;
    Shape holeRim;

    // Decorative plants (stem + cone canopy each)
    Shape plant1Stem;
    Shape plant2Stem;
    Shape plant3Stem;
    Shape plant1Cone;
    Shape plant2Cone;
    Shape plant3Cone;

    // Flag at the hole
    Shape flagPole;
    Shape flagBanner;

    // Create all shapes and call upload() on each
    void build();

    // Send geometry to the GPU
    void draw(GLuint shaderID, const Matrix4& sceneTransform, bool wireframe);
};

#endif // COURSE_H
