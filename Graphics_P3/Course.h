#ifndef COURSE_H
#define COURSE_H

#include <GL/glew.h>
#include "Shape.h"
#include "MathUtils.h"
#include "ShapeBuilder.h"

struct Course {
    Shape grassSurface;
    Shape wallN, wallS, wallE, wallW;
    Shape startingMat;
    Shape startBall;
    Shape holeCylinder;
    Shape plantStem[2];
    Shape plantLeaves[2];
    Shape flagPole;
    Shape flagBanner;
    Shape vObstacleLeft;
    Shape vObstacleRight;

    void build();
    void draw(GLuint shaderID, const Matrix4& sceneTransform, bool wireframe);
};

#endif