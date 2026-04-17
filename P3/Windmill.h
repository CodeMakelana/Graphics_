#ifndef WINDMILL_H
#define WINDMILL_H

#include <GL/glew.h>
#include "Shape.h"
#include "MathUtils.h"
#include "ShapeBuilder.h"

struct Windmill {
    Shape body;
    Shape legLeft;
    Shape legRight;
    Shape roof;
    Shape axle;
    Shape bladeStems[4];
    Shape bladeSails[4];

    // Details
    Shape windowLeft;
    Shape windowRight;

    float bladeAngle;
    float bladeSpeed;
    Matrix4 localTransform;

    void build();
    void update(float dt);
    void draw(GLuint shaderID, const Matrix4& sceneTransform, bool wireframe);
};

#endif